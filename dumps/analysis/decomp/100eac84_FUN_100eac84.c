// entry 0x100eac84  FUN_100eac84

int FUN_100eac84(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = (DAT_100eacf0 - DAT_100eacf4) * 0x20 & 0xff00;
  FUN_1014c818(uVar3 | 0x18c0032,DAT_100eacf8,param_2);
  if (param_1 == 0) {
LAB_100eacae:
    iVar2 = 0;
  }
  else {
    iVar2 = FUN_101072f0(param_1,param_2,0);
    if (iVar2 == 0) {
      iVar2 = FUN_10107294(param_1,param_3,param_4);
      if (-1 < iVar2) goto LAB_100eacae;
      uVar3 = DAT_100ead00 | uVar3;
      uVar1 = DAT_100ead04;
    }
    else {
      uVar3 = uVar3 | 0x1900031;
      uVar1 = DAT_100eacfc;
    }
    FUN_1014c818(uVar3,uVar1,iVar2);
  }
  return iVar2;
}


