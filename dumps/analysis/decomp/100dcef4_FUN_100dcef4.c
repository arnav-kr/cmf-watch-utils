// entry 0x100dcef4  FUN_100dcef4

undefined4 FUN_100dcef4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  
  uVar1 = (DAT_100dcf2c - DAT_100dcf28) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100dcf30 | uVar1,DAT_100dcf38,DAT_100dcf34,DAT_100dcf28,param_4);
  FUN_100dbb00(param_1);
  FUN_1014c818(DAT_100dcf3c | uVar1,DAT_100dcf40,DAT_100dcf34);
  return 0;
}


