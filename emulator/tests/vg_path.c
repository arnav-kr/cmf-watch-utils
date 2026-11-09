#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define CLAMP(x,a,b) ((x) < (a) ? (a) : (x) > (b) ? (b) : (x))
#define MEMTXATTRS_UNSPECIFIED 0
#define MEMTX_OK 0
static int address_space_memory;
static uint8_t memory[32];
static uint32_t ldl_le_p(const uint8_t *p) {
    return p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}
static uint16_t lduw_le_p(const uint8_t *p) { return p[0] | p[1] << 8; }
static void stw_le_p(uint8_t *p, uint16_t v) { p[0] = v; p[1] = v >> 8; }
static int address_space_read(int *s, uint32_t a, int attrs, void *p, unsigned n) {
    (void)s; (void)attrs;
    if (a < 0x18000000 || (uint64_t)a + n > 0x18000020) return 1;
    memcpy(p, memory + a - 0x18000000, n); return 0;
}
static int address_space_write(int *s, uint32_t a, int attrs, const void *p, unsigned n) {
    (void)s; (void)attrs;
    if (a < 0x18000000 || (uint64_t)a + n > 0x18000020) return 1;
    memcpy(memory + a - 0x18000000, p, n); return 0;
}
#include "../hw/wp2_vg_path.h"
int main(void) {
    uint32_t r[0x1000] = {0};
    uint32_t words[] = {2,1,1,4,3,1,4,3,3,4,1,3,0};
    Wp2Path path;
    r[0xa00] = 0x102; r[0xa10] = 0x101; r[0xa11] = 0x18000000;
    r[0xa12] = 8; r[0xa13] = r[0xa3a] = 0x00040004;
    r[0xa34] = 3; r[0xa3b] = r[0xa40] = r[0xa44] = 0x3f800000;
    assert(wp2_vg_decode_path(r, (uint8_t *)words, ARRAY_SIZE(words), &path));
    unsigned alphas[] = {0,128,255};
    unsigned expected[] = {0x001f,0x800f,0xf800};
    for (unsigned j = 0; j < ARRAY_SIZE(alphas); j++) {
        for (unsigned i = 0; i < 16; i++) stw_le_p(memory + 2*i, 0x001f);
        r[0xa02] = alphas[j] << 24 | 0x000000ff;
        assert(wp2_vg_solid_path(r, &path));
        for (unsigned y = 0; y < 4; y++) for (unsigned x = 0; x < 4; x++)
            assert(lduw_le_p(memory + 2*(4*y+x)) ==
                   (x >= 1 && x < 3 && y >= 1 && y < 3 ? expected[j] : 0x001f));
    }
    r[0xa00] = 0x10000002; r[0xa02] = 0x000000ff;
    assert(wp2_vg_solid_path(r, &path));
    assert(lduw_le_p(memory + 10) == 0xf800);
    r[0xa12] = 1;
    assert(!wp2_vg_solid_path(r, &path));
    puts("VG path alpha, opaque fill, coverage and bounds passed");
}
