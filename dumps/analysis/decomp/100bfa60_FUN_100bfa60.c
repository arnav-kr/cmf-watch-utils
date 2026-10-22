// entry 0x100bfa60  FUN_100bfa60  (100bfa60)

int FUN_100bfa60(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_2 == 0) {
    iVar2 = *(int *)(param_1 + 0x14);
    iVar1 = FUN_10184a5c(param_1 + 4,PTR_DAT_100bfaa4,iVar2 + 0x48,0xd,param_4);
    if (iVar1 < 0) {
      FUN_1014c818(DAT_100bfab0 | ((int)PTR_DAT_100bfaac - (int)PTR_LAB_100bfaa8) * 0x20 & 0xff00U,
                   DAT_100bfab4);
      iVar1 = -2;
    }
    else {
      FUN_100df934(*(undefined4 *)(iVar2 + 4));
    }
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}


