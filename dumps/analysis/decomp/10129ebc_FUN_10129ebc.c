// entry 0x10129ebc  FUN_10129ebc  (10129ebc)

undefined4 FUN_10129ebc(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = FUN_101c6e06(param_1,0,1,param_4,param_4);
  iVar2 = FUN_101c6e06(param_1,0,4);
  if ((iVar1 == DAT_10129eec) || (iVar2 == DAT_10129eec)) {
    FUN_101c5dae(param_1);
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}


