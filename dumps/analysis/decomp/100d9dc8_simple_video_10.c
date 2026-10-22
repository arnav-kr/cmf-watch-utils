// entry 0x100d9dc8  simple_video_10  (100d9dc8)

void simple_video_10(int param_1,byte param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_100d9e5c,0xf5,DAT_100d9e60,DAT_100d9e58,DAT_100d9e54,DAT_100d9e50);
    FUN_101ba194(DAT_100d9e68,DAT_100d9e64,DAT_100d9e5c,0xf5);
    FUN_101ba194(DAT_100d9e6c);
    FUN_100eeb30(DAT_100d9e5c,0xf5);
  }
  iVar1 = FUN_101c54c2(param_1,DAT_100d9e70);
  if (iVar1 == 0) {
    FUN_1013a098(3,DAT_100d9e5c,0xf5,DAT_100d9e60,DAT_100d9e58,DAT_100d9e78,DAT_100d9e74);
    FUN_101ba194(DAT_100d9e68,DAT_100d9e64,DAT_100d9e5c,0xf5);
    FUN_101ba194(DAT_100d9e6c);
    FUN_100eeb30(DAT_100d9e5c,0xf5);
  }
  *(byte *)(param_1 + 0x55) = *(byte *)(param_1 + 0x55) & 0xf7 | (param_2 & 1) << 3;
  return;
}


