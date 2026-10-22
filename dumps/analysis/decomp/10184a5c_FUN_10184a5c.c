// entry 0x10184a5c  FUN_10184a5c

undefined4 FUN_10184a5c(int param_1,int param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int extraout_r3;
  int iVar4;
  int iVar5;
  
  if (((param_1 == 0) || (param_2 == 0)) || (param_3 == 0)) {
    FUN_1014c818(DAT_10184adc | (DAT_10184ad8 - DAT_10184ad4) * 0x20 & 0xff00U,DAT_10184ae0,param_1,
                 param_2,param_3);
LAB_10184a8a:
    uVar1 = 0xffffffff;
  }
  else {
    iVar3 = 0;
    for (iVar2 = iVar3; param_4 != iVar2; iVar2 = iVar2 + 1) {
      *(undefined4 *)(param_3 + iVar2 * 8) = 0;
    }
    iVar2 = param_1;
    iVar5 = param_2;
    for (iVar4 = 0; param_4 != iVar4; iVar4 = iVar4 + 1) {
      iVar3 = FUN_10187d90(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 4),
                           *(undefined4 *)(param_2 + iVar4 * 4),iVar3,iVar2,iVar5);
      if (iVar3 == 0) {
        FUN_101d777a(param_3,iVar4);
        goto LAB_10184a8a;
      }
      *(undefined4 *)(param_3 + iVar4 * 8) = *(undefined4 *)(iVar3 + 4);
      FUN_101d78a6();
      iVar3 = extraout_r3;
    }
    uVar1 = 0;
  }
  return uVar1;
}


