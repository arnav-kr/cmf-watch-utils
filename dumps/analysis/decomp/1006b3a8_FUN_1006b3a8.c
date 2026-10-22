// entry 0x1006b3a8  FUN_1006b3a8

void FUN_1006b3a8(void)

{
  uint uVar1;
  
  uVar1 = (DAT_1006b3e8 - DAT_1006b3e4) * 0x20 & 0xff00;
  FUN_1014c818(uVar1 | 0x7a0031,DAT_1006b3f0,DAT_1006b3ec);
  thunk_FUN_100d5e34(0x114,0);
  FUN_1014c818(uVar1 | 0x7e0031,DAT_1006b3f4,DAT_1006b3ec);
  return;
}


