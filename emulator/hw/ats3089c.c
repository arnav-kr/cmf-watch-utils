/*
 * Experimental Actions ATS3089C / CMF Watch Pro 2 machine.
 * SPDX-License-Identifier: GPL-2.0-or-later
 * Register semantics derived from the Actions Apache-2.0 Leopard SDK.
 * Unimplemented accesses are explicitly reported; this is not a complete SoC.
 */
#include "qemu/osdep.h"
#include <glib/gstdio.h>
#include <math.h>
#include "qapi/error.h"
#include "hw/boards.h"
#include "hw/arm/armv7m.h"
#include "hw/arm/boot.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-clock.h"
#include "hw/loader.h"
#include "hw/irq.h"
#include "sysemu/sysemu.h"
#include "sysemu/runstate.h"
#include "sysemu/reset.h"
#include "chardev/char-fe.h"
#include "qemu/log.h"
#include "qemu/timer.h"
#include "qemu/guest-random.h"
#include "qemu/error-report.h"
#include "exec/address-spaces.h"
#include "audio/audio.h"
#include "wp2_nand.h"
#include "wp2_bt.h"
#include "wp2_de.h"
#include "wp2_jpeg.h"
#include "wp2_vg_path.h"

/* Stock's GPS init binds UART_1 (0x4003c000, IRQ 23). */
#define GPS_UART_CTL 0x3c000
#define GPS_UART_RX 0x3c004
#define GPS_UART_STATUS 0x3c00c
#define GPS_UART_RX_IRQ (1u << 18)

typedef struct ActsState ActsState;
typedef struct ActsTimer {
    ActsState *soc;
    QEMUTimer *timer;
    qemu_irq irq;
    uint32_t ctl, value, count;
    int64_t started;
} ActsTimer;

struct ActsState {
    MemoryRegion rom, flash, sram, shared, psram, psram_alias, mmio, nor, param;
    MemoryRegion trace_ram, psram_wt_alias, psram_uncached, provisioning, bt_ram;
    MemoryRegion flash_uncached, jpeg_ram, usb_phy;
    const char *nand_export;
    const char *ota_nand_snapshot;
    DeviceState *cpu;
    CharBackend uart[3];
    ActsTimer timers[6];
    uint32_t regs[0x100000 / 4];
    uint8_t seen[0x100000 / 4];
    uint8_t usb_idvbus, usb_dpdm;
    bool exploratory;
    bool charger_present;
    unsigned battery_mv;
    bool stk_enabled;
    bool health_hook;
    uint32_t health_readings;
    int64_t health_next_sample;
    int accel_mg[3];
    qemu_irq gps_irq;
    bool gps_enabled, gps_gga;
    int gps_lat_e6, gps_lon_e6;
    int64_t gps_next_sentence;
    char gps_sentence[96];
    unsigned gps_pos, gps_length, gps_available;
    uint8_t stk_regs[256], stk_reg;
    unsigned stk_task_offset[4];
    int64_t stk_next_sample;
    bool host_rtc;
    bool stock_awake;
    QEMUTimer *unplug_timer;
    bool unplug_after_boot;
    QEMUTimer *tap_timer;
    unsigned tap_x, tap_y, tap_ms, touch_x, touch_y;
    bool auto_touch;
    uint8_t i2c_tx[4][16], i2c_tx_len[4], i2c_rx[4][8];
    unsigned i2c_left[4], i2c_index[4];
    bool touch_down;
    uint8_t touch_reg;
    const char *input_path;
    FILE *input;
    QEMUTimer *input_timer;
    int64_t touch_pressed_at, crown_pressed_at;
    qemu_irq pmu_irq;
    bool crown_down;
    int crown_delta;
    uint8_t knob_regs[256];
    MemoryRegion maps[8];
    bool mapped[8];
    qemu_irq i2c_irq[4];
    Wp2Nand nand;
    Wp2Bt bt;
    qemu_irq bt_irq;
    CharBackend bt_backend;
    bool bt_external;
    QEMUTimer *bt_rx_timer;
    uint8_t bt_rx[4096];
    unsigned bt_rx_len;
    qemu_irq dma_irq[10];
    qemu_irq dac_irq, dac_fifo_irq;
    QEMUTimer *dac_timer;
    unsigned dac_queued, dac_fraction;
    int16_t dac_pcm[1024];
    unsigned dac_pcm_head;
    unsigned dac_host_rate;
    QEMUSoundCard dac_card;
    SWVoiceOut *dac_voice;
    GByteArray *dac_host_pcm;
    qemu_irq lcd_irq;
    qemu_irq vg_irq;
    qemu_irq sdma1_irq;
    qemu_irq de_irq;
    qemu_irq jpeg_irq;
    uint32_t de_palette[3][256];
    unsigned de_palette_index;
    qemu_irq gpio_irq, i2cmt_irq[2];
    QEMUTimer *panel_te_timer;
    QEMUTimer *frame_timer;
    const char *frame_path;
    unsigned panel_inset[466];
    bool panel_on, panel_te_enabled;
    uint8_t panel_brightness, viewer_brightness;
    bool panel_de_active;
    uint32_t vg_state[4096];
    uint8_t panel[466*466*3];
    unsigned panel_x0,panel_x1,panel_y0,panel_y1,panel_frames;
    QEMUTimer *adc_timer;
    qemu_irq adc_irq;
    QEMUTimer *wd_timer;
    qemu_irq wd_irq;
};
static void acts_bt_fault(const char *message)
{
    error_report("WP2 H4 backend: %s", message);
    qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
}

static void acts_bt_rx_drain(void *opaque)
{
    ActsState *s = opaque;
    while (s->bt_rx_len) {
        int length = wp2_h4_size(s->bt_rx, s->bt_rx_len);
        if (length < 0 || length > sizeof(s->bt_rx) || s->bt_rx[0] == 1) {
            acts_bt_fault("unsupported or oversized incoming packet");
            return;
        }
        if (!length || length > s->bt_rx_len) break;
        if (!wp2_bt_receive_h4(&s->bt, memory_region_get_ram_ptr(&s->shared),
                               s->bt_rx, length)) {
            timer_mod(s->bt_rx_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 1000000);
            return;
        }
        s->bt_rx_len -= length;
        memmove(s->bt_rx, s->bt_rx + length, s->bt_rx_len);
        s->regs[0x301c / 4] = 1;
        qemu_set_irq(s->bt_irq, 1);
    }
    qemu_chr_fe_accept_input(&s->bt_backend);
}

static int acts_bt_can_read(void *opaque)
{
    ActsState *s = opaque;
    return sizeof(s->bt_rx) - s->bt_rx_len;
}

static void acts_bt_read(void *opaque, const uint8_t *data, int count)
{
    ActsState *s = opaque;
    if (count > acts_bt_can_read(s)) {
        acts_bt_fault("receive buffer overflow");
        return;
    }
    memcpy(s->bt_rx + s->bt_rx_len, data, count);
    s->bt_rx_len += count;
    acts_bt_rx_drain(s);
}
static void acts_unplug(void *opaque)
{
    ActsState *s = opaque;
    s->charger_present = false;
}

static void acts_gpio_falling(ActsState *s, unsigned pin)
{
    uint32_t ctl = s->regs[(0x68000 + pin * 4) / 4];
    if ((ctl & 0x02100000) == 0x02100000) {
        s->regs[0x68240 / 4 + pin / 32] |= 1u << (pin % 32);
        qemu_set_irq(s->gpio_irq, 1);
    }
}

static void acts_touch_report(ActsState *s, bool down, unsigned x, unsigned y)
{
    s->touch_down = down;
    s->touch_x = x;
    s->touch_y = y;
    acts_gpio_falling(s, 27);
}

static void acts_touch_tick(void *opaque)
{
    ActsState *s = opaque;
    int64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    s->auto_touch = !s->auto_touch;
    acts_touch_report(s, s->auto_touch, s->tap_x, s->tap_y);
    if (s->auto_touch) {
        timer_mod(s->tap_timer, now + 150000000);
    }
}

/* CST820 supports both a repeated start and a stop between register select
 * and read. Stock reads from 0x00, while the custom driver starts at 0x01. */
static bool acts_cst820_transfer(ActsState *s, const uint8_t *tx, unsigned count,
                                  unsigned len, uint8_t *rx)
{
    if (tx[0] == 0x2a && count == 2 && len == 1) {
        s->touch_reg = tx[1];
        return true;
    }
    if (tx[0] == 0x2a && count == 3 && len == 2 &&
        (tx[1] == 0xfa || tx[1] == 0xfe)) {
        return true; /* report mode / sleep */
    }
    if (tx[0] == 0x2a && count == 3 && tx[2] == 0x2b) {
        s->touch_reg = tx[1];
    } else if (tx[0] != 0x2b || count != 1) {
        return false;
    }

    uint8_t report[] = {
        0, 0, s->touch_down,
        (s->touch_x >> 8) | (s->touch_down ? 0 : 0x40), s->touch_x,
        s->touch_y >> 8, s->touch_y,
    };
    if (s->touch_reg == 0xa7 && len == 1) {
        rx[0] = 0xb7; /* CST820 chip ID read on the physical watch */
    } else if (len && s->touch_reg + len <= sizeof(report)) {
        memcpy(rx, report + s->touch_reg, len);
    } else {
        return false;
    }
    s->touch_reg += len;
    s->i2c_index[0] = 0;
    s->i2c_left[0] = len;
    return true;
}

static void acts_pmu_irq(ActsState *s)
{
    qemu_set_irq(s->pmu_irq, !!(s->regs[0x4114 / 4] & s->regs[0x4040 / 4] & 15));
}

static void acts_stk_sample(ActsState *s, uint8_t sample[6])
{
    unsigned range = s->stk_regs[0x0f] & 15;
    int scale = range == 5 ? 512 : range == 8 ? 256 : 1024;
    for (unsigned axis = 0; axis < 3; axis++) {
        int counts = CLAMP(s->accel_mg[axis] * scale / 1000, -2048, 2047);
        stw_le_p(sample + axis * 2, (uint16_t)(counts * 16));
    }
}

static void acts_i2cmt_irq(ActsState *s, unsigned bus)
{
    uint32_t *r = &s->regs[(bus ? 0x8c000 : 0x88000) / 4];
    qemu_set_irq(s->i2cmt_irq[bus], !!((r[4] & r[5]) ||
                ((r[0] & 0x10) && (r[1] & 8))));
}

/* STK8321 on stock I2CMT1. Handle both polled DMA transfers and the
 * repeating XYZ task configured by the stock sensor descriptor. */
static bool acts_stk_transfer(ActsState *s, uint32_t *task)
{
    unsigned len = task[3];
    uint8_t data[32], sample[6];
    if (!s->stk_enabled || ((task[5] >> 1) & 127) != 0x1f || !len ||
        len > sizeof(data) || task[2] < 0x18000000 ||
        (uint64_t)task[2] + len > 0x40000000) {
        return false;
    }
    if (!(task[5] & 1)) {
        if (address_space_read(&address_space_memory, task[2], MEMTXATTRS_UNSPECIFIED,
                               data, len) != MEMTX_OK) {
            return false;
        }
        s->stk_reg = data[0];
        for (unsigned i = 1; i < len; i++) {
            uint8_t reg = s->stk_reg++;
            if (reg == 0x14 && data[i] == 0xb6) {
                memset(s->stk_regs, 0, sizeof(s->stk_regs));
                s->stk_regs[0x0f] = 3;
            } else {
                s->stk_regs[reg] = data[i];
            }
        }
        return true;
    }
    uint8_t reg = (task[0] & (1u << 28)) ? s->stk_reg : task[5] >> 8;
    acts_stk_sample(s, sample);
    for (unsigned i = 0; i < len; i++, reg++) {
        data[i] = reg == 0 ? 0x23 :
                  reg >= 2 && reg <= 7 ? sample[reg - 2] : s->stk_regs[reg];
    }
    s->stk_reg = reg;
    return address_space_write(&address_space_memory, task[2], MEMTXATTRS_UNSPECIFIED,
                               data, len) == MEMTX_OK;
}

static void acts_stk_tick(ActsState *s, int64_t now)
{
    if (!s->stk_enabled || now < s->stk_next_sample) {
        return;
    }
    s->stk_next_sample = now + 40000000;
    uint32_t *r = &s->regs[0x8c000 / 4];
    for (unsigned i = 0; i < 4; i++) {
        uint32_t *task = r + 0x100 / 4 + 16 * i;
        unsigned len = task[3], offset = s->stk_task_offset[i];
        if ((task[1] & 3) != 3 || (task[5] & 0xffff) != 0x023f ||
            ((task[0] >> 16) & 0xfff) != 6 || !len || len % 12 || offset >= len ||
            task[2] < 0x18000000 || (uint64_t)task[2] + len > 0x40000000) {
            continue;
        }
        uint8_t sample[6];
        acts_stk_sample(s, sample);
        if (address_space_write(&address_space_memory, task[2] + offset,
                                MEMTXATTRS_UNSPECIFIED, sample, sizeof(sample)) != MEMTX_OK) {
            continue;
        }
        offset += sizeof(sample);
        task[4] = len - offset;
        if (offset == len / 2 || offset == len) {
            r[5] |= 1u << (i + (offset == len / 2 ? 4 : 0));
            acts_i2cmt_irq(s, 1);
        }
        s->stk_task_offset[i] = offset % len;
    }
}

static uint8_t acts_knob_read(ActsState *s, uint8_t reg)
{
    unsigned motion = (unsigned)s->crown_delta & 0xfff;
    switch (reg) {
    case 2:
        return motion ? 0x80 : 0;
    case 3:
        return motion;
    case 4:
        return 0;
    case 0x12:
        s->crown_delta = 0;
        return (motion >> 8) << 4;
    default:
        return s->knob_regs[reg];
    }
}

static void acts_gps_irq(ActsState *s)
{
    qemu_set_irq(s->gps_irq, s->gps_available &&
                 (s->regs[GPS_UART_CTL / 4] & GPS_UART_RX_IRQ));
}

static void acts_gps_coordinate(char *out, size_t size, int coordinate, int digits)
{
    unsigned absolute = abs(coordinate);
    unsigned minutes = ((absolute % 1000000) * 60 + 50) / 100;
    snprintf(out, size, "%0*u%02u.%04u", digits, absolute / 1000000,
             minutes / 10000, minutes % 10000);
}

