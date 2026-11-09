/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Actions shared-memory controller transport. The local controller has no
 * radio backend. Ring layout follows the SDK's rbuf_core.h and rbuf_mem.h. */
typedef struct {
    unsigned rx, tx;
    bool ready;
    uint8_t address[6];
    uint8_t adv_params[15], adv_data[32], scan_response[32];
    bool advertising;
} Wp2Bt;

typedef struct {
    uint8_t *meta, *data;
    unsigned head, tail, size, hlen;
} Wp2Ring;

static bool wp2_ring_open(Wp2Ring *r, uint8_t *ram, unsigned id)
{
    if (id < 32 || id > 0x8000 - 16 || (id & 3)) {
        return false;
    }
    uint8_t *m = ram + id;
    unsigned data = lduw_le_p(m + 8);
    *r = (Wp2Ring) {
        .meta = m, .head = lduw_le_p(m), .tail = lduw_le_p(m + 4),
        .size = lduw_le_p(m + 10), .hlen = lduw_le_p(m + 12),
    };
    if (data < id + 16 || data + r->size > 0x8000 ||
        r->head >= r->size || r->tail >= r->size ||
        (r->hlen != 0 && r->hlen != 4)) {
        return false;
    }
    r->data = ram + data;
    return true;
}

static unsigned wp2_ring_used(const Wp2Ring *r)
{
    return (r->tail + r->size - r->head) % r->size;
}

static void wp2_ring_peek(const Wp2Ring *r, unsigned skip, uint8_t *data,
                          unsigned length)
{
    for (unsigned i = 0; i < length; i++) {
        data[i] = r->data[(r->head + skip + i) % r->size];
    }
}

static void wp2_ring_consume(Wp2Ring *r, unsigned length)
{
    r->head = (r->head + length) % r->size;
    stw_le_p(r->meta, r->head);
    stw_le_p(r->meta + 2, r->head);
}

static bool wp2_ring_put(Wp2Ring *r, const uint8_t *data, unsigned length)
{
    if (length >= r->size - wp2_ring_used(r)) {
        return false;
    }
    for (unsigned i = 0; i < length; i++) {
        r->data[(r->tail + i) % r->size] = data[i];
    }
    r->tail = (r->tail + length) % r->size;
    stw_le_p(r->meta + 4, r->tail);
    stw_le_p(r->meta + 6, r->tail);
    return true;
}

static bool wp2_bt_boot(Wp2Bt *bt, uint8_t *ram)
{
    Wp2Ring host, controller, rx, tx;
    if (!wp2_ring_open(&host, ram, lduw_le_p(ram + 4)) || host.hlen != 4 ||
        !wp2_ring_open(&controller, ram, lduw_le_p(ram + 16)) || controller.hlen != 4 ||
        controller.head != controller.tail || controller.tail != 0) {
        return false;
    }
    for (unsigned offset = 0; offset + 24 <= wp2_ring_used(&host); offset += 24) {
        uint8_t msg[24];
        wp2_ring_peek(&host, offset, msg, sizeof(msg));
        if (ldl_le_p(msg) != 20) {
            return false;
        }
        if (lduw_le_p(msg + 4) != 0x402) { /* MSG_BT_HCI_BUF */
            continue;
        }
        bt->rx = ldl_le_p(msg + 8);
        bt->tx = ldl_le_p(msg + 12);
    }
    if (!wp2_ring_open(&rx, ram, bt->rx) || rx.hlen ||
        !wp2_ring_open(&tx, ram, bt->tx) || tx.hlen) {
        return false;
    }
    const uint8_t reply[24] = {20, 0, 0, 0, 3, 4}; /* MSG_BT_HCI_OK */
    if (!wp2_ring_put(&controller, reply, sizeof(reply))) {
        return false;
    }
    wp2_ring_consume(&host, wp2_ring_used(&host));
    bt->ready = true;
    return true;
}

