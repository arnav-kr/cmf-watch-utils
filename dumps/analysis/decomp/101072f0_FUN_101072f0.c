// entry 0x101072f0  FUN_101072f0  (101072f0)

int FUN_101072f0(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  code *pcVar2;
  
  if (*(int *)(param_1 + 4) == 0) {
    iVar1 = -9;
  }
  else {
    pcVar2 = *(code **)(*(int *)(*(int *)(param_1 + 4) + 0x1c) + 0xc);
    if (pcVar2 == (code *)0x0) {
      iVar1 = -0x86;
    }
    else {
      iVar1 = (*pcVar2)();
      if (iVar1 < 0) {
        FUN_1014c818(DAT_10107348 | (DAT_10107344 - DAT_10107340) * 0x20 & 0xff00U,DAT_1010734c,
                     iVar1,param_2,param_3,param_1 + 9);
        FUN_101bbdaa();
      }
    }
  }
  return iVar1;
}