static void acts_gps_tick(ActsState *s, int64_t now)
{
    if (!s->gps_enabled || !(s->regs[GPS_UART_CTL / 4] & GPS_UART_RX_IRQ) ||
        s->gps_available) {
        return;
    }

    if (s->gps_pos == s->gps_length) {
        if (now < s->gps_next_sentence) {
            return;
        }
        g_autoptr(GDateTime) utc = g_date_time_new_now_utc();
        g_autofree char *time = g_date_time_format(utc, "%H%M%S");
        g_autofree char *date = g_date_time_format(utc, "%d%m%y");
        char latitude[12], longitude[12], body[86];
        acts_gps_coordinate(latitude, sizeof(latitude), s->gps_lat_e6, 2);
        acts_gps_coordinate(longitude, sizeof(longitude), s->gps_lon_e6, 3);
        char ns = s->gps_lat_e6 < 0 ? 'S' : 'N';
        char ew = s->gps_lon_e6 < 0 ? 'W' : 'E';
        if (s->gps_gga) {
            snprintf(body, sizeof(body), "GPGGA,%s.00,%s,%c,%s,%c,1,08,0.9,0.0,M,0.0,M,,",
                     time, latitude, ns, longitude, ew);
        } else {
            snprintf(body, sizeof(body), "GPRMC,%s.00,A,%s,%c,%s,%c,0.0,0.0,%s,,,A",
                     time, latitude, ns, longitude, ew, date);
        }
        unsigned checksum = 0;
        for (const char *byte = body; *byte; byte++) {
            checksum ^= (unsigned char)*byte;
        }
        s->gps_length = snprintf(s->gps_sentence, sizeof(s->gps_sentence),
                                 "$%s*%02X\r\n", body, checksum);
        s->gps_pos = 0;
        s->gps_next_sentence = now + (s->gps_gga ? 850000000 : 150000000);
        s->gps_gga = !s->gps_gga;
    }
    s->gps_available = MIN(12, s->gps_length - s->gps_pos);
    acts_gps_irq(s);
}

static void acts_panel_refresh(ActsState *s);

static void acts_input_tick(void *opaque)
{
    ActsState *s = opaque;
    int64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    char line[80];
    unsigned down, x, y;
    int delta, ax, ay, az;

    timer_mod(s->input_timer, now + 6000000);
    acts_stk_tick(s, now);
    acts_gps_tick(s, now);
    if (s->health_readings && now >= s->health_next_sample) {
        s->health_next_sample = now + 400000000;
        unsigned mode = ldub_phys(&address_space_memory, 0x2ffb1301);
        if (mode == 1 || mode == 2) {
            acts_gpio_falling(s, 22); /* VC9202 data ready */
        }
    }
    if (s->stock_awake) {
        /* Stock 1.0.0.73's system_set_auto_sleep_time loads this context.
         * The SDK uses UINT32_MAX in its first field to disable auto standby. */
        uint32_t context = ldl_le_phys(&address_space_memory, 0x38037d14);
        if (context >= 0x38000000 && context <= 0x387ffffc && !(context & 3)) {
            uint32_t timeout = ldl_le_phys(&address_space_memory, context);
            if (timeout && timeout <= 60000) {
                stl_le_phys(&address_space_memory, context, UINT32_MAX);
            }
        }
    }
    if (!s->input) {
        s->input = g_fopen(s->input_path, "rb");
    }
    if (!s->input) {
        return;
    }
    clearerr(s->input);
    long start = ftell(s->input);
    if (!fgets(line, sizeof(line), s->input)) {
        return;
    }
    if (!strchr(line, '\n')) {
        fseek(s->input, start, SEEK_SET);
        return;
    }
    if (sscanf(line, "t %u %u %u", &down, &x, &y) == 3 &&
        down <= 1 && x < 466 && y < 466) {
        if (down && (!s->touch_down || s->auto_touch)) {
            s->touch_pressed_at = now;
        } else if (!down && s->touch_down && !s->auto_touch &&
                   now - s->touch_pressed_at < 48000000) {
            fseek(s->input, start, SEEK_SET);
            return;
        }
        s->auto_touch = false;
        acts_touch_report(s, down, x, y);
    } else if (sscanf(line, "c %u", &down) == 1 && down <= 1) {
        if (!down && s->crown_down && now - s->crown_pressed_at < 100000000) {
            fseek(s->input, start, SEEK_SET);
            return;
        }
        if (down && !s->crown_down) {
            s->crown_pressed_at = now;
            s->regs[0x4114 / 4] |= 2; /* ONOFF short-press pending */
        }
        s->crown_down = down;
        acts_pmu_irq(s);
    } else if (sscanf(line, "r %d", &delta) == 1 && delta >= -320 && delta <= 320) {
        s->crown_delta = CLAMP(s->crown_delta + delta, -2048, 2047);
        if (s->crown_delta) {
            acts_gpio_falling(s, 58); /* MT3511 motion, GPIOB.26 */
        }
    } else if (sscanf(line, "a %d %d %d", &ax, &ay, &az) == 3 &&
               ax >= -2000 && ax <= 2000 && ay >= -2000 && ay <= 2000 &&
               az >= -2000 && az <= 2000) {
        s->accel_mg[0] = ax;
        s->accel_mg[1] = ay;
        s->accel_mg[2] = az;
    } else if (sscanf(line, "p %u %u", &x, &down) == 2 &&
               x >= 3300 && x <= 4400 && down <= 1) {
        timer_del(s->unplug_timer);
        s->battery_mv = x;
        if (s->charger_present != down) {
            s->charger_present = down;
            s->regs[0x4114 / 4] |= down ? 4 : 8;
            acts_pmu_irq(s);
        }
    } else if (sscanf(line, "b %u", &x) == 1 && x <= 100) {
        if (s->viewer_brightness != x) {
            s->viewer_brightness = x;
            if (s->panel_frames) {
                acts_panel_refresh(s);
            }
        }
    } else if (sscanf(line, "g %u %d %d", &down, &ax, &ay) == 3 &&
               down <= 1 && ax >= -90000000 && ax <= 90000000 &&
               ay >= -180000000 && ay <= 180000000) {
        s->gps_enabled = down;
        s->gps_lat_e6 = ax;
        s->gps_lon_e6 = ay;
        s->gps_pos = s->gps_length = s->gps_available = 0;
        s->gps_gga = false;
        s->gps_next_sentence = now;
        acts_gps_irq(s);
    } else if (s->health_hook && sscanf(line, "h %u %u %u", &down, &x, &y) == 3 &&
               down <= 1 && x >= 30 && x <= 220 && y >= 70 && y <= 100) {
        s->health_readings = down ? x | (y << 8) : 0;
        s->health_next_sample = now;
    }
}
static void acts_wd_expire(void *opaque)
{
    ActsState *s = opaque;
    if (s->regs[0xc020 / 4] & 32) {
        s->regs[0xc020 / 4] |= 64;
        qemu_set_irq(s->wd_irq, 1);
        return;
    }
    if (s->ota_nand_snapshot && s->regs[0xc03c / 4] == 0x42520002) {
        g_autoptr(GError) error = NULL;
        if (!g_file_set_contents(s->ota_nand_snapshot, (const char *)s->nand.data,
                                 (size_t)NAND_PAGE * NAND_PAGES, &error)) {
            error_report("OTA NAND snapshot failed: %s", error->message);
        }
        s->ota_nand_snapshot = NULL;
    }
    error_report("Virtual watch watchdog expired");
    qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
}
static void acts_adc_irq(ActsState *s)
{
    qemu_set_irq(s->adc_irq,!!(s->regs[0x4208/4]&s->regs[0x4204/4]));
}
static void acts_adc_tick(void *opaque)
{
    ActsState *s=opaque;
    if(s->regs[0x4200/4]&(1u<<23)) {
        s->regs[0x4208/4]|=s->regs[0x4200/4]&0xfff;
        acts_adc_irq(s);
        timer_mod(s->adc_timer,qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL)+1000000);
    }
}
static void acts_panel_publish(void *opaque)
{
    ActsState *s = opaque;
    GString *ppm=g_string_new("P6\n466 466\n255\n");
    unsigned brightness = s->panel_brightness * s->viewer_brightness;
    size_t header = ppm->len;
    if (brightness == 25500 && !s->frame_timer) {
        g_string_append_len(ppm, (char *)s->panel, sizeof(s->panel));
    } else {
        uint8_t levels[256];
        for (unsigned i = 0; i < ARRAY_SIZE(levels); i++) {
            levels[i] = (i * brightness + 12750) / 25500;
        }
        g_string_set_size(ppm, header + sizeof(s->panel));
        for (unsigned y = 0; y < 466; y++) {
            unsigned pad = s->frame_timer ? s->panel_inset[y] * 3 : 0;
            char *row = ppm->str + header + y * 466 * 3;
            const uint8_t *source = s->panel + y * 466 * 3;
            unsigned end = 466 * 3 - pad;
            memset(row, 24, pad);
            if (brightness == 25500) {
                memcpy(row + pad, source + pad, end - pad);
            } else {
                for (unsigned x = pad; x < end; x++) {
                    row[x] = levels[source[x]];
                }
            }
            memset(row + end, 24, pad);
        }
    }
    g_file_set_contents(s->frame_path,ppm->str,ppm->len,NULL);
    g_string_free(ppm,true);
}

static void acts_panel_refresh(ActsState *s)
{
    if (!s->frame_path) {
        return;
    }
    if (!s->frame_timer) {
        acts_panel_publish(s);
    } else if (!timer_pending(s->frame_timer)) {
        /* Stock transfers multiple rectangular strips for one LCD frame. */
        timer_mod(s->frame_timer, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + 16000000);
    }
}

static void acts_panel_save(ActsState *s)
{
    s->panel_frames++;
    acts_panel_refresh(s);
}
static void acts_panel_te_tick(void *opaque)
{
    ActsState *s = opaque;
    const uint32_t irq_enable = (1u << 20) | (1u << 25);
    /* GPIOB.3 (GPIO35), the board's active-high panel TE input. */
    if (s->panel_on && s->panel_te_enabled &&
        (s->regs[0x6808c / 4] & irq_enable) == irq_enable) {
        s->regs[0x68244 / 4] |= 8;
        qemu_set_irq(s->gpio_irq, 1);
    }
    if (s->panel_on && s->panel_te_enabled) {
        timer_mod(s->panel_te_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 18181818);
    }
}
static void acts_lcd_irq(ActsState *s)
{
    qemu_set_irq(s->lcd_irq,!!((s->regs[0x64048/4]&8) && (s->regs[0x6402c/4]&(1u<<30))));
}

/* GC265 subset observed in the stock boot stream. The queue is synchronous,
 * so the pixel-engine semaphore can be satisfied as soon as it is issued. */
static bool acts_vg_clear(ActsState *s, uint32_t origin, uint32_t size)
{
    const uint32_t *state = s->vg_state;
    unsigned x = origin & 0xffff, y = origin >> 16;
    unsigned width = size & 0xffff, height = size >> 16;
    unsigned pitch = state[0xa12];
    uint32_t base = state[0xa11], color = state[0xa02];
    uint16_t pixel = ((color & 0xf8) << 8) |
                     ((color >> 5) & 0x7e0) | ((color >> 19) & 31);
    uint8_t row[466 * 2];
    if (state[0xa10] != 0x101 || state[0xa00] != 0x10000001 ||
        !width || !height || x + width > (state[0xa13] & 0xffff) ||
        y + height > (state[0xa13] >> 16) || width > 466 || height > 466 ||
        pitch < (x + width) * 2 || pitch > 16384 ||
        base < 0x18000000 || (uint64_t)base + (y + height) * pitch > 0x40000000) {
        return false;
    }
    for (unsigned i = 0; i < width; i++) {
        stw_le_p(row + i * 2, pixel);
    }
    for (unsigned i = 0; i < height; i++) {
        if (address_space_write(&address_space_memory, base + (y + i) * pitch + x * 2,
                                MEMTXATTRS_UNSPECIFIED, row, width * 2) != MEMTX_OK) {
            return false;
        }
    }
    return true;
}

static float acts_vg_float(uint32_t value)
{
    float result;
    memcpy(&result, &value, sizeof(result));
    return result;
}

/* Stock resource-cache headers identify packed RGB565 + A8 bitmaps even
 * when the stock VG descriptor advertises BGRA8888. */
static bool acts_vg_resource_packed(uint32_t src, unsigned w, unsigned h)
{
    uint8_t hdr[36];
    if (src < 0x3c000040 || src >= 0x3c800000 || (src & 63) ||
        address_space_read(&address_space_memory, src - 64,
                           MEMTXATTRS_UNSPECIFIED, hdr, sizeof(hdr)) != MEMTX_OK) {
        return false;
    }
    return ldl_le_p(hdr + 32) == src - 0x04000000 &&
           ldl_le_p(hdr + 16) == w && ldl_le_p(hdr + 20) == h &&
           ldl_le_p(hdr + 24) == 3 &&
           ldl_le_p(hdr + 8) == QEMU_ALIGN_UP(64 + w * h * 3, 64);
}

