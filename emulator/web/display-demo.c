#include <stdint.h>

#define REG(addr) (*(volatile uint32_t *)(addr))
#define SIDE 466
#define STRIDE (SIDE * sizeof(uint16_t))

static uint16_t *const pixels = (void *)0x38000000;

static void command(unsigned code, uint32_t value)
{
    REG(0x40064034) = 0x02000000 | (code << 8);
    REG(0x4006402c) = 1u << 28;
    REG(0x40064020) = value;
}

static void display(void)
{
    command(0x2a, (8u << 16) | (SIDE + 7));
    command(0x2b, SIDE - 1);
    REG(0x40064008) = ((SIDE - 1) << 16) | (SIDE - 1);
    REG(0x40078108) = (uintptr_t)pixels;
    REG(0x40078c08) = STRIDE;
    REG(0x4006402c) = (1u << 31) | (2u << 10);
}

static void draw(unsigned accent)
{
    const uint16_t colors[] = {0xfbe0, 0x4e9f, 0xffbf, 0xb6e4};
    for (int y = 0; y < SIDE; ++y) {
        for (int x = 0; x < SIDE; ++x) {
            int dx = x - SIDE / 2;
            int dy = y - SIDE / 2;
            int radius2 = dx * dx + dy * dy;
            uint16_t color = 0x0862;
            if (radius2 >= 187 * 187 && radius2 <= 196 * 196) {
                color = colors[accent];
            } else if (radius2 < 150 * 150) {
                color = ((x / 32) ^ (y / 32)) & 1 ? 0x2145 : 0x18c4;
                if (dx > -100 && dx < 100 && dy > -18 && dy < 18) {
                    color = colors[accent];
                }
            }
            pixels[y * SIDE + x] = color;
        }
    }
    display();
}

static void tick(void)
{
}

static void boot(void)
{
    unsigned accent = 0;
    unsigned last_pressed = 0;
    command(0x29, 0);
    command(0x51, 255);
    draw(accent);
    REG(0xe000ed08) = 0x10000200;
    REG(0xe000e014) = 960000 - 1;
    REG(0xe000e018) = 0;
    REG(0xe000e010) = 7;
    __asm__ volatile("cpsie i");
    for (;;) {
        unsigned pressed = REG(0x40004108) & (1u << 17);
        if (pressed && !last_pressed) {
            accent = (accent + 1) % 4;
            draw(accent);
        }
        last_pressed = pressed;
        __asm__ volatile("wfi");
    }
}

__attribute__((section(".vectors"), used))
const uintptr_t vectors[16] = {0x2ff70000, (uintptr_t)boot, [15] = (uintptr_t)tick};
