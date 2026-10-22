// entry 0x100e7300  FUN_100e7300

void FUN_100e7300(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  uVar1 = (DAT_100e7338 - DAT_100e7334) * 0x20 & 0xff00;
  uStack_14 = param_2;
  uStack_10 = param_3;
  uStack_c = param_4;
  FUN_1014c818(DAT_100e733c | uVar1,DAT_100e7344,DAT_100e7340,uVar1,param_1);
  FUN_100c14b4(&uStack_14);
  FUN_101b9f08(DAT_100e734c,DAT_100e7348,10000);
  return;
}


