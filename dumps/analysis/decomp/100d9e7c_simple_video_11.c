// entry 0x100d9e7c  simple_video_11  (100d9e7c)

byte simple_video_11(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_100d9f14,0x105,DAT_100d9f18,DAT_100d9f10,DAT_100d9f0c,DAT_100d9f08,param_4);
    FUN_101ba194(DAT_100d9f20,DAT_100d9f1c,DAT_100d9f14,0x105);
    FUN_101ba194(DAT_100d9f24);
    FUN_100eeb30(DAT_100d9f14,0x105);
  }
  iVar1 = FUN_101c54c2(param_1,DAT_100d9f28);
  if (iVar1 == 0) {
    FUN_1013a098(3,DAT_100d9f14,0x105,DAT_100d9f18,DAT_100d9f10,DAT_100d9f30,DAT_100d9f2c,param_4);
    FUN_101ba194(DAT_100d9f20,DAT_100d9f1c,DAT_100d9f14,0x105);
    FUN_101ba194(DAT_100d9f24);
    FUN_100eeb30(DAT_100d9f14,0x105);
  }
  return *(byte *)(param_1 + 0x55) & 1;
}


