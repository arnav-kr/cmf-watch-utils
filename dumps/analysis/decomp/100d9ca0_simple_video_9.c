// entry 0x100d9ca0  simple_video_9  (100d9ca0)

void simple_video_9(int param_1,uint param_2)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_100d9da4,0xd5,DAT_100d9da8,DAT_100d9da0,DAT_100d9d9c,DAT_100d9d98);
    FUN_101ba194(DAT_100d9db0,DAT_100d9dac,DAT_100d9da4,0xd5);
    FUN_101ba194(DAT_100d9db4);
    FUN_100eeb30(DAT_100d9da4,0xd5);
  }
  iVar2 = FUN_101c54c2(param_1,DAT_100d9db8);
  if (iVar2 == 0) {
    FUN_1013a098(3,DAT_100d9da4,0xd5,DAT_100d9da8,DAT_100d9da0,DAT_100d9dc0,DAT_100d9dbc);
    FUN_101ba194(DAT_100d9db0,DAT_100d9dac,DAT_100d9da4,0xd5);
    FUN_101ba194(DAT_100d9db4);
    FUN_100eeb30(DAT_100d9da4,0xd5);
  }
  bVar1 = *(byte *)(param_1 + 0x55);
  if (((int)((uint)bVar1 << 0x1e) < 0) && ((bVar1 & 1) != param_2)) {
    *(byte *)(param_1 + 0x55) = bVar1 & 0xfe | (byte)param_2 & 1;
    if (param_2 == 0) {
      if (*(int *)(param_1 + 0x58) != 0) {
        FUN_101cc7a6();
        return;
      }
    }
    else {
      if (*(short *)(param_1 + 200) == 0) {
        iVar2 = simple_video_6(param_1);
        *(short *)(param_1 + 200) = (short)iVar2;
        if (iVar2 == 0) {
          *(byte *)(param_1 + 0x55) = *(byte *)(param_1 + 0x55) & 0xfe;
          return;
        }
      }
      if (*(int *)(param_1 + 0x58) != 0) {
        FUN_101cc7b0();
        return;
      }
      if (*(char *)(param_1 + 0x54) == '\0') {
        *(undefined1 *)(param_1 + 0x54) = 0x19;
      }
      uVar3 = lv_timer_2(DAT_100d9dc4,1000 / *(byte *)(param_1 + 0x54),param_1);
      *(undefined4 *)(param_1 + 0x58) = uVar3;
    }
  }
  return;
}


