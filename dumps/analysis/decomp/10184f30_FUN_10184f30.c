// entry 0x10184f30  FUN_10184f30  (10184f30)

void FUN_10184f30(undefined4 param_1,uint *param_2,int param_3,undefined4 param_4)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint extraout_r2;
  
  puVar1 = DAT_10184f74;
  if (param_2 != (uint *)0x0) {
    uVar2 = *DAT_10184f70;
    uVar2 = uVar2 * ((param_3 + (uVar2 - 1)) / uVar2);
    uVar3 = uVar2 + *param_2;
    if (*DAT_10184f74 < uVar3) {
      if (*param_2 != 0) {
        FUN_10186ce4();
        uVar3 = extraout_r2;
      }
      if (*puVar1 <= uVar2) {
        FUN_10186ce4(param_1,uVar2,uVar3,*puVar1,param_4);
        uVar2 = 0;
      }
      *param_2 = uVar2;
    }
    else {
      *param_2 = uVar3;
    }
  }
  return;
}