/* WFC textures have no resource-cache header. */
static bool acts_vg_wfc_packed(const uint8_t *data, unsigned w, unsigned h)
{
    static GHashTable *local_textures;
    if (!local_textures) {
        local_textures = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
        const char *file = g_getenv("QEMU_WP2_WFC_INDEX");
        if (file) {
            g_autofree char *contents = NULL;
            if (!g_file_get_contents(file, &contents, NULL, NULL)) {
                error_report("Cannot read WFC texture index %s", file);
                exit(1);
            }
            g_auto(GStrv) lines = g_strsplit(contents, "\n", -1);
            for (unsigned i = 0; lines[i]; i++) {
                unsigned tw, th;
                char hash[65], extra;
                g_strchomp(lines[i]);
                if (!*lines[i]) continue;
                if (sscanf(lines[i], "%u:%u:%64[0123456789abcdef]%c", &tw, &th, hash, &extra) != 3 ||
                    !tw || !th || tw > 466 || th > 466 || strlen(hash) != 64) {
                    error_report("Invalid WFC texture index line %u", i + 1);
                    exit(1);
                }
                g_hash_table_add(local_textures, g_strdup(lines[i]));
            }
            error_report("WFC index: %u textures loaded", g_hash_table_size(local_textures));
        }
    }
    static const struct {
        unsigned w, h;
        const char *sha256;
    } textures[] = {
#include "wp2_wfc_index.h"
    };
    g_autofree char *digest = NULL;
    if (g_hash_table_size(local_textures)) {
        digest = g_compute_checksum_for_data(G_CHECKSUM_SHA256, data, w * h * 3);
        g_autofree char *key = g_strdup_printf("%u:%u:%s", w, h, digest);
        if (g_hash_table_contains(local_textures, key)) return true;
    }
    for (unsigned i = 0; i < ARRAY_SIZE(textures); i++) {
        if (w == textures[i].w && h == textures[i].h) {
            if (!digest) {
                digest = g_compute_checksum_for_data(G_CHECKSUM_SHA256,
                                                     data, w * h * 3);
            }
            if (g_str_equal(digest, textures[i].sha256)) {
                return true;
            }
        }
    }
    return false;
}

static uint16_t acts_vg_color(const uint8_t *pixel, unsigned bytes)
{
    if (bytes == 4) {
        return ((pixel[2] & 0xf8) << 8) | ((pixel[1] & 0xfc) << 3) |
               (pixel[0] >> 3);
    }
    return lduw_le_p(pixel);
}

static uint16_t acts_vg_linear(const uint8_t *image, unsigned pitch,
                               unsigned w, unsigned h, unsigned bytes,
                               float u, float v, unsigned *opacity)
{
    float x = MAX(0.0f, u * w - 0.5f);
    float y = MAX(0.0f, v * h - 0.5f);
    unsigned x0 = (unsigned)x, y0 = (unsigned)y;
    unsigned x1 = MIN(x0 + 1, w - 1), y1 = MIN(y0 + 1, h - 1);
    float fx = x - x0, fy = y - y0;
    const uint8_t *samples[] = {
        image + y0 * pitch + x0 * bytes, image + y0 * pitch + x1 * bytes,
        image + y1 * pitch + x0 * bytes, image + y1 * pitch + x1 * bytes,
    };
    float weights[] = {(1 - fx) * (1 - fy), fx * (1 - fy),
                       (1 - fx) * fy, fx * fy};
    uint16_t pixels[4];
    for (unsigned i = 0; i < ARRAY_SIZE(pixels); i++) {
        pixels[i] = acts_vg_color(samples[i], bytes);
    }
    if (bytes > 2) {
        /* Transparent texels contribute coverage, without leaking their RGB. */
        float coverage = 0;
        for (unsigned i = 0; i < ARRAY_SIZE(samples); i++) {
            weights[i] *= samples[i][bytes - 1];
            coverage += weights[i];
        }
        *opacity = (unsigned)(coverage + 0.5f);
        if (!*opacity) {
            return 0;
        }
        for (unsigned i = 0; i < ARRAY_SIZE(weights); i++) {
            weights[i] /= coverage;
        }
    }
    static const unsigned shifts[] = {0, 5, 11};
    static const unsigned masks[] = {31, 63, 31};
    uint16_t result = 0;
    for (unsigned i = 0; i < ARRAY_SIZE(shifts); i++) {
        unsigned shift = shifts[i], mask = masks[i];
        float channel = 0;
        for (unsigned j = 0; j < ARRAY_SIZE(pixels); j++) {
            channel += ((pixels[j] >> shift) & mask) * weights[j];
        }
        result |= (unsigned)(channel + 0.5f) << shift;
    }
    return result;
}

/* Source coordinates in the command stream are normalized to image size. */
static bool acts_vg_blit(ActsState *s, uint32_t origin, uint32_t size,
                         const Wp2Path *path)
{
    const uint32_t *state = s->vg_state;
    unsigned x = origin & 0xffff, y = origin >> 16;
    unsigned width = size & 0xffff, height = size >> 16;
    unsigned sw = state[0xa2f] & 0xffff, sh = state[0xa2f] >> 16;
    unsigned pitch = state[0xa12], spitch = state[0xa2b];
    uint32_t src = state[0xa29], dst = state[0xa11];
    bool fade = state[0xa00] == 0xa101;
    bool pattern = path && state[0xa00] == 0x10001002 &&
                   state[0xa27] == 0xff000000;
    bool linear_rgb565 = state[0xa25] == 0x01020105;
    bool argb = state[0xa00] == 0x9101 &&
                (state[0xa25] == 0x41020100 || state[0xa25] == 0x41000100);
    bool linear = linear_rgb565 || (argb && state[0xa25] == 0x41020100);
    unsigned bytes = argb ? 4 : 2;
    unsigned alpha = state[0xa02] >> 24;
    qemu_log_mask(LOG_UNIMP,
                  "VG blit cfg=%08x fmt=%08x src=%08x %ux%u/%u dst=%08x (%u,%u %ux%u)\n",
                  state[0xa00], state[0xa25], src, sw, sh, spitch, dst, x, y, width, height);
    if ((!argb && ((!fade && !pattern && state[0xa00] != 0x10001001) ||
                   (state[0xa25] != 0x01000105 && !linear_rgb565))) ||
        state[0xa10] != 0x101 ||
        (state[0xa27] && !pattern) || state[0xa2d] ||
        state[0xa1a] != 0x3f800000 || state[0xa1e] || state[0xa22] ||
        !width || !height || !sw || !sh || sw > 466 || sh > 466 ||
        x + width > 466 || y + height > 466 || pitch < (x + width) * 2 ||
        spitch < sw * bytes || spitch > 8192 || pitch > 8192 ||
        src < 0x18000000 || dst < 0x18000000 ||
        (uint64_t)src + spitch * sh > 0x40000000 ||
        (uint64_t)dst + pitch * (y + height) > 0x40000000) {
        return false;
    }
    bool packed = argb && acts_vg_resource_packed(src, sw, sh);
    if (packed) {
        spitch = sw * 3;
        bytes = 3;
    }
    g_autofree uint8_t *image = g_malloc(spitch * sh);
    if (address_space_read(&address_space_memory, src, MEMTXATTRS_UNSPECIFIED,
                           image, spitch * sh) != MEMTX_OK) {
        return false;
    }
    if (argb && !packed && acts_vg_wfc_packed(image, sw, sh)) {
        packed = true;
        spitch = sw * 3;
        bytes = 3;
    }
    float u0 = acts_vg_float(state[0xa18]), v0 = acts_vg_float(state[0xa19]);
    float ux = acts_vg_float(state[0xa1c]), vx = acts_vg_float(state[0xa1d]);
    float uy = acts_vg_float(state[0xa20]), vy = acts_vg_float(state[0xa21]);
    uint8_t row[466 * 2];
    for (unsigned j = 0; j < height; j++) {
        int winding[467];
        int inside = 0;
        if (path) {
            memset(winding, 0, (width + 1) * sizeof(*winding));
            wp2_vg_path_row(path, y + j, x, x + width, winding);
        }
        uint32_t address = dst + (y + j) * pitch + x * 2;
        if (address_space_read(&address_space_memory, address, MEMTXATTRS_UNSPECIFIED,
                               row, width * 2) != MEMTX_OK) {
            return false;
        }
        for (unsigned i = 0; i < width; i++) {
            if (path) {
                inside += winding[i];
                if (path->even_odd ? inside % 2 == 0 : inside == 0) {
                    continue;
                }
            }
            float u = u0 + ux * (x + i) + uy * (y + j);
            float v = v0 + vx * (x + i) + vy * (y + j);
            if (u >= 0 && u < 1 && v >= 0 && v < 1) {
                unsigned ix = MIN((unsigned)(u * sw), sw - 1);
                unsigned iy = MIN((unsigned)(v * sh), sh - 1);
                const uint8_t *p = image + iy * spitch + ix * bytes;
                unsigned opacity = packed ? p[2] : argb ? p[3] : alpha;
                uint16_t source = linear
                    ? acts_vg_linear(image, spitch, sw, sh, bytes, u, v, &opacity)
                    : acts_vg_color(p, bytes);
                if (fade || argb) {
                    uint16_t dest = lduw_le_p(row + i * 2);
                    unsigned red = ((source >> 11) * opacity + (dest >> 11) * (255 - opacity) + 127) / 255;
                    unsigned green = (((source >> 5) & 63) * opacity + ((dest >> 5) & 63) * (255 - opacity) + 127) / 255;
                    unsigned blue = ((source & 31) * opacity + (dest & 31) * (255 - opacity) + 127) / 255;
                    source = (red << 11) | (green << 5) | blue;
                }
                stw_le_p(row + i * 2, source);
            } else if (pattern) {
                stw_le_p(row + i * 2, 0);
            }
        }
        if (address_space_write(&address_space_memory, address, MEMTXATTRS_UNSPECIFIED,
                                row, width * 2) != MEMTX_OK) {
            return false;
        }
    }
    return true;
}

static bool acts_vg_submit(ActsState *s, uint32_t units)
{
    uint32_t base = s->regs[0x70500 / 4], semaphore = 0;
    if (!units || units > 0x4000 || (base & 7) || base >= 0x40000000 - units * 8) {
        return false;
    }
    g_autofree uint8_t *stream = g_malloc(units * 8);
    if (address_space_read(&address_space_memory, base, MEMTXATTRS_UNSPECIFIED,
                           stream, units * 8) != MEMTX_OK) {
        return false;
    }
    for (unsigned i = 0; i < units; i++) {
        uint32_t op = ldl_le_p(stream + i * 8);
        uint32_t arg = ldl_le_p(stream + i * 8 + 4);
        if (op == 0) {
            s->regs[0x70010 / 4] |= 1; /* command stream end */
            qemu_set_irq(s->vg_irq, !!(s->regs[0x70014 / 4] & 1));
            return true;
        }
        if ((op & 0xfffff000) == 0x30010000) {
            s->vg_state[op & 0xfff] = arg;
        } else if (op == 0x10000007) {
            semaphore = 7;
        } else if (op == 0x20000007 && semaphore == 7) {
            semaphore = 0;
        } else if ((op & 0xffff0000) == 0x40000000 &&
                   (op & 0xffff) && (op & 0xffff) < units - i &&
                   (s->vg_state[0xa00] == 0x00000002 ||
                    s->vg_state[0xa00] == 0x00000102 ||
                    s->vg_state[0xa00] == 0x10000102 ||
                    s->vg_state[0xa00] == 0x10000002 ||
                    s->vg_state[0xa00] == 0x10001002)) {
            unsigned length = op & 0xffff;
            const uint32_t *state = s->vg_state;
            Wp2Path path;
            if (!wp2_vg_decode_path(state, stream + (i + 1) * 8, length * 2, &path) ||
                !(state[0xa00] == 0x10001002
                  ? acts_vg_blit(s, state[0xa39], state[0xa3a], &path)
                  : wp2_vg_solid_path(state, &path))) {
                qemu_log_mask(LOG_UNIMP, "Unsupported VG Lite path at packet %u\n", i);
                return false;
            }
            i += length;
        } else if (op == 0x40000001 && i + 1 < units) {
            uint32_t origin = ldl_le_p(stream + (i + 1) * 8);
            uint32_t size = ldl_le_p(stream + (i + 1) * 8 + 4);
            if (!acts_vg_clear(s, origin, size) && !acts_vg_blit(s, origin, size, NULL)) {
                qemu_log_mask(LOG_UNIMP, "Unsupported VG Lite draw packet %u\n", i);
                return false;
            }
            ++i;
        } else {
            qemu_log_mask(LOG_UNIMP, "Unsupported VG Lite packet %u: %08x %08x\n",
                          i, op, arg);
            return false;
        }
    }
    return false;
}
static void acts_lcd_write(ActsState *s,unsigned addr,uint32_t value)
{
    if(addr==0x64048) {s->regs[addr/4]&=~value;acts_lcd_irq(s);return;}
    s->regs[addr/4]=value;
    if(addr==0x64020 && (s->regs[0x6402c/4]&(1u<<28))) {
        unsigned cmd=(s->regs[0x64034/4]>>8)&255;
        qemu_log_mask(LOG_UNIMP,"PANEL command %02x data=%08x len=%u\n",cmd,value,(s->regs[0x6402c/4]>>18)&63);
        if(cmd==0x2a) {s->panel_x0=value>>16;s->panel_x1=value&65535;}
        if(cmd==0x2b) {s->panel_y0=value>>16;s->panel_y1=value&65535;}
        if (cmd == 0x51 && (s->regs[0x64034 / 4] >> 24) == 0x02 &&
            s->panel_brightness != (uint8_t)value) {
            s->panel_brightness = value;
            if (s->panel_frames) {
                acts_panel_refresh(s);
            }
        }
        if (cmd == 0x29 || cmd == 0x28 || cmd == 0x10 ||
            cmd == 0x35 || cmd == 0x34) {
            if (cmd == 0x29) {
                s->panel_on = true;
            } else if (cmd == 0x28 || cmd == 0x10) {
                s->panel_on = false;
            } else {
                s->panel_te_enabled = cmd == 0x35;
            }
            timer_del(s->panel_te_timer);
            if (s->panel_on && s->panel_te_enabled) {
                timer_mod(s->panel_te_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 18181818);
            }
        }
    }
    if(addr!=0x6402c || !(value&(1u<<31))) return;
    unsigned source=(value>>10)&3;
    if (source == 1) {
        return; /* DE-triggered transfer completes when the overlay renders. */
    }
    if (source == 0 && s->panel_de_active) {
        /* Stock switches the SPI mux back to AHB between DE transfers. */
        s->regs[addr / 4] &= ~(1u << 31);
        return;
    }
    if(source!=2) {
        error_report("Unsupported LCD pixel source %u (SPI_CTL=%08x)", source, value);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);return;
    }
    s->panel_de_active = false;
    uint32_t *dma=&s->regs[0x78100/4], *line=&s->regs[0x78c00/4];
    unsigned w=(s->regs[0x64008/4]&65535)+1,h=(s->regs[0x64008/4]>>16)+1;
    unsigned format=(value>>24)&7,bpp=format==0?2:format<3?3:4;
    qemu_log_mask(LOG_UNIMP, "LCD pixels %u,%u %ux%u ctl=%08x dma=%08x src=%08x stride=%u\n",
                  s->panel_x0, s->panel_y0, w, h, value, dma[0], dma[2], line[2]);
    if(w>466 || h>466 || format>4) {
        error_report("Unsupported LCD size/format %ux%u fmt=%u",w,h,format);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);return;
    }
    uint8_t row[466*4];
    for(unsigned y=0;y<h;y++) {
        uint32_t src=dma[2]+(int32_t)(y*(line[2]?line[2]:w*bpp))*((dma[0]&(1u<<6))?-1:1);
        cpu_physical_memory_read(src,row,w*bpp);
        for(unsigned x=0;x<w;x++) {
            int px=(int)s->panel_x0-8+x,py=s->panel_y0+y;
            if(px<0||px>=466||py<0||py>=466) continue;
            uint8_t *d=&s->panel[(py*466+px)*3],*p=&row[x*bpp];
            if(format==0) {unsigned c=p[0]|(p[1]<<8);d[0]=((c>>11)&31)*255/31;d[1]=((c>>5)&63)*255/63;d[2]=(c&31)*255/31;}
            else {d[0]=p[2];d[1]=p[1];d[2]=p[0];}
            if(value&16) {uint8_t t=d[0];d[0]=d[2];d[2]=t;}
        }
    }
    dma[1]=0;dma[7]=0;line[4]=0;
    s->regs[addr/4]&=~(1u<<31);
    s->regs[0x64048/4]|=8;
    acts_panel_save(s);acts_lcd_irq(s);
}
static void acts_sdma1_irq(ActsState *s)
{
    qemu_set_irq(s->sdma1_irq, !!(s->regs[0x78000 / 4] & s->regs[0x78004 / 4] & 2));
}

