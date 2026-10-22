// 0x10225d9a FUN_100e0404 :: sys2ux update screenon_info val %d
// entry 0x100e0404  FUN_100e0404

undefined4 FUN_100e0404(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  
  uVar1 = (DAT_100e0430 - DAT_100e0434) * 0x20 & 0xff00;
  FUN_1014c818(uVar1 | 0xa00031,DAT_100e0438,param_1,uVar1,param_4);
  FUN_101d7326(1,0);
  return 0;
}


