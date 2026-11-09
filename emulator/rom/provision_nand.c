/* Runs only in the provisioning VM, at a debugger breakpoint in thread
 * context. Stock flash driver writes its own translation tables and OOB. */
#include <stdint.h>
#include <stdbool.h>

struct resource {
    uint32_t offset, length, source;
};
struct flash_api {
    int (*read)(const void *, uint32_t, void *, uint32_t);
    int (*write)(const void *, uint32_t, const void *, uint32_t);
    uint32_t unused[4];
    int (*flush)(const void *, bool);
};

static __attribute__((noreturn)) void finish(unsigned stage)
{
    *(volatile uint32_t *)0x400ffff4 = stage;
    for (;;) {}
}

__attribute__((section(".text.entry"), noreturn))
void provision(void)
{
    const void *dev = (const void *)0x101df000;
    const struct flash_api *api = (const struct flash_api *)0x38001c10;
    const struct resource *table = (const struct resource *)0x20010000;
    uint8_t *scratch = (uint8_t *)0x20001000;
    const unsigned chunk = 32768;

    for (unsigned i = 0; table[i].length; i++) {
        const struct resource *res = &table[i];
        const uint8_t *src = (const uint8_t *)res->source;
        for (unsigned pos = 0; pos < res->length; pos += chunk) {
            unsigned n = res->length - pos;
            if (n > chunk) {
                n = chunk;
            }
            if (api->write(dev, res->offset + pos, src + pos, n)) {
                finish(100 + i);
            }
        }
        if (api->flush(dev, false)) {
            finish(200 + i);
        }
        for (unsigned pos = 0; pos < res->length; pos += chunk) {
            unsigned n = res->length - pos;
            if (n > chunk) {
                n = chunk;
            }
            if (api->read(dev, res->offset + pos, scratch, n)) {
                finish(300 + i);
            }
            for (unsigned j = 0; j < n; j++) {
                if (scratch[j] != src[pos + j]) {
                    finish(400 + i);
                }
            }
        }
    }
    const uint32_t flags = *(const uint32_t *)0x200101fc;
    if (flags & 1) {
        /* At the provisioning breakpoint stock already populated the working
         * copies with its default settings and user data. Commit both using
         * stock's data_storage_write(type, index, object) entry point. */
        int (*write_object)(unsigned, int, const void *) = (void *)0x100c366d;
        if (flags & 2) {
            int (*set_setting)(unsigned, const void *, unsigned) = (void *)0x100e08b9;
            uint32_t confirmed = 1;
            set_setting(0x51, &confirmed, sizeof(confirmed));
        }
        if (write_object(0, -1, 0) < 0 || write_object(1, -1, 0) < 0 ||
            api->flush(dev, false)) {
            finish(500);
        }
    }
    finish(0);
}
