/* Run in the isolated stock guest after the stock NAND driver initializes. */
#include <stdint.h>

__attribute__((noreturn, noinline))
void stage_ready(int status)
{
    *(volatile int *)0x200000fc = status;
    for (;;) {
        __asm__ volatile ("nop");
    }
}

__attribute__((noreturn))
void read_ota_temp(void)
{
    const void *device = (const void *)0x101df000;
    int (*read)(const void *, uint32_t, void *, uint32_t) =
        *(int (**)(const void *, uint32_t, void *, uint32_t))0x38001c10;
    const unsigned chunk = 32768;
    const unsigned length = 0x380000;
    uint8_t *output = (uint8_t *)0x20010000;
    for (unsigned offset = 0; offset < length; offset += chunk) {
        unsigned size = length - offset;
        if (size > chunk) {
            size = chunk;
        }
        int rc = read(device, offset, output + offset, size);
        if (rc) {
            stage_ready(rc);
        }
    }
    stage_ready(0);
}
