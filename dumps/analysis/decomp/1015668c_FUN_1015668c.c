// entry 0x1015668c  FUN_1015668c  (1015668c)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_1015668c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = (_LAB_101566d8 - _DAT_101566dc) * 0x20 & 0xff00;
  if (*(int *)(param_1 + 0x34) != 0) {
    iVar1 = FUN_101474d4(*(int *)(param_1 + 0x34),3,param_3,_DAT_101566dc,param_4);
    if (iVar1 != 0) {
      FUN_1014c818(_LAB_101566e0 | uVar2,_LAB_101566e4);
      return iVar1;
    }
    *(undefined4 *)(param_1 + 0x38) = 1;
  }
  FUN_1014c818(_LAB_101566e8 | uVar2,_LAB_101566ec,*(undefined4 *)(param_1 + 0x34));
  FUN_1015661c(param_1 + 0xc);
  return 0;
}


