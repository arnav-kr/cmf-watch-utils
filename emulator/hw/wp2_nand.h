/* SPDX-License-Identifier: GPL-2.0-or-later
 * Functional W25N01GV subset, ideal timing, volatile storage. See README.
 * Transactions follow Winbond W25N01GV Rev G instruction table (buffer mode).
 */
#define NAND_PAGE 2112u
#define NAND_PAGES 65536u
typedef struct Wp2Nand {
    uint8_t *data;
    uint8_t cache[NAND_PAGE], tx[NAND_PAGE+16], feature[256];
    unsigned txlen, rxpos, row, logs;
    uint32_t ctl, bc;
} Wp2Nand;

static void nand_init(Wp2Nand *n)
{
    n->data=g_malloc((size_t)NAND_PAGE*NAND_PAGES);
    memset(n->data,255,(size_t)NAND_PAGE*NAND_PAGES);
    memset(n->cache,255,sizeof(n->cache));
    n->feature[0xa0]=0; /* development medium, initially unprotected */
    n->feature[0xb0]=0x18; /* ECC and buffer mode */
    n->ctl=8;
}
static void nand_end(Wp2Nand *n)
{
    if(!n->txlen) return;
    unsigned cmd=n->tx[0], row=(n->tx[1]<<16)|(n->tx[2]<<8)|n->tx[3];
    unsigned col=(n->tx[1]<<8)|n->tx[2];
    if(n->logs++<128) qemu_log_mask(LOG_UNIMP,"NAND cmd=%02x bytes=%u addr=%06x rx=%u\n",cmd,n->txlen,row,n->rxpos);
    switch(cmd) {
    case 0xff: n->feature[0xc0]=0; break;
    case 0x06: n->feature[0xc0]|=2; break;
    case 0x04: n->feature[0xc0]&=~2; break;
    case 0x01: case 0x1f:
        if(n->txlen==3 && (n->tx[1]==0xa0 || n->tx[1]==0xb0)) n->feature[n->tx[1]]=n->tx[2];
        break;
    case 0x13:
        if(n->txlen==4 && row<NAND_PAGES) {
            n->row=row; memcpy(n->cache,n->data+(size_t)row*NAND_PAGE,NAND_PAGE);
        }
        break;
    case 0x02: case 0x32: case 0x84: case 0x34:
        if(cmd==2 || cmd==0x32) memset(n->cache,255,NAND_PAGE);
        if(n->txlen>=3 && col<NAND_PAGE)
            memcpy(n->cache+col,n->tx+3,MIN(n->txlen-3,NAND_PAGE-col));
        break;
    case 0x10:
        if(n->txlen==4 && row<NAND_PAGES && (n->feature[0xc0]&2)) {
            if(n->feature[0xa0]&0x7c) n->feature[0xc0]|=8;
            else for(unsigned i=0;i<NAND_PAGE;i++) n->data[(size_t)row*NAND_PAGE+i]&=n->cache[i];
            n->feature[0xc0]&=~2;
        }
        break;
    case 0xd8:
        if(n->txlen==4 && row<NAND_PAGES && (n->feature[0xc0]&2)) {
            if(n->feature[0xa0]&0x7c) n->feature[0xc0]|=4;
            else memset(n->data+(size_t)(row&~63u)*NAND_PAGE,255,64*NAND_PAGE);
            n->feature[0xc0]&=~2;
        }
        break;
    case 0x9f: case 0x0f: case 5: case 3: case 0xb: case 0x3b: case 0x6b: case 0xbb: case 0xeb:
        break;
    default:
        error_report("Unsupported NAND command %02x",cmd);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
    }
}
static uint8_t nand_rx(Wp2Nand *n)
{
    uint8_t value=255;
    switch(n->tx[0]) {
    case 0x9f: { const uint8_t id[]={0xef,0xaa,0x21,0}; value=id[n->rxpos%4]; break; }
    case 0x0f: case 5: value=n->feature[n->tx[1]];break;
    case 3: case 0xb: case 0x3b: case 0x6b: case 0xbb: case 0xeb: {
        unsigned col=(n->tx[1]<<8)|n->tx[2];
        if(col+n->rxpos<NAND_PAGE) value=n->cache[col+n->rxpos];
        break;
    }
    default: error_report("Unsupported NAND read command %02x",n->tx[0]);
        qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
    }
    n->rxpos++;
    if(n->bc) n->bc--;
    return value;
}
static uint32_t nand_reg_read(Wp2Nand *n,unsigned off)
{
    switch(off) {
    case 0:return n->ctl;
    case 4:return 0x10 | ((n->ctl&1) && n->bc ? 0:0x40);
    case 0xc: {
        unsigned count=n->ctl&(1u<<30)?4:1; uint32_t v=0;
        for(unsigned i=0;i<count;i++) v|=(uint32_t)nand_rx(n)<<(8*i);
        return v;
    }
    case 0x10:return n->bc;
    default:return 0;
    }
}
static void nand_reg_write(Wp2Nand *n,unsigned off,uint32_t value)
{
    switch(off) {
    case 0:
        if(!(n->ctl&8) && (value&8)) nand_end(n);
        if((n->ctl&8) && !(value&8)) {n->txlen=0;n->rxpos=0;memset(n->tx,0,sizeof(n->tx));}
        n->ctl=value;break;
    case 4:break;
    case 8: {
        unsigned count=n->ctl&(1u<<30)?4:1;
        for(unsigned i=0;i<count;i++) {
            if(n->txlen<sizeof(n->tx)) n->tx[n->txlen++]=value>>(i*8);
            if(n->bc) n->bc--;
        }
        break;
    }
    case 0x10:n->bc=value;break;
    }
}
