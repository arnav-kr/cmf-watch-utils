// entry 0x10184804  FUN_10184804  (10184804)

undefined4
FUN_10184804(undefined4 param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  
  if (param_2 == (undefined4 *)0x0) {
    iVar3 = DAT_10184864 - DAT_10184868;
    uVar1 = DAT_10184870;
    uVar2 = DAT_1018486c;
    uVar5 = param_1;
  }
  else {
    uVar5 = 0;
    iVar3 = FUN_10184674(param_3,param_4,param_5,1,0,param_2,param_3);
    param_2[2] = iVar3;
    if (iVar3 != 0) {
      iVar3 = FUN_10186440(iVar3,param_1);
      if (iVar3 == 0) {
        return 0xffffffff;
      }
      *param_2 = param_1;
      param_2[1] = iVar3;
      return 0;
    }
    iVar3 = DAT_10184864 - DAT_10184868;
    uVar1 = DAT_10184878;
    uVar2 = DAT_10184874;
  }
  uVar4 = iVar3 * 0x20 & 0xff00;
  FUN_1014c818(uVar1 | uVar4,uVar2,DAT_10184868,uVar4,uVar5);
  return 0xffffffff;
}


