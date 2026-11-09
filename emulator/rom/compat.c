/* ROM ABI compatibility, not a dump of the Actions ROM.
 * The stock app remains unchanged. Only pure libc calls are provided initially.
 * Layout source: SDK soc/arm/actions/leopard/brom_interface.h.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <display/sw_math.h>
extern int compat_cbvprintf(int (*emit)(int,void *),void *,const char *,va_list);
extern int compat_vsnprintf(char *,size_t,const char *,va_list);
extern int LZ4_decompress_safe(const char *, char *, int, int);

/* Displaced stock 1.0.0.73 VC9202 handler prologue. Continue it unchanged
 * when virtual readings are disabled or stock is in another optical mode. */
__attribute__((naked, noinline)) static void stock_ppg(unsigned sport, unsigned spo2_mode)
{
    __asm__ volatile(".inst.w 0xe92d47f0\n"
                     ".inst.w 0xed2d8b02\n"
                     "ldr.w pc, =0x10104b95");
}

__attribute__((section(".health_entry"), used))
void compat_ppg(unsigned sport, unsigned spo2_mode)
{
    uint32_t readings = *(volatile uint32_t *)0x400fffe0;
    uint8_t mode = *(volatile uint8_t *)0x2ffb1301;
    if (readings && mode == 1) {
        void (*heart)(unsigned, unsigned, unsigned) = (void *)0x10197179;
        heart(readings & 255, 0, 0);
    } else if (readings && mode == 2) {
        void (*spo2)(unsigned) = (void *)0x1019735d;
        spo2((readings >> 8) & 255);
    } else {
        stock_ppg(sport, spo2_mode);
    }
}

__attribute__((noreturn)) static void unsupported(void)
{
    *(volatile uint32_t *)0x400ffff0 = 0x524f4d;
    for (;;) {}
}

static unsigned checksum(void *data, unsigned count)
{
    const uint32_t *words=data;
    unsigned sum=0;
    while(count >= 4) { sum+=*words++; count-=4; }
    return sum;
}

/* Function-pointer tables are words in the firmware ABI. */
#define FN(f) ((uintptr_t)(f))
/* Bootstrap NOR ABI. 0x11000000 is an emulator-only backing window, NOT
 * a claimed ATS3089C mapping. All writes are volatile, never host flash. */
#define NOR ((volatile unsigned char *)0x11000000)
#define NOR_SIZE 0x400000u
static unsigned nor_chipid(void *s) { (void)s; return 0x1640ef; }
static unsigned nor_status(void *s, unsigned char cmd)
{
    (void)s;
    if(cmd==5) return NOR[NOR_SIZE];
    if(cmd==0x35) return NOR[NOR_SIZE+1];
    if(cmd==0x15) return NOR[NOR_SIZE+2];
    unsupported();
}
static void nor_write_status(void *s,unsigned char cmd,unsigned char *p,int n)
{
    (void)s;
    unsigned off=cmd==1?0:cmd==0x31?1:cmd==0x11?2:3;
    if(off==3 || n<0 || n>3-(int)off) unsupported();
    for(int i=0;i<n;i++) NOR[NOR_SIZE+off+i]=p[i];
}
static int nor_read(void *s,unsigned addr,void *buf,int n)
{
    (void)s;
    if(n<0 || addr>NOR_SIZE || (unsigned)n>NOR_SIZE-addr) return -22;
    memcpy(buf,(const void *)(NOR+addr),n); return 0;
}
static int nor_write(void *s,unsigned addr,const void *buf,int n)
{
    (void)s; const unsigned char *p=buf;
    if(n<0 || addr>NOR_SIZE || (unsigned)n>NOR_SIZE-addr) return -22;
    for(int i=0;i<n;i++) NOR[addr+i]&=p[i];
    return 0;
}
static int nor_erase(void *s,unsigned addr,int n)
{
    (void)s;
    if(n<0 || (addr&4095) || (n&4095) || addr>NOR_SIZE || (unsigned)n>NOR_SIZE-addr) return -22;
    memset((void *)(NOR+addr),255,n); return 0;
}
static void nor_reset(void *s) { (void)s; }
static void nor_command(void *s,unsigned char cmd)
{
    (void)s;
    /* Synchronous NOR operations cannot be busy when suspend/resume arrives. */
    if(cmd==0x75 || cmd==0x7a || cmd==0x06 || cmd==0x04) return;
    unsupported();
}
/* A synthetic W25Q32-class identity, not a recovered chip identity. */
__attribute__((section(".spinor_api"),used))
const uintptr_t spinor_api[13] = {
 FN(unsupported),FN(unsupported),FN(nor_chipid),FN(nor_status),
 FN(nor_write_status),FN(nor_read),FN(nor_write),FN(nor_erase),FN(nor_reset),
 FN(unsupported),FN(unsupported),FN(nor_command),FN(unsupported)};
__attribute__((section(".brom_api"),used))
const uintptr_t boot_api[] = {
    FN(unsupported),FN(unsupported),FN(unsupported),FN(unsupported),
    FN(unsupported),FN(unsupported),FN(unsupported),FN(spinor_api),
    FN(memset),FN(memcpy),FN(memcmp),FN(unsupported),0,0,0,0,FN(checksum)
};

__attribute__((section(".libc_api"),used))
const uintptr_t libc_api[] = {
    FN(strcpy),FN(strncpy),FN(strchr),FN(strrchr),FN(strlen),FN(strnlen),
    FN(strcmp),FN(strncmp),FN(strtok_r),FN(strcat),FN(strncat),FN(strstr),
    FN(strspn),FN(strcspn),FN(memcmp),FN(memmove),FN(memcpy),FN(memset),
    FN(memchr),FN(compat_cbvprintf),FN(compat_vsnprintf)
};

__attribute__((section(".misc_api"),used))
const uintptr_t misc_api[] = {FN(LZ4_decompress_safe)};

/* Leopard GUI slot 2, called by the stock software drawing fallback.
 * Both pitches are in bytes, and alpha is interleaved after each RGB565 word. */
static void blend_argb8565(uint8_t *dst, const uint8_t *src,
                           uint16_t dst_pitch, uint16_t src_pitch,
                           uint16_t width, uint16_t height)
{
    for (unsigned y = 0; y < height; y++) {
        uint8_t *out = dst + y * (unsigned)dst_pitch;
        const uint8_t *in = src + y * (unsigned)src_pitch;
        for (unsigned x = 0; x < width; x++, in += 3, out += 2) {
            unsigned alpha = in[2];
            if (!alpha) {
                continue;
            }
            unsigned fg = in[0] | (in[1] << 8);
            if (alpha != 255) {
                unsigned bg = out[0] | (out[1] << 8);
                unsigned inverse = 255 - alpha;
                unsigned red = (fg >> 11) * alpha + (bg >> 11) * inverse;
                unsigned green = (fg & 0x07e0) * alpha + (bg & 0x07e0) * inverse;
                unsigned blue = (fg & 0x001f) * alpha + (bg & 0x001f) * inverse;
                fg = ((red >> 8) << 11) | ((green >> 8) & 0x07e0) | (blue >> 8);
            }
            out[0] = fg & 0xff;
            out[1] = fg >> 8;
        }
    }
}

__attribute__((section(".gui_api"),used))
const uintptr_t gui_api[22] = {
    [0 ... 1] = FN(unsupported),
    [2] = FN(blend_argb8565),
    [3 ... 7] = FN(unsupported),
    FN(sw_cos30), FN(sw_sin30), FN(sw_transform_point32), FN(sw_transform_area16),
    [12 ... 21] = FN(unsupported)
};
