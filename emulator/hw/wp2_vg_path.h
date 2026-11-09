/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <math.h>
typedef struct { float x, y; } Wp2PathPoint;
typedef struct { Wp2PathPoint a, b; } Wp2PathEdge;
typedef struct {
    Wp2PathEdge edges[1024];
    unsigned count;
    bool even_odd;
} Wp2Path;

/* Inline S32 VG Lite paths, with curves flattened into line segments. */
static bool wp2_vg_decode_path(const uint32_t *r, const uint8_t *words,
                               unsigned count, Wp2Path *path)
{
    if (r[0xa10] != 0x101 || (r[0xa34] & 3) != 3 ||
        r[0xa3b] != 0x3f800000 || r[0xa40] != 0x3f800000 || r[0xa44] != 0x3f800000 ||
        r[0xa3c] || r[0xa41] || r[0xa42] || r[0xa43] || r[0xa45] || count > 512) {
        return false;
    }
    Wp2PathEdge *edges = path->edges;
    Wp2PathPoint current = {0}, first = {0};
    unsigned n = 0;
    bool open = false, ended = false;
    for (unsigned i = 0; i < count;) {
        unsigned op = ldl_le_p(words + i++ * 4);
        if (op <= 3 && open) {
            if (n == ARRAY_SIZE(path->edges)) return false;
            edges[n++] = (Wp2PathEdge){current, first};
            open = false;
        }
        if (op == 0) {
            ended = true;
            break;
        }
        if (op == 1) {
            current = first;
            continue;
        }
        if (op > 9) return false;
        unsigned points = op >= 8 ? 3 : op >= 6 ? 2 : 1;
        if (i + 2 * points > count || (op >= 4 && !open)) return false;
        Wp2PathPoint p[3];
        for (unsigned j = 0; j < points; j++) {
            p[j].x = (int32_t)ldl_le_p(words + i++ * 4);
            p[j].y = (int32_t)ldl_le_p(words + i++ * 4);
            if (op & 1) {
                p[j].x += current.x;
                p[j].y += current.y;
            }
            if (fabsf(p[j].x) > 4096 || fabsf(p[j].y) > 4096) return false;
        }
        if (op <= 3) {
            first = current = p[0];
            open = true;
            continue;
        }
        unsigned segments = points == 1 ? 1 : 12;
        if (n + segments > ARRAY_SIZE(path->edges)) return false;
        Wp2PathPoint last = current;
        for (unsigned j = 1; j <= segments; j++) {
            float t = (float)j / segments, u = 1 - t;
            Wp2PathPoint next = p[points - 1];
            if (points == 2) {
                next.x = u*u*current.x + 2*u*t*p[0].x + t*t*p[1].x;
                next.y = u*u*current.y + 2*u*t*p[0].y + t*t*p[1].y;
            } else if (points == 3) {
                next.x = u*u*u*current.x + 3*u*u*t*p[0].x + 3*u*t*t*p[1].x + t*t*t*p[2].x;
                next.y = u*u*u*current.y + 3*u*u*t*p[0].y + 3*u*t*t*p[1].y + t*t*t*p[2].y;
            }
            edges[n++] = (Wp2PathEdge){last, next};
            last = next;
        }
        current = p[points - 1];
    }
    path->count = n;
    path->even_odd = r[0xa34] & 0x10;
    return ended;
}

static void wp2_vg_path_row(const Wp2Path *path, unsigned y, unsigned x0,
                             unsigned x1, int *winding)
{
    float yc = y + 0.5f;
    for (unsigned i = 0; i < path->count; i++) {
        Wp2PathPoint a = path->edges[i].a, b = path->edges[i].b;
        if ((a.y > yc) == (b.y > yc)) continue;
        float intersection = a.x + (yc - a.y) * (b.x - a.x) / (b.y - a.y);
        int x = CLAMP((int)ceilf(intersection - 0.5f), (int)x0, (int)x1);
        winding[x - x0] += b.y > a.y ? 1 : -1;
    }
}

static bool wp2_vg_solid_path(const uint32_t *r, const Wp2Path *path)
{
    unsigned width = r[0xa13] & 0xffff, height = r[0xa13] >> 16, pitch = r[0xa12];
    uint32_t base = r[0xa11], color = r[0xa02];
    if (!width || !height || width > 466 || height > 466 || pitch < width * 2 ||
        pitch > 16384 || base < 0x18000000 ||
        (uint64_t)base + height * pitch > 0x40000000) return false;
    unsigned x0 = r[0xa39] & 0xffff, y0 = r[0xa39] >> 16;
    unsigned x1 = MIN(width, x0 + (r[0xa3a] & 0xffff));
    unsigned y1 = MIN(height, y0 + (r[0xa3a] >> 16));
    uint16_t rgb = ((color & 0xf8) << 8) | ((color >> 5) & 0x7e0) | ((color >> 19) & 31);
    if (x0 >= x1 || y0 >= y1) return true;
    for (unsigned y = y0; y < y1; y++) {
        uint8_t row[466 * 2];
        int winding[467] = {0};
        wp2_vg_path_row(path, y, x0, x1, winding);
        uint32_t addr = base + y * pitch + x0 * 2;
        if (address_space_read(&address_space_memory, addr, MEMTXATTRS_UNSPECIFIED,
                               row, (x1 - x0) * 2) != MEMTX_OK) return false;
        int inside = 0;
        for (unsigned x = x0; x < x1; x++) {
            inside += winding[x - x0];
            if (path->even_odd ? inside % 2 != 0 : inside != 0) {
                uint16_t pixel = rgb;
                if (r[0xa00] & 0x100) {
                    unsigned alpha = color >> 24;
                    uint16_t old = lduw_le_p(row + (x - x0) * 2);
                    unsigned red = (((rgb >> 11) * alpha) +
                                    (old >> 11) * (255 - alpha) + 127) / 255;
                    unsigned green = ((((rgb >> 5) & 63) * alpha) +
                                      ((old >> 5) & 63) * (255 - alpha) + 127) / 255;
                    unsigned blue = (((rgb & 31) * alpha) +
                                     (old & 31) * (255 - alpha) + 127) / 255;
                    pixel = (red << 11) | (green << 5) | blue;
                }
                stw_le_p(row + (x - x0) * 2, pixel);
            }
        }
        if (address_space_write(&address_space_memory, addr, MEMTXATTRS_UNSPECIFIED,
                                row, (x1 - x0) * 2) != MEMTX_OK) return false;
    }
    return true;
}
