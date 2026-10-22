// entry 0x100bfab8  FUN_100bfab8  (100bfab8)

void FUN_100bfab8(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  if (*(int *)(param_1 + 0x14) == 0) {
    return;
  }
  if (param_2 != 0) {
    lv_event_10(*(undefined4 *)(*(int *)(param_1 + 0x14) + 4),0xb,0,param_4);
    return;
  }
  FUN_10182238(0x1e);
  FUN_1014c818(DAT_100bfaf8 | (DAT_100bfaf0 - DAT_100bfaf4) * 0x20 & 0xff00U,DAT_100bfafc,
               DAT_100bfaf4,param_4);
  return;
}


