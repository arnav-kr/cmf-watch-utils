// 0x102039ad FUN_1005e728 :: ***  JX APP Firmware Version  ***
// entry 0x1005e728  FUN_1005e728

void FUN_1005e728(void)

{
  uint uVar1;
  
  uVar1 = (DAT_1005e76c - DAT_1005e768) * 0x20 & 0xff00;
  FUN_1014c818(uVar1 | 0x490031,DAT_1005e770);
  FUN_1014c818(uVar1 | 0x4a0032,DAT_1005e774,DAT_1005e77c,DAT_1005e778);
  FUN_1014c818(uVar1 | 0x4b0031,DAT_1005e780);
  return;
}


