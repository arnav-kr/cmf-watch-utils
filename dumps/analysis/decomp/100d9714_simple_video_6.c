// entry 0x100d9714  simple_video_6  (100d9714)

undefined2 simple_video_6(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  undefined4 uStack_c;
  
  uStack_c = param_4;
  FUN_101072f0(param_1 + 0x9c,8,0,param_4,param_1,param_2,param_3);
  uVar1 = FUN_10107238(param_1 + 0x9c,(int)&uStack_c + 2,2);
  if (uVar1 < 2) {
    FUN_1013a098(3,DAT_100d975c,0x118,DAT_100d9758,DAT_100d9754);
    uStack_c._2_2_ = 0;
  }
  else {
  }
  return uStack_c._2_2_;
}