static void acts_de_irq(ActsState *s)
{
    qemu_set_irq(s->de_irq, !!(s->regs[0x6c00c / 4] & s->regs[0x6c30c / 4]));
}

static bool acts_de_overlay(ActsState *s)
{
    uint32_t *r = &s->regs[0x6c000 / 4];
    unsigned width = (r[0x10 / 4] & 0xffff) + 1;
    unsigned height = (r[0x10 / 4] >> 16) + 1;
    uint32_t dst = r[0x31c / 4], pitch = r[0x320 / 4];
    unsigned fmt = (r[0] >> 2) & 3;
    bool fill = r[0] & 0x20;
    bool display = (r[0] & 0x300) == 0;
    unsigned bytes_per_pixel = fmt == 3 ? 4 : 2;
    Wp2DeLayer layers[3];
    if ((r[0] & 0x13c0) != (display ? 0 : 0x200) ||
        (fmt && !(fmt == 3 && fill && !display)) || width > 466 || height > 466 ||
        (!display && (pitch < width * bytes_per_pixel || pitch > 16384 ||
                     dst < 0x18000000 || (uint64_t)dst + height * pitch > 0x40000000)) ||
        (fill && (r[0x324 / 4] || r[0x328 / 4] != r[0x10 / 4]))) {
        return false;
    }
    for (unsigned i = 0; i < 3; i++) {
        if (!wp2_de_layer(&layers[i], r, i)) return false;
    }
    unsigned alpha = r[0x300 / 4] & 255;
    uint32_t bg = r[0x14 / 4];
    if (fill) bg = r[0x32c / 4];
    uint16_t background = ((bg >> 8) & 0xf800) | ((bg >> 5) & 0x7e0) | ((bg >> 3) & 31);
    for (unsigned y = 0; y < height; y++) {
        uint16_t row[466];
        uint8_t pixels[466 * 4];
        for (unsigned x = 0; x < width; x++) row[x] = background;
        if (!fill) {
            for (unsigned i = 0; i < 3; i++) {
                if (!wp2_de_row(&layers[i], y, width, alpha, s->de_palette[i], row)) return false;
            }
        }
        for (unsigned x = 0; x < width; x++) {
            if (display) {
                int px = (int)s->panel_x0 - 8 + x;
                unsigned py = s->panel_y0 + y;
                if (px >= 0 && px < 466 && py < 466) {
                    uint8_t *pixel = &s->panel[(py * 466 + px) * 3];
                    uint16_t rgb = row[x];
                    pixel[0] = (rgb >> 11) * 255 / 31;
                    pixel[1] = ((rgb >> 5) & 63) * 255 / 63;
                    pixel[2] = (rgb & 31) * 255 / 31;
                }
            } else if (fmt == 3) {
                stl_le_p(pixels + x * 4, bg);
            } else {
                stw_le_p(pixels + x * 2, row[x]);
            }
        }
        if (!display && address_space_write(&address_space_memory, dst + y * pitch,
                                MEMTXATTRS_UNSPECIFIED, pixels,
                                width * bytes_per_pixel) != MEMTX_OK) {
            return false;
        }
    }
    if (display) {
        s->panel_de_active = true;
        s->regs[0x6402c / 4] &= ~(1u << 31);
        s->regs[0x64048 / 4] |= 8;
        acts_panel_save(s);
        acts_lcd_irq(s);
    }
    r[0x30c / 4] |= 1u << (display ? 31 : 30);
    acts_de_irq(s);
    return true;
}

static bool acts_sdma1_copy(ActsState *s)
{
    uint32_t *ch = &s->regs[0x78200 / 4];
    uint32_t *line = &s->regs[0x78c20 / 4];
    bool fill = ch[0] & 2, strided = ch[0] & (1u << 24);
    unsigned width = strided ? line[0] : ch[6];
    unsigned height = strided ? line[1] : 1;
    unsigned src_stride = strided ? line[2] : width;
    unsigned dst_stride = strided ? line[3] : width;
    if ((ch[0] & ~0x1000002u) || !width || !height ||
        (uint64_t)width * height != ch[6] || ch[6] >= (1 << 20) ||
        (!fill && ((uint64_t)ch[2] + (uint64_t)(height - 1) * src_stride + width > 0x40000000)) ||
        ch[4] < 0x18000000 ||
        (uint64_t)ch[4] + (uint64_t)(height - 1) * dst_stride + width > 0x40000000) {
        return false;
    }
    g_autofree uint8_t *row = g_malloc(width);
    if (fill) {
        uint8_t pattern[12];
        for (unsigned i = 0; i < 3; i++) {
            stl_le_p(pattern + 4 * i, s->regs[0x78d00 / 4 + i]);
        }
        for (unsigned i = 0; i < width; i++) {
            row[i] = pattern[i % sizeof(pattern)];
        }
    }
    for (unsigned y = 0; y < height; y++) {
        if ((!fill && address_space_read(&address_space_memory, ch[2] + y * src_stride,
                                         MEMTXATTRS_UNSPECIFIED, row, width) != MEMTX_OK) ||
            address_space_write(&address_space_memory, ch[4] + y * dst_stride,
                                MEMTXATTRS_UNSPECIFIED, row, width) != MEMTX_OK) {
            return false;
        }
    }
    ch[1] = ch[7] = line[4] = line[5] = 0;
    s->regs[0x78000 / 4] |= 2;
    acts_sdma1_irq(s);
    return true;
}
static void acts_dma_irqs(ActsState *s)
{
    uint32_t pd=s->regs[0x1c000/4]&s->regs[0x1c004/4];
    for(unsigned i=0;i<10;i++) qemu_set_irq(s->dma_irq[i],!!(pd&((1u<<i)|(1u<<(i+16)))));
}

enum {
    DAC_BASE = 0x5c000,
    DAC_PCM_CTL = DAC_BASE + 0x10,
    DAC_PCM_STAT = DAC_BASE + 0x14,
    DAC_PCM_HE = DAC_BASE + 0x18,
    DAC_PCM_HF = DAC_BASE + 0x1c,
    DAC_PCM_COUNT = DAC_BASE + 0x30,
    DAC_PCM_CAPACITY = 1024,
};

static void acts_dma_run(ActsState *s, unsigned ch);

static void acts_dac_audio_write(void *opaque, int available)
{
    ActsState *s = opaque;
    size_t count = MIN((unsigned)MAX(available, 0), s->dac_host_pcm->len) & ~1u;
    if (count) {
        size_t written = AUD_write(s->dac_voice, s->dac_host_pcm->data, count);
        g_byte_array_remove_range(s->dac_host_pcm, 0, written);
    }
}

static void acts_dac_audio_stop(ActsState *s)
{
    if (s->dac_voice) {
        AUD_set_active_out(s->dac_voice, 0);
        g_byte_array_set_size(s->dac_host_pcm, 0);
    }
}

static void acts_dac_audio_play(ActsState *s, unsigned rate, unsigned played)
{
    if (!s->dac_host_pcm) {
        return;
    }
    if (!s->dac_voice || s->dac_host_rate != rate) {
        struct audsettings format = {
            .freq = rate, .nchannels = 1, .fmt = AUDIO_FORMAT_S16,
            .endianness = 0,
        };
        acts_dac_audio_stop(s);
        s->dac_voice = AUD_open_out(&s->dac_card, s->dac_voice, "WP2 DAC",
                                    s, acts_dac_audio_write, &format);
        if (!s->dac_voice) {
            return;
        }
        s->dac_host_rate = rate;
    }
    /* Leopard volume codes step 0.375 dB from 0xbe. ZERODT mutes the DAC. */
    unsigned level = s->regs[(DAC_BASE + 0x28) / 4] & 0xff;
    bool mute = s->regs[(DAC_BASE + 0x34) / 4] & (1u << 3);
    double gain = mute ? 0 : pow(10.0, ((int)level - 0xbe) * 0.01875);
    uint8_t pcm[96 * 2];
    for (unsigned i = 0; i < played; i++) {
        int sample = s->dac_pcm[(s->dac_pcm_head + i) % DAC_PCM_CAPACITY] * gain;
        stw_le_p(pcm + i * 2, CLAMP(sample, INT16_MIN, INT16_MAX));
    }
    /* Bound host backlog independently of guest DMA and sample counters. */
    unsigned limit = (rate / 5) * 2;
    unsigned total = s->dac_host_pcm->len + played * 2;
    if (total > limit) {
        g_byte_array_remove_range(s->dac_host_pcm, 0, total - limit);
    }
    g_byte_array_append(s->dac_host_pcm, pcm, played * 2);
    AUD_set_active_out(s->dac_voice, 1);
}

static void acts_dac_irqs(ActsState *s)
{
    uint32_t pending = s->regs[DAC_PCM_STAT / 4] >> 12;
    uint32_t count = s->regs[DAC_PCM_COUNT / 4];
    qemu_set_irq(s->dac_fifo_irq, !!(pending & s->regs[DAC_PCM_CTL / 4] & 0xf0));
    qemu_set_irq(s->dac_irq, (count & 0x60000) == 0x60000);
}

static void acts_dac_tick(void *opaque)
{
    static const unsigned rates[] = {
        8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000,
        88200, 96000,
    };
    ActsState *s = opaque;
    unsigned rate = (s->regs[DAC_BASE / 4] >> 4) & 15;
    if (rate >= ARRAY_SIZE(rates) || !s->dac_queued) {
        return;
    }
    unsigned budget = rates[rate] + s->dac_fraction;
    unsigned old = s->dac_queued;
    unsigned played = MIN(old, budget / 1000);
    acts_dac_audio_play(s, rates[rate], played);
    s->dac_pcm_head = (s->dac_pcm_head + played) % DAC_PCM_CAPACITY;
    s->dac_fraction = budget % 1000;
    s->dac_queued -= played;
    unsigned half_empty = s->regs[DAC_PCM_HE / 4] & 0xfff;
    if (old > half_empty && s->dac_queued <= half_empty) {
        s->regs[DAC_PCM_STAT / 4] |= 1u << 17;
    }
    if (!s->dac_queued) {
        s->regs[DAC_PCM_STAT / 4] |= 1u << 19;
    }
    uint32_t *count = &s->regs[DAC_PCM_COUNT / 4];
    if (*count & (1u << 16)) {
        unsigned next = (*count & 0xffff) + played;
        *count = (*count & 0x70000) | next;
        if (next >= 0x10000) {
            *count = (*count & ~0xffffu) | (next & 0xffff) | (1u << 18);
        }
    }
    if (s->dac_queued) {
        timer_mod(s->dac_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 1000000);
    }
    for (unsigned ch = 0; ch < 10; ch++) {
        uint32_t *dma = &s->regs[(0x1c100 + ch * 0x100) / 4];
        if (((dma[0] >> 8) & 31) == 11 && (dma[1] & 1)) {
            acts_dma_run(s, ch);
        }
    }
    acts_dac_irqs(s);
}

static void acts_dac_queue(ActsState *s, unsigned samples)
{
    unsigned old = s->dac_queued;
    s->dac_queued += samples;
    unsigned half_full = s->regs[DAC_PCM_HF / 4] & 0xfff;
    if (old < half_full && s->dac_queued >= half_full) {
        s->regs[DAC_PCM_STAT / 4] |= 1u << 16;
    }
    if (s->dac_queued == DAC_PCM_CAPACITY) {
        s->regs[DAC_PCM_STAT / 4] |= 1u << 18;
    }
    if (!old) {
        timer_mod(s->dac_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 1000000);
    }
    acts_dac_irqs(s);
}

