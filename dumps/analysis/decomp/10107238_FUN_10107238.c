// entry 0x10107238  FUN_10107238  (10107238)

int FUN_10107238(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  code *pcVar2;
  undefined4 uVar3;
  
  if (*(int *)(param_1 + 4) == 0) {
    iVar1 = -9;
  }
  else {
    pcVar2 = *(code **)(*(int *)(*(int *)(param_1 + 4) + 0x1c) + 4);
    if (pcVar2 == (code *)0x0) {
      iVar1 = -0x86;
    }
    else {
      uVar3 = param_3;
      iVar1 = (*pcVar2)();
      if (iVar1 < 0) {
        FUN_1014c818((DAT_1010728c - DAT_10107288) * 0x20 & 0xff00U | 0x1080013,DAT_10107290,iVar1,
                     param_3,param_1 + 9,param_2,uVar3);
        FUN_101bbdaa();
      }
    }
  }
  return iVar1;
}


