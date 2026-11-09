/* SPDX-License-Identifier: GPL-2.0-or-later */
/* RGB565 composition and packed alpha masks from the Leopard DE registers. */
typedef struct {
    unsigned x, y, width, height, stride, bits, bit_offset;
    bool enabled, no_stride, alpha, indexed, msb_first, byte_swap, rb_swap;
    bool scaled;
    unsigned src_width, src_height, step_x, step_y, start_x, start_y;
    uint32_t address, color;
} Wp2DeLayer;

static bool wp2_de_layer(Wp2DeLayer *layer, const uint32_t *r, unsigned i)
{
    const uint32_t *reg = &r[(i == 2 ? 0x700 : (i + 1) * 0x100) / 4];
    uint32_t ctl = r[(i == 2 ? 0x24 : 0) / 4];
    unsigned fmt = (ctl >> (i ? 21 : 15)) & 7;
    unsigned alpha_cfg = r[0x340 / 4] >> (8 * i);
    static const unsigned alpha_bits[] = {8, 4, 1, 2};
    *layer = (Wp2DeLayer) {
        .enabled = !!(ctl & (1u << (i ? 26 : 20))),
        .x = reg[0] & 0xffff, .y = reg[0] >> 16,
        .width = (reg[1] & 0xffff) + 1, .height = (reg[1] >> 16) + 1,
        .address = reg[2], .stride = reg[3], .color = reg[7],
        .alpha = fmt == 7,
        .indexed = fmt == 7 && (alpha_cfg & 8),
        .bits = fmt == 7 ? alpha_bits[alpha_cfg & 3] : 16,
        .bit_offset = fmt == 7 ? (alpha_cfg >> 4) & 7 : 0,
        .msb_first = !!(alpha_cfg & 4),
        .no_stride = !!(ctl & (1u << (i ? 30 : 31))),
        .byte_swap = !(ctl & (1u << (i ? 27 : 14))),
        .rb_swap = !!(ctl & (1u << (i ? 24 : 18))),
        .scaled = i != 0 && (ctl & 1),
        .src_width = (r[0x600 / 4] & 0xffff) + 1,
        .src_height = (r[0x600 / 4] >> 16) + 1,
        .step_x = r[0x604 / 4] & 0xffff, .step_y = r[0x604 / 4] >> 16,
        .start_x = r[0x608 / 4] & 0xffff, .start_y = r[0x608 / 4] >> 16,
    };
    if (!layer->enabled) {
        return true;
    }
    return (fmt == 0 || fmt == 7) && layer->bit_offset % layer->bits == 0 &&
           !(ctl & (1u << (i ? 25 : 19))) &&
           (!i || !(ctl & (1u << 28))) &&
           (!i || !(ctl & 2)) &&
           (!layer->scaled || (fmt == 0 && !layer->no_stride &&
                               layer->src_width <= 466 && layer->src_height <= 466 &&
                               layer->stride >= layer->src_width * 2)) &&
           layer->width <= 466 && layer->height <= 466 &&
           layer->stride <= 16384;
}

static uint16_t wp2_de_pixel(const Wp2DeLayer *layer, const uint8_t *bytes)
{
    uint16_t pixel = lduw_le_p(bytes);
    if (layer->byte_swap) {
        pixel = bswap16(pixel);
    }
    if (layer->rb_swap) {
        pixel = (pixel & 0x7e0) | (pixel >> 11) | ((pixel & 31) << 11);
    }
    return pixel;
}

