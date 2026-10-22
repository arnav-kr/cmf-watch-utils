// entry 0x10155cb4  FUN_10155cb4

void FUN_10155cb4(int param_1,int param_2)

{
  int *piVar1;
  short sVar2;
  short extraout_r2;
  short sVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  piVar1 = DAT_10155d6c;
  uVar5 = (DAT_10155d64 - DAT_10155d60) * 0x20 & 0xff00;
  FUN_1014c818(DAT_10155d68 | uVar5,DAT_10155d70,param_1,param_2);
  iVar6 = *piVar1;
  if (iVar6 == 0) {
    return;
  }
  iVar4 = *(int *)(iVar6 + 8);
  *DAT_10155d74 = (char)param_1;
  if (iVar4 == 0) {
    return;
  }
  FUN_1014c818(DAT_10155d78 | uVar5,DAT_10155d7c);
  if (param_1 == 0) {
LAB_10155d2a:
    iVar4 = *piVar1;
    *(undefined2 *)(iVar4 + 0x2e) = 0x100;
    if (param_1 == 0) {
      *(undefined1 *)(iVar4 + 0x48) = 0;
      goto LAB_10155d3a;
    }
  }
  else {
    if (param_2 == 1) {
      sVar3 = 0x28a;
      iVar4 = *piVar1;
    }
    else {
      if (param_2 != 2) goto LAB_10155d2a;
      sVar2 = FUN_1015fc0c();
      sVar3 = extraout_r2;
      if (0x82 < (ushort)(sVar2 - 100U)) {
        sVar3 = 0xe6;
      }
      iVar4 = *piVar1;
      if ((ushort)(sVar2 - 100U) < 0x83) {
        sVar3 = sVar2 + -0xf;
      }
    }
    *(short *)(iVar4 + 0x2e) = sVar3;
  }
  *(char *)(*piVar1 + 0x48) = (char)param_2;
LAB_10155d3a:
  iVar4 = FUN_10155c38();
  if (iVar4 == 0) {
    return;
  }
  FUN_1014c818(DAT_10155d80 | uVar5,DAT_10155d7c,*(undefined4 *)(iVar6 + 8));
                    /* WARNING: Could not recover jumptable at 0x10155d58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(iVar6 + 8))(iVar6,1,param_1);
  return;
}