static void acts_dma_run(ActsState *s, unsigned ch)
{
    uint32_t *r=&s->regs[(0x1c100+ch*0x100)/4];
    if(!(r[1]&1)) return;
    uint32_t src=r[2],dst=r[4],len=r[6]&0x7ffff;
    if (((r[0] >> 8) & 31) == 11 && (r[0] & 31) == 0 &&
        !(r[0] & (1u << 18))) {
        unsigned bytes = (r[0] & (1u << 20)) ? 2 : 4;
        unsigned samples = len / bytes;
        if (!samples || len % bytes || samples > DAC_PCM_CAPACITY ||
            !address_space_access_valid(&address_space_memory, src, len, false,
                                        MEMTXATTRS_UNSPECIFIED)) {
            error_report("Unsupported DAC DMA%u ctl=%08x src=%08x len=%u",
                         ch, r[0], src, len);
            qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
            return;
        }
        if (samples > DAC_PCM_CAPACITY - s->dac_queued) {
            r[7] = len;
            return;
        }
        if (s->dac_host_pcm) {
            uint8_t pcm[DAC_PCM_CAPACITY * 4];
            if (address_space_read(&address_space_memory, src, MEMTXATTRS_UNSPECIFIED,
                                   pcm, len) != MEMTX_OK) {
                error_report("Cannot read PCM for DAC DMA%u", ch);
                qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
                return;
            }
            unsigned tail = s->dac_pcm_head + s->dac_queued;
            for (unsigned i = 0; i < samples; i++) {
                /* 32-bit FIFO writes carry a left-aligned sample. */
                s->dac_pcm[(tail + i) % DAC_PCM_CAPACITY] =
                    lduw_le_p(pcm + i * bytes + bytes - 2);
            }
        }
        acts_dac_queue(s, samples);
        goto complete;
    }
    /* Actions DMA peripheral request 10 supplies SPI3 base; library passes
     * only FIFO offset 0x0c/0x08 in SADDR/DADDR. */
    if((r[0]&31)==10) src=0x4003400c;
    if(((r[0]>>8)&31)==10) dst=0x40034008;
    int uart=-1;
    for(unsigned i=0;i<3;i++) if(dst==0x40038008+i*0x4000) uart=i;
    if(s->nand.logs<40) qemu_log_mask(LOG_UNIMP,"DMA%u ctl=%08x src=%08x dst=%08x len=%u spi=%08x\n",ch,r[0],src,dst,len,s->nand.ctl);
    if((src==0x4003400c && !(s->nand.ctl&1)) ||
       (dst==0x40034008 && !(s->nand.ctl&2))) return;
    if((src>=0x40000000 && src!=0x4003400c) ||
       (dst>=0x40000000 && dst!=0x40034008 && uart<0) || (r[0]&(1u<<18))) {
        error_report("Unsupported DMA%u ctl=%08x src=%08x dst=%08x len=%u",ch,r[0],src,dst,len);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);return;
    }
    uint8_t *buf=g_malloc(len);
    if(src==0x4003400c) for(unsigned j=0;j<len;j++) buf[j]=nand_rx(&s->nand);
    else cpu_physical_memory_read(src,buf,len);
    if(uart>=0) qemu_chr_fe_write_all(&s->uart[uart],buf,len);
    else if(dst==0x40034008) {
        for(unsigned j=0;j<len;j++) {
            if(s->nand.txlen<sizeof(s->nand.tx)) s->nand.tx[s->nand.txlen++]=buf[j];
        }
        s->nand.bc=0;
    } else cpu_physical_memory_write(dst,buf,len);
    g_free(buf);
complete:
    r[7]=0;r[1]&=~1u;
    s->regs[0x1c000/4]|=(1u<<ch)|(1u<<(ch+16));
    acts_dma_irqs(s);
}
static const unsigned i2c_base[4]={0x44000,0x48000,0x4c000,0x74000};
static int acts_i2c_index(hwaddr addr)
{
    for(int i=0;i<4;i++) if(addr>=i2c_base[i] && addr<=i2c_base[i]+0x28) return i;
    return -1;
}

/* SDK soc_memctrl.c programs mapping then entry; entries refer to physical
 * NOR. Implement non-CRC aliases first, explicitly stop on CRC mappings. */
static void acts_map_update(ActsState *s, unsigned i)
{
    uint32_t map=s->regs[(0x10300+i*8)/4];
    uint32_t entry=s->regs[(0x10304+i*8)/4];
    memory_region_transaction_begin();
    if(s->mapped[i]) {
        memory_region_del_subregion(get_system_memory(), &s->maps[i]);
        s->mapped[i]=false;
    }
    if(map && !(entry & 0x80000000) && entry < 0x400000) {
        memory_region_set_alias_offset(&s->maps[i], entry);
        memory_region_set_size(&s->maps[i], MIN(0x400000-entry,
                               ((uint64_t)(map & 0x3f)+1)<<20));
        memory_region_add_subregion_overlap(get_system_memory(),map & ~0x3fu,
                                            &s->maps[i], 10+i);
        s->mapped[i]=true;
    } else if(map && entry & 0x80000000) {
        error_report("ATS3089C unsupported CRC mapping slot %u map=%08x entry=%08x",i,map,entry);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
    }
    memory_region_transaction_commit();
}

/* SDK system timer selects the 32 MHz HOSC. Other clock selectors are TODO. */
static uint32_t acts_count(ActsTimer *t)
{
    if (!(t->ctl & (1 << 5))) {
        return t->count;
    }
    return t->count + (qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) - t->started) * 32 / 1000;
}

static void acts_timer_arm(ActsTimer *t)
{
    timer_del(t->timer);
    if (t->ctl & (1 << 5)) {
        uint32_t remaining = t->value - acts_count(t);
        timer_mod(t->timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) +
                  MAX(1ULL, ((uint64_t)remaining * 1000 + 31) / 32));
    }
}

static void acts_timer_expired(void *opaque)
{
    ActsTimer *t = opaque;
    t->ctl |= 1;
    qemu_set_irq(t->irq, !!(t->ctl & 2));
    if (t->ctl & 0x400) {
        /* SDK 0xe22 compare/continuous mode: keep counting while ISR
         * acknowledges the interrupt and programs the next comparison. */
    } else if (t->ctl & 4) {
        t->count = 0;
        t->started = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        acts_timer_arm(t);
    } else {
        t->count = t->value;
        t->ctl &= ~(1 << 5);
    }
}

static void acts_unknown(ActsState *s, hwaddr addr, bool write, uint64_t value)
{
    unsigned idx = addr / 4;
    uint8_t bit = write ? 2 : 1;
    if (!(s->seen[idx] & bit)) {
        qemu_log_mask(LOG_UNIMP, "ATS3089C UNIMPLEMENTED %s 0x%08" HWADDR_PRIx
                      " value=0x%08" PRIx64 "\n", write ? "write" : "read",
                      addr + 0x40000000, value);
        s->seen[idx] |= bit;
    }
    if (!s->exploratory) {
        error_report("ATS3089C stopped on unimplemented %s at 0x%08" HWADDR_PRIx,
                     write ? "write" : "read", addr + 0x40000000);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
    }
}

static bool acts_storage_reg(hwaddr addr)
{
    /* Clock enables/dividers and reset registers retain written state. Effects
     * on peripherals and analog oscillator settling are not implemented yet. */
    return addr < 8 || (addr >= 0x100 && addr <= 0x174) ||
           addr == 0x2c03c || addr == 0x2c040 || /* PSRAM PHY delay taps */
           (addr >= 0x1000 && addr <= 0x1114) ||
           addr == 0x4030 ||
           addr == 0x4000 || addr == 0x4004 || addr == 0x4008 ||
           addr == 0x400c || addr == 0x4014 || addr == 0x4040 || addr == 0x4044 ||
           addr == 0x4110 || addr == 0x8004 || addr == 0x8008 ||
           (addr >= 0x4100 && addr <= 0x410c) || /* charger/power config */
           addr == 0x4200 || addr == 0x4204 || addr == 0x420c ||
           (addr >= 0x68000 && addr <= 0x68140) || /* pinmux config */
           (addr >= 0x68200 && addr <= 0x68208) ||
           (addr >= 0x68300 && addr <= 0x6830c) || /* WIO pin controls */
           addr == 0x68400 || /* JTAG mux config */
           (addr >= 0xc030 && addr <= 0xc044) || /* RTC retention words */
           addr == 0x28000 || addr == 0x28024 || /* SPI0 ctrl/delay; HLE */
           addr == 0x34000 || /* SPI3 configuration, transfer registers separate */
           (addr >= 0x10300 && addr <= 0x1033c) ||
           addr == 0x88000 || addr == 0x8c000 || /* I2CMT control, idle only */
           addr == 0x78000 || addr == 0x78004 ||
           (addr>=0x64000 && addr<=0x640ac) ||
           (addr>=0x78100 && addr<=0x7811c) ||
           (addr>=0x78200 && addr<=0x7821c) ||
           (addr>=0x78c00 && addr<=0x78c14) ||
           (addr>=0x78c20 && addr<=0x78c34) ||
           (addr>=0x78d00 && addr<=0x78d08) ||
           (addr>=0x6c000 && addr<=0x6c7fc) || /* DE config; starts handled separately */
           (addr>=0x60000 && addr<=0x60818 && (addr&255)<=0x18) || /* PWM config */
           (addr >= 0xc000 && addr <= 0xc01c) || /* RTC calendar/alarm */
           addr == 0xc1d0 || /* HCL oscillator config, ideal clock */
           addr == 0x4118 || addr == 0x411c || addr == 0x4120 ||
           (addr >= 0x5c118 && addr <= 0x5c134) || /* audio ADC analogue config */
           (addr >= 0x5c100 && addr <= 0x5c10c) ||
           addr == 0x5c000 || addr == 0x5c004 ||
           (addr >= 0x5c010 && addr <= 0x5c050 && addr != 0x5c014) ||
           addr == 0x14000 || addr == 0x18000 || addr == 0x18094;
}

static uint64_t acts_read(void *opaque, hwaddr addr, unsigned size)
{
    ActsState *s = opaque;
    if (addr == 0x64020 && (s->regs[0x6402c / 4] & (1u << 28)) &&
        (s->regs[0x64034 / 4] >> 24) == 0x03) {
        /* CO5300 QSPI status registers used by the stock panel check. */
        switch ((s->regs[0x64034 / 4] >> 8) & 0xff) {
        case 0x0a:
            return s->panel_on ? 0x9c : 0x08;
        case 0x0d:
        case 0x0f:
        case 0x66:
            return 0;
        }
    }
    if (addr == 0xfffe0 && s->health_hook) {
        return s->health_readings;
    }
    if ((addr >= 0x94000 && addr <= 0x9405c) ||
        (addr >= 0x780a0 && addr <= 0x780b4) ||
        (addr >= 0x78300 && addr <= 0x7851c && (addr & 255) <= 0x1c) ||
        (addr >= 0x78c40 && addr <= 0x78c94)) {
        return s->regs[addr / 4];
    }
    if (addr == 0x3010 || addr == 0x301c) {
        return s->regs[addr / 4];
    }
    if (addr >= 0x20400 && addr <= 0x20408) {
        return s->regs[addr / 4];
    }
    /* Crypto engines are idle at boot. Active requests remain strict. */
    if (addr == 0x20000 || addr == 0x20200 || addr == 0x20600) {
        return 0;
    }
    if (((addr & ~0x3fff) == 0x88000 || (addr & ~0x3fff) == 0x8c000) &&
        ((addr & 0x3fff) <= 0x14 || (addr & 0x3fff) == 0x20 ||
         ((addr & 0x3fff) >= 0x100 && (addr & 0x3fff) < 0x200 &&
          (addr & 0x3f) <= 0x14))) {
        return s->regs[addr / 4];
    }
    if(addr==0x6c008) return 0; /* configuration latch, no active transfer */
    if(addr==0x70004) return 0x0b05; /* VG Lite idle, before submission */
    if(addr==0x70020) return 0x265; /* SDK GC265 */
    if(addr==0x70024) return 0x2001; /* SDK REVISION */
    if(addr==0x70030) return 0x421; /* SDK CID */
    if(addr==0x700e8) return 0; /* SDK ECOID */
    if (addr == 0x70010) {
        uint32_t pending = s->regs[addr / 4];
        s->regs[addr / 4] = 0;
        qemu_set_irq(s->vg_irq, 0);
        return pending;
    }
    if(addr==0x70000 || addr==0x70010 || addr==0x70014 ||
       addr==0x70100 || addr==0x70104 || addr==0x70500) return s->regs[addr/4];
    if(addr==0x1c000 || addr==0x1c004) return s->regs[addr/4];
    if(addr>=0x1c100 && addr<=0x1ca1c && (addr&255)<=0x1c) {
        acts_dma_run(s,(addr-0x1c100)/0x100);return s->regs[addr/4];
    }
    if (addr == 0x14004 || addr == 0x18004) return 0; /* coherent cache aliases */
    if (addr == 0x18008 || addr == 0x1800c) return s->regs[addr / 4];
    if (addr == 0xc004) return 0x5a69; /* RTC update acknowledged */
    if (s->host_rtc && addr >= 0xc014 && addr <= 0xc01c) {
        g_autoptr(GDateTime) now = g_date_time_new_now_local();
        if (addr == 0xc014) return g_date_time_get_microsecond(now) / 10000;
        if (addr == 0xc018) {
            return (g_date_time_get_hour(now) << 16) |
                   (g_date_time_get_minute(now) << 8) |
                   g_date_time_get_second(now);
        }
        return (((g_date_time_get_year(now) - 2000) & 0x7f) << 16) |
               (g_date_time_get_month(now) << 8) | g_date_time_get_day_of_month(now);
    }
    int bus=acts_i2c_index(addr);
    if(bus>=0) {
        unsigned off=addr-i2c_base[bus];
        if(off==0x1c) return s->regs[addr/4]&~6u;
        if (off == 0x14 && s->i2c_left[bus]) {
            s->i2c_left[bus]--;
            return s->i2c_rx[bus][s->i2c_index[bus]++];
        }
        if (off == 0x20) {
            unsigned remaining = s->i2c_left[bus];
            return s->regs[addr/4] | 0x10 | (remaining ? remaining << 8 : 4);
        }
        return s->regs[addr/4];
    }
    if (addr == 0x4010) return 0x1000 | s->charger_present;
    if (addr == 0x4108) return (s->regs[addr / 4] & ~(1u << 17)) |
                             (s->crown_down ? 1u << 17 : 0);
    if (addr == 0x4118) return (s->regs[addr/4]&0xfc000000) |
          ((qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL)/125000000)&0x3ffffff);
    if (addr == DAC_PCM_STAT) {
        return s->regs[addr / 4] | (DAC_PCM_CAPACITY - s->dac_queued);
    }
    if (addr == 0x5c008) return 16; /* DAC FIFO empty */
    if (addr == 0x4034) return s->regs[0x4030/4]; /* ideal power acknowledgement */
    if (addr>=0x34000 && addr<=0x34010) return nand_reg_read(&s->nand,addr-0x34000);
    if (addr >= 0x68230 && addr <= 0x68238) {
        unsigned bank=(addr-0x68230)/4;
        uint32_t input=0;
        for(unsigned bit=0;bit<32 && bank*32+bit<81;bit++) {
            uint32_t cfg=s->regs[(0x68000+(bank*32+bit)*4)/4];
            if(cfg & (1<<6)) input |= s->regs[0x68200/4+bank] & (1u<<bit);
            else if(cfg & ((1<<8)|(1<<11))) input |= 1u<<bit;
        }
        if (bank == 1 && s->input_path) {
            input = s->crown_delta ? input & ~(1u << 26) : input | (1u << 26);
        }
        return input;
    }
    if (addr >= 0x68240 && addr <= 0x68248) return s->regs[addr / 4];
    /* Ideal continuously sampled inputs. BATV uses SDK 300/1024 mV per count. */
    if (addr == 0x4208) return s->regs[addr/4];
    if (addr >= 0x4210 && addr <= 0x423c) {
        if(addr==0x4214) return s->battery_mv * 1024 / 300;
        if(addr==0x4218) return s->charger_present ? 3412 : 0;
        if(addr==0x4224) return 1023; /* no resistor key pressed */
        if (addr == 0x422c) return s->regs[0x420c / 4] & (1u << 5) ? 480 : 1000;
        return 0;
    }
    if (addr == 0x28004) {
        return 1u << 8; /* SPI_STA_READY: synchronous HLE backend */
    }
    if (addr == 0x168) {
        uint32_t value=s->regs[addr / 4];
        return value | ((value & 1) ? 1u << 4 : 0);
    }
    if (addr == 0x16c) {
        uint32_t cycles=s->regs[0x168 / 4] >> 8;
        unsigned select=(s->regs[0x160 / 4] >> 16) & 15;
        return select == 3 ? (uint64_t)cycles * 8192 :
               (uint64_t)cycles * 32000000 / 32768;
    }
    if (addr == 0x8000) {
        return s->regs[addr / 4];
    }
    if (addr >= 0x8010 && addr <= 0x804c) {
        return 0; /* synthetic blank OTP, not physical device fuse values */
    }
    if (addr >= 0xa0 && addr <= 0xac) {
        return 0; /* synthetic, non-identifying chip UID/revision */
    }
    if (addr == 0x108) {
        /* soc_pm.c calibration start/done handshake, ideal nominal clock. */
        uint32_t value = s->regs[addr / 4];
        if (value & 8) {
            value = (value & 0x01ffffff) | (64u << 25) | (1u << 24) | (1u << 8);
        }
        return value;
    }
    if (addr == 0x4114) {
        return s->regs[addr / 4];
    }
    if (addr == 0xc020 || (addr >= 0x90000 && addr <= 0x9003c)) {
        /* Initial disabled watchdog/PPI state; enabling is not yet modeled. */
        return s->regs[addr / 4];
    }
    if (addr == 0x100) {
        /* soc.c: HOSC enable bit 25, ready bit 28. Ideal oscillator,
         * ready immediately; analog settling time is not simulated. */
        uint32_t value = s->regs[addr / 4] & ~(1u << 28);
        return value | ((value & (1u << 25)) ? 1u << 28 : 0);
    }
    if (addr >= 0xc100 && addr < 0xc1c0) {
        ActsTimer *t = &s->timers[(addr - 0xc100) / 0x20];
        switch ((addr - 0xc100) % 0x20) {
        case 0: return t->ctl;
        case 4: return t->value;
        case 8:
            /* SDK mbrec uses TIMER0 in periodic down-counter mode. */
            if (addr == 0xc108 && (t->ctl & ~1u) == 0x24) {
                return t->value - acts_count(t);
            }
            return acts_count(t);
        }
    }
    if (addr == GPS_UART_RX) {
        if (!s->gps_available) {
            return 0;
        }
        uint8_t byte = s->gps_sentence[s->gps_pos++];
        s->gps_available--;
        acts_gps_irq(s);
        return byte;
    }
    if (addr == GPS_UART_STATUS && s->gps_available) {
        return (1u << 10) | (s->gps_available << 11) | 1;
    }
    for (unsigned i = 0; i < 3; i++) {
        if (addr == 0x3800c + i * 0x4000) {
            return (1 << 10) | (1 << 5); /* TX empty, RX empty */
        }
        if (addr == 0x38000 + i * 0x4000 || addr == 0x38010 + i * 0x4000) {
            return s->regs[addr / 4];
        }
    }
    if (!acts_storage_reg(addr)) {
        acts_unknown(s, addr, false, s->regs[addr / 4]);
    }
    return s->regs[addr / 4];
}

