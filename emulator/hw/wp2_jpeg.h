/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "wp2-jpeg/tjpgd.h"

typedef struct {
    const uint8_t *input;
    size_t remaining;
    uint8_t *output;
    unsigned pitch, bytes, x, y, w, h;
} Wp2Jpeg;

static size_t wp2_jpeg_read(JDEC *decoder, uint8_t *buffer, size_t size)
{
    Wp2Jpeg *io = decoder->device;
    size = MIN(size, io->remaining);
    if (buffer) memcpy(buffer, io->input, size);
    io->input += size;
    io->remaining -= size;
    return size;
}

static int wp2_jpeg_pixels(JDEC *decoder, void *pixels, JRECT *rect)
{
    Wp2Jpeg *io = decoder->device;
    unsigned stride = (rect->right - rect->left + 1) * 3;
    for (unsigned y = MAX(rect->top, io->y); y <= rect->bottom && y < io->y + io->h; y++) {
        for (unsigned x = MAX(rect->left, io->x); x <= rect->right && x < io->x + io->w; x++) {
            const uint8_t *p = (uint8_t *)pixels + (y - rect->top) * stride + (x - rect->left) * 3;
            uint8_t *d = io->output + (y - io->y) * io->pitch + (x - io->x) * io->bytes;
            if (io->bytes == 2) {
                stw_le_p(d, ((p[0] & 0xf8) << 8) | ((p[1] & 0xfc) << 3) | (p[2] >> 3));
            } else {
                d[0] = p[2];
                d[1] = p[1];
                d[2] = p[0];
            }
        }
    }
    return 1;
}

/* Input SDMA begins at the entropy-coded scan. Recover the contiguous
 * JPEG header from the guest buffer and require its SOS to end at SADDR. */
static bool wp2_jpeg_matches_scan(const uint8_t *jpeg, unsigned scan)
{
    for (unsigned pos = 2; pos + 4 <= scan; ) {
        if (jpeg[pos++] != 0xff) return false;
        while (pos < scan && jpeg[pos] == 0xff) pos++;
        if (pos + 2 >= scan) return false;
        unsigned marker = jpeg[pos++];
        unsigned length = lduw_be_p(jpeg + pos);
        if (length < 2 || pos + length > scan) return false;
        pos += length;
        if (marker == 0xda) return pos == scan;
    }
    return false;
}

static bool wp2_jpeg_decode(uint32_t *r)
{
    uint32_t *in = r + 0x78300 / 4, *out = r + 0x78400 / 4;
    uint32_t *coupled = r + 0x78500 / 4, *line = r + 0x78c60 / 4;
    uint32_t *coupled_line = r + 0x78c80 / 4, *hw = r + 0x94000 / 4;
    bool coupled_mode = r[0x780a0 / 4] != 0;
    uint32_t dst = coupled_mode ? coupled[4] : out[4];
    unsigned pitch = coupled_mode ? coupled_line[3] : line[3];
    unsigned width = hw[6] & 65535, height = hw[6] >> 16;
    unsigned bytes = (out[0] & (1u << 20)) ? 2 : 3;
    unsigned length = in[6], scan = in[2];
    unsigned prefix = 4096;
    if (in[0] != 0x100 || (out[0] & ~(1u << 20)) != 0x01000001 ||
        !(in[1] & out[1] & 1) || width == 0 || height == 0 ||
        width > 512 || height > 512 || pitch < width * bytes ||
        pitch > 8192 || line[0] != width * bytes || line[1] != height ||
        dst < 0x18000000 || (uint64_t)dst + pitch * height > 0x40000000 ||
        scan < prefix || length < 2 || length > 0x100000 ||
        (uint64_t)scan + length > 0x40000000) {
        return false;
    }
    g_autofree uint8_t *input = g_malloc(prefix + length);
    if (address_space_read(&address_space_memory, scan - prefix, MEMTXATTRS_UNSPECIFIED,
                           input, prefix + length) != MEMTX_OK) {
        return false;
    }
    unsigned start = prefix;
    if (input[start] != 0xff || input[start + 1] != 0xd8) {
        for (start = prefix - 2; start > 0; start--) {
            if (input[start] == 0xff && input[start + 1] == 0xd8 &&
                wp2_jpeg_matches_scan(input + start, prefix - start)) break;
        }
        if (!start && (input[0] != 0xff || input[1] != 0xd8 ||
                       !wp2_jpeg_matches_scan(input, prefix))) return false;
    }
    g_autofree uint8_t *output = g_malloc0(pitch * height);
    g_autofree void *pool = g_malloc(16384);
    JDEC decoder;
    Wp2Jpeg io = {
        .input = input + start, .remaining = prefix + length - start,
        .output = output, .pitch = pitch, .bytes = bytes,
        .x = hw[8] & 65535, .y = hw[8] >> 16, .w = width, .h = height,
    };
    if (jd_prepare(&decoder, wp2_jpeg_read, pool, 16384, &io) != JDR_OK ||
        decoder.width > 1024 || decoder.height > 1024 ||
        io.x + width > decoder.width || io.y + height > decoder.height ||
        jd_decomp(&decoder, wp2_jpeg_pixels, 0) != JDR_OK) {
        return false;
    }
    for (unsigned y = 0; y < height; y++) {
        if (address_space_write(&address_space_memory, dst + y * pitch, MEMTXATTRS_UNSPECIFIED,
                                output + y * pitch, width * bytes) != MEMTX_OK) {
            return false;
        }
    }
    in[1] = in[7] = out[1] = out[7] = 0;
    coupled[1] = coupled[7] = 0;
    r[0x780ac / 4] = 0;
    return true;
}