/* HCI Command Complete return parameters, beginning with status. */
static unsigned wp2_bt_command(Wp2Bt *bt, unsigned opcode, const uint8_t *params,
                                unsigned length, uint8_t *result)
{
    qemu_log_mask(LOG_UNIMP, "WP2 local HCI command %04x, params=%u\n", opcode, length);
    switch (opcode) {
    case 0x0c03: /* Reset */
        result[0] = length ? 0x12 : 0;
        if (!length) {
            bt->advertising = false;
        }
        return 1;
    case 0x0c01: case 0x2001: /* Event masks */
        result[0] = length == 8 ? 0 : 0x12;
        return 1;
    case 0x0c56: case 0x0c45: case 0x0c2f: /* SSP, inquiry and SCO flow control */
        result[0] = length == 1 ? 0 : 0x12;
        return 1;
    case 0x0c13: /* Write Local Name */
        result[0] = length == 248 ? 0 : 0x12;
        return 1;
    case 0x0c52: /* Extended Inquiry Response */
        result[0] = length == 241 ? 0 : 0x12;
        return 1;
    case 0x080f: case 0x0c18: case 0x0c6d: /* Link policy, page timeout, LE host support */
        result[0] = length == 2 ? 0 : 0x12;
        return 1;
    case 0x0c24: /* Class of device */
        result[0] = length == 3 ? 0 : 0x12;
        return 1;
    case 0x1001: /* Read Local Version Information */
        result[1] = result[4] = 9; /* Core 5.0 */
        result[5] = 0xff;
        result[6] = 0xff;
        return 9;
    case 0x1002: /* Read Local Supported Commands */
        result[1 + 5] = 0xc0; /* event mask and reset */
        result[1 + 14] = 0x3c; /* local information queries */
        result[1 + 15] = 0x02; /* read BD_ADDR */
        result[1 + 25] = 0x17; /* LE masks, buffers, features, random address */
        result[1 + 27] = 0x80; /* LE Rand */
        return 65;
    case 0x1003: /* Read Local Supported Features */
        result[5] = 0x40; /* dual-mode host interface */
        return 9;
    case 0x1005: /* Read Buffer Size */
        stw_le_p(result + 1, 251);
        stw_le_p(result + 4, 4);
        return 8;
    case 0x1009: /* Read BD_ADDR: local emulator identity */
        memcpy(result + 1, bt->address, 6);
        return 7;
    case 0x2002: /* LE Read Buffer Size */
        stw_le_p(result + 1, 251);
        result[3] = 4;
        return 4;
    case 0x2003: return 9; /* LE Read Local Supported Features */
    case 0x2005: return 1; /* LE Set Random Address (radio disabled) */
    case 0x2006:
        if (length == sizeof(bt->adv_params)) {
            memcpy(bt->adv_params, params, length);
        } else {
            result[0] = 0x12;
        }
        return 1;
    case 0x2007: return 2; /* LE Read Advertising Channel Tx Power: 0 dBm */
    case 0x2008: case 0x2009:
        if (length == 32 && params[0] <= 31) {
            memcpy(opcode == 0x2008 ? bt->adv_data : bt->scan_response, params, length);
        } else {
            result[0] = 0x12;
        }
        return 1;
    case 0x200a:
        if (length == 1 && params[0] <= 1) {
            bt->advertising = params[0];
        } else {
            result[0] = 0x12;
        }
        return 1;
    case 0x200f: return 2; /* LE Read Filter Accept List Size: zero */
    case 0x201c: return 9; /* LE Read Supported States: no radio operations */
    case 0x2018: /* LE Rand */
        qemu_guest_getrandom_nofail(result + 1, 8);
        return 9;
    default:
        result[0] = 1; /* Unknown HCI Command */
        return 1;
    }
}

static bool wp2_bt_service(Wp2Bt *bt, uint8_t *ram)
{
    Wp2Ring rx, tx;
    if (!bt->ready || !wp2_ring_open(&rx, ram, bt->rx) ||
        !wp2_ring_open(&tx, ram, bt->tx)) {
        return false;
    }
    bool sent = false;
    while (wp2_ring_used(&tx) >= 8) {
        uint8_t header[8], params[255], reply[76] = {0};
        wp2_ring_peek(&tx, 0, header, sizeof(header));
        unsigned padded = lduw_le_p(header);
        if (header[2] != 1 || padded < 4 || padded > 260 ||
            (padded & 3) || padded < header[6] + 3 ||
            padded + 4 > wp2_ring_used(&tx)) {
            break;
        }
        unsigned opcode = lduw_le_p(header + 4);
        wp2_ring_peek(&tx, 7, params, header[6]);
        unsigned length = wp2_bt_command(bt, opcode, params, header[6], reply + 9);
        unsigned event_size = (length + 5 + 3) & ~3u;
        stw_le_p(reply, event_size);
        reply[2] = 4; /* HCI Event packet */
        reply[4] = 0x0e; /* Command Complete */
        reply[5] = length + 3;
        reply[6] = 1;
        stw_le_p(reply + 7, opcode);
        if (!wp2_ring_put(&rx, reply, event_size + 4)) {
            break;
        }
        wp2_ring_consume(&tx, padded + 4);
        sent = true;
    }
    return sent;
}

/* H4 lengths include the packet type. Zero means an incomplete header. */
static int wp2_h4_size(const uint8_t *data, unsigned count)
{
    if (!count) return 0;
    switch (data[0]) {
    case 1: case 3: return count >= 4 ? 4 + data[3] : 0;
    case 2: return count >= 5 ? 5 + lduw_le_p(data + 3) : 0;
    case 4: return count >= 3 ? 3 + data[2] : 0;
    case 5: return count >= 5 ? 5 + (lduw_le_p(data + 3) & 0x3fff) : 0;
    default: return -1;
    }
}

static bool wp2_bt_send_h4(Wp2Bt *bt, uint8_t *ram, CharBackend *backend)
{
    Wp2Ring tx;
    if (!bt->ready || !wp2_ring_open(&tx, ram, bt->tx)) return true;
    while (wp2_ring_used(&tx) >= 8) {
        uint8_t header[4], packet[4096];
        wp2_ring_peek(&tx, 0, header, sizeof(header));
        unsigned padded = lduw_le_p(header);
        if (padded < 4 || padded >= sizeof(packet) || (padded & 3) ||
            padded + 4 > wp2_ring_used(&tx) || header[2] == 4) return false;
        packet[0] = header[2];
        wp2_ring_peek(&tx, 4, packet + 1, padded);
        int length = wp2_h4_size(packet, padded + 1);
        if (length <= 0 || ((length + 2) & ~3u) != padded) return false;
        if (qemu_chr_fe_write_all(backend, packet, length) != length) return false;
        wp2_ring_consume(&tx, padded + 4);
    }
    return true;
}

static bool wp2_bt_receive_h4(Wp2Bt *bt, uint8_t *ram, const uint8_t *packet,
                              unsigned length)
{
    Wp2Ring rx;
    uint8_t frame[4100] = {0};
    unsigned padded = (length + 2) & ~3u;
    if (!bt->ready || !wp2_ring_open(&rx, ram, bt->rx) ||
        padded + 4 > sizeof(frame)) return false;
    stw_le_p(frame, padded);
    frame[2] = packet[0];
    memcpy(frame + 4, packet + 1, length - 1);
    return wp2_ring_put(&rx, frame, padded + 4);
}