static void acts_write(void *opaque, hwaddr addr, uint64_t value, unsigned size)
{
    ActsState *s = opaque;
    if (addr == 0 && !(value & (1u << 10))) { /* RMU USB controller reset */
        s->usb_idvbus = 0;
        s->usb_dpdm = 1u << 4; /* LSDETEN reset default */
    }
    if ((addr >= 0x94000 && addr <= 0x9405c) ||
        (addr >= 0x780a0 && addr <= 0x780b4) ||
        (addr >= 0x78300 && addr <= 0x7851c && (addr & 255) <= 0x1c) ||
        (addr >= 0x78c40 && addr <= 0x78c94)) {
        s->regs[addr / 4] = value;
        if (addr == 0x94004) {
            qemu_set_irq(s->jpeg_irq, !!value);
        }
        if (addr == 0x94000 && (value & 1)) {
            if (wp2_jpeg_decode(s->regs)) {
                s->regs[addr / 4] &= ~1u;
                s->regs[0x94004 / 4] |= 2;
                qemu_set_irq(s->jpeg_irq, !!(value & 4));
                return;
            }
            for (unsigned i = 0x780a0; i <= 0x9405c; i += 4) {
                if ((i <= 0x78c94 || i >= 0x94000) && s->regs[i / 4])
                    qemu_log_mask(LOG_UNIMP, "JPEG %05x = %08x\n", i, s->regs[i / 4]);
            }
            acts_unknown(s, addr, true, value);
        }
        return;
    }
    if (addr == 4) {
        uint32_t old = s->regs[addr / 4];
        s->regs[addr / 4] = value;
        if (!(value & (1u << 24))) {
            s->bt.ready = false;
            s->bt.rx = s->bt.tx = 0;
            s->regs[0x301c / 4] = 0;
            qemu_set_irq(s->bt_irq, 0);
        } else if (!(old & (1u << 24)) &&
                   wp2_bt_boot(&s->bt, memory_region_get_ram_ptr(&s->shared))) {
            s->regs[0x301c / 4] = 1;
            qemu_set_irq(s->bt_irq, 1);
        }
        return;
    }
    if (addr == 0x301c) {
        s->regs[addr / 4] &= ~value;
        qemu_set_irq(s->bt_irq, !!s->regs[addr / 4]);
        return;
    }
    if (addr == 0x3010) {
        s->regs[addr / 4] = value;
        if ((value & 1) && s->bt_external) {
            if (!wp2_bt_send_h4(&s->bt, memory_region_get_ram_ptr(&s->shared),
                                 &s->bt_backend)) acts_bt_fault("command transport failed");
            return;
        }
        if ((value & 1) && wp2_bt_service(&s->bt, memory_region_get_ram_ptr(&s->shared))) {
            s->regs[0x301c / 4] = 1;
            qemu_set_irq(s->bt_irq, 1);
        }
        return;
    }
    if (addr == 0x20400) {
        uint32_t old = s->regs[addr / 4];
        s->regs[addr / 4] = value & ~(1u << 31);
        if ((value & 1) && !(old & 1)) {
            qemu_guest_getrandom_nofail(&s->regs[0x20404 / 4], 8);
            s->regs[addr / 4] |= 1u << 31;
        }
        return;
    }
    unsigned base_mt = addr & ~0x3fff;
    unsigned off_mt = addr & 0x3fff;
    if (base_mt == 0x88000 || base_mt == 0x8c000) {
        unsigned bus = base_mt == 0x8c000;
        uint32_t *regs = &s->regs[base_mt / 4];
        if (off_mt == 0x14 || off_mt == 4) {
            regs[off_mt / 4] &= ~value;
        } else if (off_mt == 0) {
            regs[0] = value & ~2u;
            if (value & 2) {
                regs[1] |= 0x109; /* TCB, IRQ pending and address NACK */
            }
        } else if (off_mt == 8 || off_mt == 0x10 || off_mt == 0x20 ||
                   (off_mt >= 0x100 && off_mt < 0x200 && (off_mt & 0x3f) <= 0x14)) {
            regs[off_mt / 4] = value;
            if (off_mt >= 0x100) {
                unsigned task_id = (off_mt - 0x100) / 0x40;
                unsigned field = off_mt & 0x3f;
                uint32_t *task = regs + 0x100 / 4 + 16 * task_id;
                if (bus == 1 && field == 4 && !(value & 1)) {
                    s->stk_task_offset[task_id] = 0;
                }
                if (!field && (value & (1u << 31))) {
                    bool ack = bus == 1 && acts_stk_transfer(s, task);
                    task[4] = ack ? 0 : task[3];
                    regs[5] |= 1u << (task_id + (ack ? 0 : 8));
                    qemu_log_mask(LOG_UNIMP, "I2CMT%u task %u slave %02x %s\n",
                                  bus, task_id, (task[5] >> 1) & 127, ack ? "ACK" : "NACK");
                }
            }
        } else {
            acts_unknown(s, addr, true, value);
        }
        acts_i2cmt_irq(s, bus);
        return;
    }
    if (addr == 0x78000 || addr == 0x78004) {
        if (addr == 0x78000) {
            s->regs[addr / 4] &= ~value;
        } else {
            s->regs[addr / 4] = value;
        }
        acts_sdma1_irq(s);
        return;
    }
    if (addr == 0x78204 && value == 1) {
        if (!acts_sdma1_copy(s)) {
            acts_unknown(s, addr, true, value);
        }
        return;
    }
    /* WIC/PPI routing can be configured with no external sensor triggers. */
    if (addr >= 0x90000 && addr <= 0x9003c) {
        if (addr == 0x90008 || addr == 0x9000c) {
            s->regs[addr / 4] &= ~value;
        } else {
            s->regs[addr / 4] = value;
        }
        return;
    }
    if (addr == 0x70504) {
        if (acts_vg_submit(s, value)) {
            return;
        }
        uint32_t base = s->regs[0x70500 / 4];
        qemu_log_mask(LOG_UNIMP, "VG Lite submission @%08x, units=%" PRIu64 "\n", base, value);
        for (unsigned i = 0; i < MIN(value, 96); i++) {
            uint8_t pair[8];
            cpu_physical_memory_read(base + i * 8, pair, sizeof(pair));
            qemu_log_mask(LOG_UNIMP, "VG[%03u] %08x %08x\n",
                          i, ldl_le_p(pair), ldl_le_p(pair + 4));
        }
        acts_unknown(s, addr, true, value);
        return;
    }
    if(addr==0x70000 || addr==0x70014 || addr==0x70100 ||
       addr==0x70104 || addr==0x70500) {s->regs[addr/4]=value;return;}
    if (addr == 0x6c020 && (value & 1) && acts_de_overlay(s)) {
        s->regs[addr / 4] = value & ~1u;
        return;
    }
    if (addr == 0x6c400 && value == 0x80) {
        s->regs[addr / 4] &= ~0x80u; /* RT_STAT_COMPLETE, W1C */
        return;
    }
    if (addr == 0x6c338) {
        s->de_palette_index = value & 255;
        s->regs[addr / 4] = value;
        return;
    }
    if (addr == 0x6c33c && (s->regs[0x6c338 / 4] >> 8) < 3) {
        unsigned layer = s->regs[0x6c338 / 4] >> 8;
        s->de_palette[layer][s->de_palette_index] = value;
        s->de_palette_index = (s->de_palette_index + 1) & 255;
        s->regs[addr / 4] = value;
        return;
    }
    if((addr==0x6c020 && (value&1)) || (addr==0x6c400 && value)) {
        for (unsigned i = 0; i < 0x800; i += 4) {
            uint32_t word = s->regs[(0x6c000 + i) / 4];
            if (word) {
                qemu_log_mask(LOG_UNIMP, "DE register %03x = %08x\n", i, word);
            }
        }
        acts_unknown(s,addr,true,value);return;
    }
    if(addr==0x6c30c) {s->regs[addr/4]&=~value;acts_de_irq(s);return;}
    if(addr==0xc020) {
        uint32_t old=s->regs[addr/4];
        s->regs[addr/4]=(value&~65u)|((old&64)&~(value&64));
        qemu_set_irq(s->wd_irq,!!(s->regs[addr/4]&64));
        if(!(value&16)) timer_del(s->wd_timer);
        else if((value&1) || !(old&16)) timer_mod(s->wd_timer,
             qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL)+(176LL<<((value>>1)&7))*1000000);
        return;
    }
    if(addr>=0x64000 && addr<=0x640ac) {acts_lcd_write(s,addr,value);return;}
    if(addr==0x4010) return; /* W1C pending; ideal always-ready counter */
    if (addr == DAC_PCM_CTL || addr == DAC_PCM_STAT || addr == DAC_PCM_COUNT) {
        uint32_t *reg = &s->regs[addr / 4];
        if (addr == DAC_PCM_STAT) {
            *reg &= ~(value & 0xf0000);
        } else if (addr == DAC_PCM_COUNT) {
            uint32_t count = value & (1u << 16) ? *reg & 0xffff : 0;
            *reg = count | (value & 0x30000) | (*reg & ~value & 0x40000);
        } else {
            *reg = value;
        }
        acts_dac_irqs(s);
        return;
    }
    if (addr == DAC_BASE + 4 && !(value & 1)) {
        s->dac_queued = s->dac_fraction = 0;
        timer_del(s->dac_timer);
        acts_dac_audio_stop(s);
    }
    if (addr == DAC_BASE + 8) return; /* DAC FIFO status W1C */
    if(addr==0x1c000 || addr==0x1c004) {
        if(addr==0x1c000) s->regs[addr/4]&=~value;else s->regs[addr/4]=value;
        acts_dma_irqs(s);return;
    }
    if(addr>=0x1c100 && addr<=0x1ca1c && (addr&255)<=0x1c) {
        s->regs[addr/4]=value;
        if((addr&255)==4) acts_dma_run(s,(addr-0x1c100)/0x100);
        return;
    }
    if(addr>=0x34000 && addr<=0x34010) {
        nand_reg_write(&s->nand,addr-0x34000,value);
        if(addr==0x34000) for(unsigned i=0;i<10;i++) acts_dma_run(s,i);
        return;
    }
    if((addr==0x88000 || addr==0x8c000) && (value&2)) {
        acts_unknown(s,addr,true,value); return;
    }
    int bus=acts_i2c_index(addr);
    if(bus>=0) {
        unsigned base=i2c_base[bus], off=addr-base;
        if(off==8 || off==0x20) s->regs[addr/4]&=~value;
        else s->regs[addr/4]=value;
        if (off == 0x10 && s->i2c_tx_len[bus] < sizeof(s->i2c_tx[bus])) {
            s->i2c_tx[bus][s->i2c_tx_len[bus]++] = value;
        }
        if (off == 0x1c && (value & 6)) {
            s->regs[(base+0x20)/4] = 0;
            if (value & 4) s->i2c_tx_len[bus] = 0;
            if (value & 2) s->i2c_left[bus] = 0;
        }
        /* Complete transfers to attached devices, NACK other addresses. */
        if((off==0x18 && (value&0x8000)) || (off==0 && (value&2))) {
            unsigned n = s->regs[(base + 0x24) / 4];
            uint8_t *tx = s->i2c_tx[bus];
            unsigned count = s->i2c_tx_len[bus];
            bool ack = false;
            uint8_t *rx = s->i2c_rx[bus];
            if (bus == 0 && off == 0x18 && count &&
                (tx[0] >> 1) == 0x15) {
                qemu_log_mask(LOG_UNIMP, "CST820 cmd=%04" PRIx64 " n=%u count=%u tx=%02x %02x %02x\n",
                              value, n, count, tx[0], tx[1], tx[2]);
                ack = acts_cst820_transfer(s, tx, count, n, rx);
            } else if (bus == 2 && off == 0x18 &&
                       count >= 2 && tx[0] == 0xea) {
                if (count == 3 && tx[2] == 0xeb && n && n <= sizeof(s->i2c_rx[bus])) {
                    for (unsigned i = 0; i < n; i++) {
                        rx[i] = acts_knob_read(s, tx[1] + i);
                    }
                    s->i2c_index[bus] = 0;
                    s->i2c_left[bus] = n;
                    ack = true;
                } else if (count == n + 1 && count >= 3) {
                    for (unsigned i = 2; i < count; i++) {
                        s->knob_regs[(uint8_t)(tx[1] + i - 2)] = tx[i];
                    }
                    ack = true;
                }
            }
            s->i2c_tx_len[bus] = 0;
            s->regs[(base+8)/4]=0x85 | ((value&8)?0x10:0);
            s->regs[(base+0x20)/4]=3;
            if (ack) {
                s->regs[(base+8)/4] &= ~1u;
                s->regs[(base+0x20)/4] &= ~2u;
            }
        }
        qemu_set_irq(s->i2c_irq[bus], !!((s->regs[base/4]&0x40) &&
                       (s->regs[(base+8)/4]&4)));
        return;
    }
    if (addr == 0x14004 || addr == 0x18004) return;
    if (addr == 0x18008 || addr == 0x1800c) { s->regs[addr / 4] = value; return; }
    if (addr == 0x4208) {s->regs[addr/4]&=~value;acts_adc_irq(s);return;}
    if (addr == 0x4200 || addr == 0x4204) {
        s->regs[addr/4]=value;acts_adc_irq(s);
        timer_mod(s->adc_timer,qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL)+1000000);return;
    }
    if(addr>=0x68210 && addr<=0x68218) {
        s->regs[(addr-0x10)/4] |= value; return;
    }
    if(addr>=0x68220 && addr<=0x68228) {
        s->regs[(addr-0x20)/4] &= ~value; return;
    }
    if (addr >= 0x68240 && addr <= 0x68248) {
        s->regs[addr / 4] &= ~value;
        qemu_set_irq(s->gpio_irq, !!(s->regs[0x68240 / 4] |
                     s->regs[0x68244 / 4] | s->regs[0x68248 / 4]));
        return;
    }
    if (addr >= 0x10300 && addr <= 0x1033c) {
        s->regs[addr/4]=value;
        /* entry write commits; zero mapping removes the entry */
        if ((addr & 4) || !value) acts_map_update(s,(addr-0x10300)/8);
        return;
    }
    if (addr == 0x4114) {
        s->regs[addr / 4] &= ~value;
        acts_pmu_irq(s);
        return;
    }
    if (addr == 0x4040) {
        s->regs[addr / 4] = value;
        acts_pmu_irq(s);
        return;
    }
    if (addr == 0x8000) {
        /* Stock 0x10106004: load bit2 clears when bit0 becomes ready. */
        s->regs[addr / 4] = (value & ~4u) | ((value & 4u) ? 1u : 0u);
        return;
    }
    if (addr == 0x8010 && value == 0x26467358) {
        /* Stock 0x10106080: read-unlock challenge, bit1 acknowledges. */
        s->regs[0x8000 / 4] |= 2;
        return;
    }
    if (addr == 0xffff0 && (value == 0x524f4d || (value >> 24) == 0x46)) {
        error_report("ATS3089C compatibility ROM reached unsupported API (0x%" PRIx64 ")", value);
        if (current_cpu) {
            cpu_dump_state(current_cpu, stderr, 0);
        }
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
        return;
    }
    if (addr == 0xffff4 && s->nand_export) {
        GError *error = NULL;
        if (value != 0) {
            error_report("NAND provisioning failed, stage %" PRIu64, value);
        } else if (!g_file_set_contents(s->nand_export, (const char *)s->nand.data,
                                        (size_t)NAND_PAGE * NAND_PAGES, &error)) {
            error_report("NAND export failed: %s", error->message);
            g_error_free(error);
        } else {
            info_report("Exported provisioned physical NAND");
        }
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
        return;
    }
    if (addr >= 0xc100 && addr < 0xc1c0) {
        ActsTimer *t = &s->timers[(addr - 0xc100) / 0x20];
        switch ((addr - 0xc100) % 0x20) {
        case 0:
            t->count = acts_count(t);
            t->started = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
            t->ctl = (value & ~1) | ((t->ctl & 1) & ~(value & 1));
            qemu_set_irq(t->irq, !!((t->ctl & 1) && (t->ctl & 2)));
            acts_timer_arm(t);
            return;
        case 4: t->value = value; acts_timer_arm(t); return;
        case 8: t->count = value; t->started = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
                acts_timer_arm(t); return;
        }
    }
    if (addr == GPS_UART_CTL) {
        s->regs[addr / 4] = value;
        if (!(value & GPS_UART_RX_IRQ)) {
            s->gps_pos = s->gps_length = s->gps_available = 0;
        }
        acts_gps_irq(s);
        return;
    }
    for (unsigned i = 0; i < 3; i++) {
        if (addr == 0x38008 + i * 0x4000) {
            uint8_t byte = value;
            qemu_chr_fe_write_all(&s->uart[i], &byte, 1);
            return;
        }
        if (addr == 0x38000 + i * 0x4000 || addr == 0x38010 + i * 0x4000 ||
            addr == 0x3800c + i * 0x4000) {
            s->regs[addr / 4] = value;
            return;
        }
    }
    if (!acts_storage_reg(addr)) {
        acts_unknown(s, addr, true, value);
    }
    s->regs[addr / 4] = value;
}

