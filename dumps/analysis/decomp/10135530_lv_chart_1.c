// 0x1023ef4d lv_chart_1 :: lv_chart_set_update_mode
// entry 0x10135530  lv_chart_1

void lv_chart_1(int param_1,uint param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_101355d8,0xa2,DAT_101355dc,DAT_101355d4,DAT_101355d0,DAT_101355cc);
    FUN_101ba194(DAT_101355e4,DAT_101355e0,DAT_101355d8,0xa2);
    FUN_101ba194(DAT_101355e8);
    FUN_100eeb30(DAT_101355d8,0xa2);
  }
  iVar1 = FUN_101c54c2(param_1,DAT_101355ec);
  if (iVar1 == 0) {
    FUN_1013a098(3,DAT_101355d8,0xa2,DAT_101355dc,DAT_101355d4,DAT_101355f4,DAT_101355f0);
    FUN_101ba194(DAT_101355e4,DAT_101355e0,DAT_101355d8,0xa2);
    FUN_101ba194(DAT_101355e8);
    FUN_100eeb30(DAT_101355d8,0xa2);
  }
  if ((*(byte *)(param_1 + 0xb2) & 0xf) >> 3 != param_2) {
    *(byte *)(param_1 + 0xb2) = *(byte *)(param_1 + 0xb2) & 0xf7 | (byte)((param_2 & 1) << 3);
    lv_obj_pos_18(param_1);
    return;
  }
  return;
}


