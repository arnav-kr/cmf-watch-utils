// entry 0x100bfa14  FUN_100bfa14  (100bfa14)

undefined4 FUN_100bfa14(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int *piVar2;
  
  if (((*(int *)(param_1 + 0x14) == 0) || (param_2 != DAT_100bfa4c)) ||
     (piVar2 = (int *)ux2sys_api_7(0x51,0,param_3,DAT_100bfa4c,param_4), *piVar2 != 0)) {
    uVar1 = 0;
  }
  else {
    FUN_1014c818(DAT_100bfa58 | (DAT_100bfa54 - DAT_100bfa50) * 0x20 & 0xff00U,DAT_100bfa5c);
    uVar1 = 1;
  }
  return uVar1;
}