static const MemoryRegionOps acts_ops = {
    .read = acts_read,
    .write = acts_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 4, .max_access_size = 4, .unaligned = false },
};

static uint64_t acts_usb_phy_read(void *opaque, hwaddr addr, unsigned size)
{
    ActsState *s = opaque;
    if (addr == 0) { /* IDVBUSCTRL */
        return s->usb_idvbus;
    }
    if (addr == 1) { /* DPDMCTRL: PLUGIN stays low with no USB data peer */
        return s->usb_dpdm;
    }
    /* Reset has completed and neither D+ nor D- is driven by a USB peer.
     * The watch's DC charger is modeled independently of USB data. */
    return 0;
}

static void acts_usb_phy_write(void *opaque, hwaddr addr, uint64_t value,
                               unsigned size)
{
    ActsState *s = opaque;
    if (addr == 0) {
        s->usb_idvbus = value & 0x0c;
    } else if (addr == 1) {
        s->usb_dpdm = value & 0x1f;
    }
}

static const MemoryRegionOps acts_usb_phy_ops = {
    .read = acts_usb_phy_read,
    .write = acts_usb_phy_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 1 },
    .impl = { .min_access_size = 1, .max_access_size = 1 },
};

static void acts_ram(MachineState *m, MemoryRegion *region, const char *name,
                     uint64_t addr, uint64_t size, bool readonly)
{
    memory_region_init_ram(region, NULL, name, size, &error_fatal);
    memory_region_set_readonly(region, readonly);
    memory_region_add_subregion(get_system_memory(), addr, region);
}

static void acts_reset(void *opaque)
{
    ActsState *s = opaque;
    /* The RTC retention bank carries reboot reasons across a watchdog reset. */
    uint32_t rtc_remain[6];
    memcpy(rtc_remain, &s->regs[0xc030 / 4], sizeof(rtc_remain));
    memset(s->regs, 0, sizeof(s->regs));
    memcpy(&s->regs[0xc030 / 4], rtc_remain, sizeof(rtc_remain));
    timer_del(s->dac_timer);
    s->dac_queued = s->dac_fraction = 0;
    s->dac_pcm_head = 0;
    acts_dac_audio_stop(s);
    acts_dac_irqs(s);
    s->usb_idvbus = 0;
    s->usb_dpdm = 1u << 4;
    s->bt = (Wp2Bt) {.address = {0x02, 0x00, 0x00, 0x89, 0x30, 0x02}};
    s->bt_rx_len = 0;
    timer_del(s->bt_rx_timer);
    qemu_set_irq(s->bt_irq, 0);
    memset(s->vg_state, 0, sizeof(s->vg_state));
    memset(s->de_palette, 0, sizeof(s->de_palette));
    s->de_palette_index = 0;
    memset(s->panel, 0, sizeof(s->panel));
    s->panel_on = s->panel_te_enabled = s->panel_de_active = false;
    s->panel_brightness = 255;
    s->panel_x0 = 8;
    s->panel_x1 = 473;
    s->panel_y0 = 0;
    s->panel_y1 = 465;
    timer_del(s->panel_te_timer);
    qemu_set_irq(s->gpio_irq, 0);
    qemu_set_irq(s->lcd_irq, 0);
    qemu_set_irq(s->sdma1_irq, 0);
    qemu_set_irq(s->de_irq, 0);
    qemu_set_irq(s->jpeg_irq, 0);
    qemu_set_irq(s->vg_irq, 0);
    qemu_set_irq(s->i2cmt_irq[0], 0);
    qemu_set_irq(s->i2cmt_irq[1], 0);
    for (unsigned i=0;i<8;i++) acts_map_update(s,i);
    memset(s->seen, 0, sizeof(s->seen));
    timer_del(s->adc_timer);qemu_set_irq(s->adc_irq,0);
    timer_del(s->wd_timer);qemu_set_irq(s->wd_irq,0);
    s->regs[0x4030 / 4] = 1u << 31; /* app handoff: CPU domain powered */
    s->charger_present = true;
    s->battery_mv = 4000;
    s->accel_mg[0] = s->accel_mg[1] = 0;
    s->accel_mg[2] = 1000;
    memset(s->stk_regs, 0, sizeof(s->stk_regs));
    s->stk_regs[0x0f] = 3;
    s->stk_reg = 0;
    memset(s->stk_task_offset, 0, sizeof(s->stk_task_offset));
    s->stk_next_sample = 0;
    s->gps_enabled = s->gps_gga = false;
    s->gps_pos = s->gps_length = s->gps_available = 0;
    s->gps_next_sentence = 0;
    acts_gps_irq(s);
    s->health_readings = 0;
    s->health_next_sample = 0;
    timer_del(s->tap_timer);
    memset(s->i2c_left, 0, sizeof(s->i2c_left));
    s->touch_down = false;
    s->touch_reg = 0;
    s->auto_touch = false;
    s->crown_down = false;
    s->crown_delta = 0;
    memset(s->knob_regs, 0, sizeof(s->knob_regs));
    acts_pmu_irq(s);
    if (s->input_path) {
        timer_mod(s->input_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 6000000);
    }
    memset(s->i2c_tx_len, 0, sizeof(s->i2c_tx_len));
    if (s->tap_ms) timer_mod(s->tap_timer,
                           qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + (int64_t)s->tap_ms * 1000000);
    timer_del(s->unplug_timer);
    if (s->unplug_after_boot) {
        timer_mod(s->unplug_timer, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + 900000000);
    }
    /* App assumes ROM enabled COREPLL; 48*8 MHz supplies SDK 384/256/128
     * divider rates. Synthetic handoff clock, not a captured reset value. */
    s->regs[0x120/4]=0x80|48;
    s->regs[0xc01c/4]=(26u<<16)|(11u<<8)|8; /* deterministic RTC date */
    /* SDK soc_boot.h boot_info: synthetic app handoff, not bootloader execution. */
    uint8_t *info = memory_region_get_ram_ptr(&s->sram);
    memset(info, 0, 64);
    stl_le_p(info + 4, 0x11800000); /* param_save_addr */
    stl_le_p(info + 8, 0x24000); /* system_phy_addr */
    stl_le_p(info + 36, 0x1000); /* param_phy_addr */
    stl_le_p(info + 16, 0x11400100); /* emulator supplied SDK NAND ID table */
    stl_le_p(info + 20, 790);
    for (unsigned i = 0; i < 6; i++) {
        ActsTimer *t = &s->timers[i];
        timer_del(t->timer);
        t->ctl = t->value = t->count = 0;
        t->started = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        qemu_set_irq(t->irq, 0);
    }
    /* This machine starts at the app handoff, not ROM reset. The SDK
     * bootloader starts TIMER2 and uses it in soc_boot.c immediately
     * before launching the app. Preserve that free-running cycle counter. */
    s->timers[2].ctl = 0x824;
    s->timers[2].value = UINT32_MAX;
    acts_timer_arm(&s->timers[2]);
}

