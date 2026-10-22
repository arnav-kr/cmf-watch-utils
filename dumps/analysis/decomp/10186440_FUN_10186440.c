// entry 0x10186440  FUN_10186440  (10186440)

int FUN_10186440(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  uVar2 = (DAT_101864a4 - DAT_101864a0) * 0x20 & 0xff00;
  if (param_1 == 0) {
    FUN_1014c818(DAT_101864a8 | uVar2,DAT_101864ac,param_2,DAT_101864a0,0,param_2);
  }
  else {
    iVar3 = *(int *)(param_1 + 0x88);
    iVar4 = param_1;
    FUN_1014c818(DAT_101864b0 | uVar2,DAT_101864b4,param_2,*(undefined4 *)(param_1 + 0x84),param_1);
    for (iVar1 = 0; *(int *)(param_1 + 0x84) != iVar1; iVar1 = iVar1 + 1) {
      if (*(int *)(iVar3 + 0x18) == param_2) {
        return iVar3;
      }
      iVar3 = iVar3 + 0x24;
    }
    FUN_1014c818(DAT_101864b8 | uVar2,DAT_101864bc,param_2,iVar3,iVar4);
  }
  return 0;
}