static bool wp2_de_scaled_row(const Wp2DeLayer *layer, unsigned y,
                              unsigned count, uint16_t *row)
{
    enum { ONE = 4096 };
    unsigned sy = MIN(layer->start_y + (y - layer->y) * layer->step_y,
                      (layer->src_height - 1) * ONE);
    unsigned y0 = sy / ONE, y1 = MIN(y0 + 1, layer->src_height - 1);
    unsigned fy = sy % ONE;
    uint8_t top[466 * 2], bottom[466 * 2];
    unsigned bytes = layer->src_width * 2;
    uint64_t base = layer->address;
    if (base + y1 * layer->stride + bytes > 0x40000000 ||
        address_space_read(&address_space_memory, base + y0 * layer->stride,
                           MEMTXATTRS_UNSPECIFIED, top, bytes) != MEMTX_OK ||
        address_space_read(&address_space_memory, base + y1 * layer->stride,
                           MEMTXATTRS_UNSPECIFIED, bottom, bytes) != MEMTX_OK) {
        return false;
    }
    for (unsigned x = 0; x < count; x++) {
        unsigned sx = MIN(layer->start_x + x * layer->step_x,
                          (layer->src_width - 1) * ONE);
        unsigned x0 = sx / ONE, x1 = MIN(x0 + 1, layer->src_width - 1);
        unsigned fx = sx % ONE;
        uint16_t p00 = wp2_de_pixel(layer, top + x0 * 2);
        uint16_t p01 = wp2_de_pixel(layer, top + x1 * 2);
        uint16_t p10 = wp2_de_pixel(layer, bottom + x0 * 2);
        uint16_t p11 = wp2_de_pixel(layer, bottom + x1 * 2);
        static const unsigned shifts[] = {0, 5, 11};
        static const unsigned masks[] = {31, 63, 31};
        uint16_t rgb = 0;
        for (unsigned c = 0; c < ARRAY_SIZE(shifts); c++) {
            unsigned shift = shifts[c], mask = masks[c];
            unsigned a = ((p00 >> shift) & mask) * (ONE - fx) +
                         ((p01 >> shift) & mask) * fx;
            unsigned b = ((p10 >> shift) & mask) * (ONE - fx) +
                         ((p11 >> shift) & mask) * fx;
            rgb |= ((a * (ONE - fy) + b * fy + ONE * ONE / 2) >> 24) << shift;
        }
        row[x] = rgb;
    }
    return true;
}

static bool wp2_de_row(const Wp2DeLayer *layer, unsigned y, unsigned width,
                       unsigned plane_alpha, const uint32_t *palette, uint16_t *row)
{
    if (!layer->enabled || y < layer->y || y >= layer->y + layer->height ||
        layer->x >= width) {
        return true;
    }
    unsigned count = MIN(layer->width, width - layer->x);
    if (layer->scaled) {
        return wp2_de_scaled_row(layer, y, count, row + layer->x);
    }
    unsigned pitch = layer->no_stride ? layer->width * layer->bits : layer->stride * 8;
    unsigned offset = (y - layer->y) * pitch + layer->bit_offset;
    unsigned start = offset % 8;
    unsigned length = (start + count * layer->bits + 7) / 8;
    uint8_t bytes[466 * 2 + 1];
    uint64_t address = (uint64_t)layer->address + offset / 8;
    if (address + length > 0x40000000 ||
        address_space_read(&address_space_memory, address, MEMTXATTRS_UNSPECIFIED,
                           bytes, length) != MEMTX_OK) {
        return false;
    }
    for (unsigned x = 0; x < count; x++) {
        uint16_t *dst = row + layer->x + x;
        if (!layer->alpha) {
            *dst = wp2_de_pixel(layer, bytes + x * 2);
            continue;
        }
        unsigned pos = start + x * layer->bits;
        unsigned shift = pos % 8;
        if (layer->msb_first) shift = 8 - layer->bits - shift;
        unsigned mask = (1u << layer->bits) - 1;
        unsigned sample = (bytes[pos / 8] >> shift) & mask;
        uint32_t color = layer->indexed ? palette[sample] : layer->color;
        unsigned alpha = layer->indexed ? (color >> 24) * plane_alpha / 255 :
                                          sample * plane_alpha / mask;
        unsigned r = (((color >> 19) & 31) * alpha + (*dst >> 11) * (255 - alpha) + 127) / 255;
        unsigned g = (((color >> 10) & 63) * alpha + ((*dst >> 5) & 63) * (255 - alpha) + 127) / 255;
        unsigned b = (((color >> 3) & 31) * alpha + (*dst & 31) * (255 - alpha) + 127) / 255;
        *dst = (r << 11) | (g << 5) | b;
    }
    return true;
}