static void acts_init(MachineState *machine)
{
    ActsState *s = g_new0(ActsState, 1);
    nand_init(&s->nand);
    s->exploratory = g_strcmp0(g_getenv("QEMU_WP2_EXPLORATORY"), "1") == 0;
    s->host_rtc = g_strcmp0(g_getenv("QEMU_WP2_HOST_RTC"), "1") == 0;
    s->stock_awake = g_strcmp0(g_getenv("QEMU_WP2_STOCK_AWAKE"), "1") == 0;
    s->input_path = g_getenv("QEMU_WP2_INPUT");
    s->viewer_brightness = 100;
    s->stk_enabled = s->input_path && g_getenv("QEMU_WP2_LIVE_SENSORS");
    s->health_hook = s->input_path && g_getenv("QEMU_WP2_LIVE_HEALTH");
    s->frame_path = g_getenv("QEMU_WP2_FRAME");
    if (s->frame_path && g_getenv("QEMU_WP2_LIVE_FRAME")) {
        s->frame_timer = timer_new_ns(QEMU_CLOCK_REALTIME, acts_panel_publish, s);
        for (int y = 0; y < 466; y++) {
            int dy = 2 * y - 465;
            int x = 0;
            while ((2 * x - 465) * (2 * x - 465) + dy * dy > 466 * 466) {
                x++;
            }
            s->panel_inset[y] = x;
        }
    }
    s->input_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_input_tick, s);
    s->unplug_after_boot = g_strcmp0(g_getenv("QEMU_WP2_UNPLUGGED"), "1") == 0;
    s->unplug_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_unplug, s);
    s->tap_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_touch_tick, s);
    const char *tap = g_getenv("QEMU_WP2_TAP");
    char extra;
    if (tap && (sscanf(tap, "%u,%u,%u%c", &s->tap_x, &s->tap_y, &s->tap_ms, &extra) != 3 ||
                s->tap_x >= 466 || s->tap_y >= 466 || !s->tap_ms)) {
        error_report("QEMU_WP2_TAP must be x,y,virtual-ms on the 466px panel");
        exit(1);
    }
    MemoryRegion *mem = get_system_memory();
    Clock *clock = clock_new(OBJECT(machine), "cpuclk");
    clock_set_hz(clock, 96000000);
    acts_ram(machine, &s->rom, "acts.bootrom", 0, 0x10000, true);
    acts_ram(machine, &s->nor, "acts.hle.nor", 0x11000000, 0x401000, false);
    /* The DE bus master addresses physical NOR through the unmap/uncached
     * aperture. Offset zero is flash offset zero, not the app partition. */
    memory_region_init_alias(&s->flash_uncached, OBJECT(machine), "acts.xip.uncached",
                             &s->nor, 0, 0x400000);
    memory_region_set_readonly(&s->flash_uncached, true);
    memory_region_add_subregion(mem, 0x14000000, &s->flash_uncached);
    for (unsigned i=0;i<8;i++) {
        g_autofree char *name=g_strdup_printf("acts.map%u",i);
        memory_region_init_alias(&s->maps[i],OBJECT(machine),name,&s->nor,0,0x400000);
        memory_region_set_readonly(&s->maps[i],true);
    }
    memset(memory_region_get_ram_ptr(&s->nor), 0xff, 0x400000);
    const char *nor_path = g_getenv("QEMU_WP2_NOR");
    if (nor_path) {
        g_autofree gchar *data=NULL;
        gsize len;
        if(!g_file_get_contents(nor_path,&data,&len,NULL) || len != 0x400000) {
            error_report("Reconstructed NOR must be exactly 4 MiB");
            exit(1);
        }
        memcpy(memory_region_get_ram_ptr(&s->nor),data,len);
        g_autofree char *dir=g_path_get_dirname(nor_path);
        g_autofree char *ids=g_build_filename(dir,"nand-id.bin",NULL);
        g_autofree char *id_data=NULL;
        gsize id_len;
        if(!g_file_get_contents(ids,&id_data,&id_len,NULL) || id_len!=790) {
            error_report("Missing SDK NAND ID table (run prepare_storage.py)");exit(1);
        }
        memcpy((uint8_t *)memory_region_get_ram_ptr(&s->nor)+0x400100,id_data,id_len);
    }
    if (machine->kernel_filename) {
        /* Keep the explicit app override used by the legacy live viewer. */
        acts_ram(machine, &s->flash, "acts.xip", 0x10000000, 0x1000000, true);
        memcpy(memory_region_get_ram_ptr(&s->flash),
               (uint8_t *)memory_region_get_ram_ptr(&s->nor) + 0x24000, 0x3dc000);
    } else {
        if (!nor_path || !machine->firmware || s->health_hook) {
            error_report("NOR application handoff needs a NOR image and ROM with the health hook disabled");
            exit(1);
        }
        /* Use one backing for NOR programming and instruction fetch. The
         * CPU still starts at the application vector, after the bootloader. */
        memory_region_init_alias(&s->flash, OBJECT(machine), "acts.xip",
                                 &s->nor, 0x24000, 0x3dc000);
        memory_region_set_readonly(&s->flash, true);
        memory_region_add_subregion(mem, 0x10000000, &s->flash);
    }
    memory_region_init_alias(&s->param, OBJECT(machine), "acts.param.xip",
                             &s->nor, 0x1000, 0x1000);
    memory_region_set_readonly(&s->param, true);
    memory_region_add_subregion(mem, 0x11800000, &s->param);
    acts_ram(machine, &s->sram, "acts.sram", 0x2ff60000, 0xe4000, false);
    acts_ram(machine, &s->jpeg_ram, "acts.jpeg.tables.fifo", 0x2ff17800, 0x800, false);
    acts_ram(machine, &s->shared, "acts.shared", 0x2ff18000, 0x8000, false);
    acts_ram(machine, &s->bt_ram, "acts.bt.ram", 0x2ff20000, 0x10000, false);
    acts_ram(machine, &s->trace_ram, "acts.trace.ram",0x31000000,1024,false);
    const char *nand_path = g_getenv("QEMU_WP2_NAND");
    if (nand_path) {
        g_autofree char *data = NULL;
        gsize length = 0;
        if (!g_file_get_contents(nand_path, &data, &length, NULL) ||
            length != (size_t)NAND_PAGE * NAND_PAGES) {
            error_report("Invalid physical NAND image: %s", nand_path);
            exit(1);
        }
        memcpy(s->nand.data, data, length);
    }
    s->nand_export = g_getenv("QEMU_WP2_NAND_EXPORT");
    s->ota_nand_snapshot = g_getenv("QEMU_WP2_OTA_NAND_SNAPSHOT");
    if (s->nand_export) {
        acts_ram(machine, &s->provisioning, "acts.nand.provisioning",
                 0x20000000, 0x06000000, false);
        const char *paths[] = {
            g_getenv("QEMU_WP2_NAND_PROGRAM"),
            g_getenv("QEMU_WP2_NAND_STAGING"),
        };
        uint8_t *ram = memory_region_get_ram_ptr(&s->provisioning);
        for (unsigned i = 0; i < 2; i++) {
            g_autofree char *data = NULL;
            gsize length = 0;
            unsigned offset = i ? 0x10000 : 0;
            unsigned limit = i ? 0x06000000 - offset : 0x1000;
            if (!paths[i] || !g_file_get_contents(paths[i], &data, &length, NULL) ||
                !length || length > limit) {
                error_report("Invalid NAND provisioning input %u", i);
                exit(1);
            }
            memcpy(ram + offset, data, length);
        }
    }
    acts_ram(machine, &s->psram, "acts.psram", 0x38000000, 0x800000, false);
    memory_region_init_alias(&s->psram_alias, OBJECT(machine), "acts.psram.alias",
                             &s->psram, 0, 0x800000);
    memory_region_add_subregion(mem, 0x18000000, &s->psram_alias);
    /* SDK SPI1_CACHE_WT_WNA_ADDR: the same PSRAM, write-through alias. */
    memory_region_init_alias(&s->psram_wt_alias,OBJECT(machine),"acts.psram.wt",
                             &s->psram,0,0x800000);
    memory_region_add_subregion(mem,0x34000000,&s->psram_wt_alias);
    memory_region_init_alias(&s->psram_uncached, OBJECT(machine), "acts.psram.uncached",
                             &s->psram, 0, 0x800000);
    memory_region_add_subregion(mem, 0x3c000000, &s->psram_uncached);
    memory_region_init_io(&s->mmio, OBJECT(machine), &acts_ops, s, "acts.mmio", 0x100000);
    memory_region_init_io(&s->usb_phy, OBJECT(machine),
                          &acts_usb_phy_ops, s, "acts.usb-phy", 3);
    memory_region_add_subregion_overlap(&s->mmio, 0x50420, &s->usb_phy, 1);
    memory_region_add_subregion(mem, 0x40000000, &s->mmio);

    s->cpu = qdev_new(TYPE_ARMV7M);
    object_property_add_child(OBJECT(machine), "armv7m", OBJECT(s->cpu));
    qdev_prop_set_string(s->cpu, "cpu-type", ARM_CPU_TYPE_NAME("cortex-m33"));
    qdev_prop_set_uint32(s->cpu, "num-irq", 64);
    qdev_prop_set_uint32(s->cpu, "init-svtor", 0x10000200);
    qdev_connect_clock_in(s->cpu, "cpuclk", clock);
    object_property_set_link(OBJECT(s->cpu), "memory", OBJECT(mem), &error_abort);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(s->cpu), &error_fatal);
    s->lcd_irq=qdev_get_gpio_in(s->cpu,43);
    s->bt_irq = qdev_get_gpio_in(s->cpu, 56);
    s->bt_rx_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_bt_rx_drain, s);
    Chardev *bt_chr = qemu_chr_find("wp2-hci");
    if (bt_chr) {
        s->bt_external = true;
        qemu_chr_fe_init(&s->bt_backend, bt_chr, &error_fatal);
        qemu_chr_fe_set_handlers(&s->bt_backend, acts_bt_can_read, acts_bt_read,
                                 NULL, NULL, s, NULL, true);
    }
    s->vg_irq = qdev_get_gpio_in(s->cpu, 59);
    s->sdma1_irq = qdev_get_gpio_in(s->cpu, 39);
    s->de_irq = qdev_get_gpio_in(s->cpu, 49);
    s->dac_irq = qdev_get_gpio_in(s->cpu, 40);
    s->dac_fifo_irq = qdev_get_gpio_in(s->cpu, 55);
    s->dac_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_dac_tick, s);
    const char *audiodev = g_getenv("QEMU_WP2_AUDIODEV");
    if (audiodev) {
        s->dac_card.state = audio_state_by_name(audiodev, &error_fatal);
        if (AUD_register_card("Watch Pro 2", &s->dac_card, &error_fatal)) {
            s->dac_host_pcm = g_byte_array_new();
        }
    }
    s->jpeg_irq = qdev_get_gpio_in(s->cpu, 57);
    s->gpio_irq = qdev_get_gpio_in(s->cpu, 37);
    s->gps_irq = qdev_get_gpio_in(s->cpu, 23);
    s->pmu_irq = qdev_get_gpio_in(s->cpu, 52);
    s->i2cmt_irq[0] = qdev_get_gpio_in(s->cpu, 62);
    s->i2cmt_irq[1] = qdev_get_gpio_in(s->cpu, 63);
    s->panel_te_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_panel_te_tick, s);
    s->adc_irq=qdev_get_gpio_in(s->cpu,51);
    s->adc_timer=timer_new_ns(QEMU_CLOCK_VIRTUAL,acts_adc_tick,s);
    s->wd_irq=qdev_get_gpio_in(s->cpu,1);
    s->wd_timer=timer_new_ns(QEMU_CLOCK_VIRTUAL,acts_wd_expire,s);
    s->panel_x0=8;s->panel_x1=473;s->panel_y1=465;
    for(unsigned i=0;i<10;i++) s->dma_irq[i]=qdev_get_gpio_in(s->cpu,27+i);
    for(unsigned i=0;i<4;i++) s->i2c_irq[i]=qdev_get_gpio_in(s->cpu,i<2?18+i:23+i);
    for (unsigned i = 0; i < 6; i++) {
        ActsTimer *t = &s->timers[i];
        t->soc = s;
        t->irq = qdev_get_gpio_in(s->cpu, i == 5 ? 10 : 2 + i);
        t->timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, acts_timer_expired, t);
    }
    for (unsigned i = 0; i < 3; i++) {
        qemu_chr_fe_init(&s->uart[i], serial_hd(i), &error_fatal);
    }
    qemu_register_reset(acts_reset, s);
    if (machine->firmware && load_image_targphys(machine->firmware, 0, 0x10000) < 0) {
        error_report("Cannot load supplied boot ROM / compatibility ROM");
        exit(1);
    }
    armv7m_load_kernel(ARM_CPU(first_cpu), machine->kernel_filename, 0x10000000, 0x1000000);
    if (s->health_hook) {
        const uint8_t prologue[] = {0x2d, 0xe9, 0xf0, 0x47, 0x2d, 0xed, 0x02, 0x8b};
        const uint8_t jump[] = {0xdf, 0xf8, 0x00, 0xf0, 0x01, 0x7f, 0x00, 0x00};
        uint8_t *entry = rom_ptr(0x10104b8c, sizeof(jump));
        const void *magic = rom_ptr(0x7efc, 4);
        if (!entry || memcmp(entry, prologue, sizeof(prologue)) ||
            !magic || ldl_le_p(magic) != 0x57503248) {
            error_report("Virtual health results require stock 1.0.0.73 and the compatibility ROM");
            exit(1);
        }
        memcpy(entry, jump, sizeof(jump));
    }
}

static void acts_machine_class(MachineClass *mc)
{
    mc->desc = "Experimental ATS3089C Watch Pro 2 (incomplete peripherals)";
    mc->init = acts_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-m33");
}
DEFINE_MACHINE("ats3089c-wp2", acts_machine_class)
