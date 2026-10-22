// entry 0x101848d4  FUN_101848d4  (101848d4)

undefined4 FUN_101848d4(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  short sVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  
  if ((param_1 == 0) || (param_2 == 0)) {
    FUN_1014c818(DAT_10184990 | (DAT_1018498c - DAT_10184988) * 0x20 & 0xff00U,DAT_10184994,param_1,
                 param_2,param_3);
LAB_10184906:
    uVar2 = 0xffffffff;
  }
  else {
    if (param_3 != 0) {
      for (iVar4 = param_3; iVar4 != param_5 * 0xc + param_3; iVar4 = iVar4 + 0xc) {
        *(undefined4 *)(iVar4 + 8) = 0;
      }
    }
    for (iVar4 = 0; iVar4 != param_5; iVar4 = iVar4 + 1) {
      piVar3 = (int *)FUN_10187d90(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 4),
                                   *(undefined4 *)(param_2 + iVar4 * 4));
      if (piVar3 == (int *)0x0) {
        if (param_3 != 0) {
          FUN_101d775a(param_3,param_5);
        }
        goto LAB_10184906;
      }
      if (param_3 != 0) {
        FUN_101d7666(iVar4 * 0xc + param_3);
      }
      if (param_4 != 0) {
        sVar1 = *(short *)(*piVar3 + 0x12);
        *(int *)(param_4 + iVar4 * 8) = (int)*(short *)(*piVar3 + 0x10);
        *(int *)(param_4 + 4 + iVar4 * 8) = (int)sVar1;
      }
      FUN_101d78a6(piVar3);
    }
    uVar2 = 0;
  }
  return uVar2;
}


