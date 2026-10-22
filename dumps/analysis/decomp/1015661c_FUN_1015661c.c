// entry 0x1015661c  FUN_1015661c  (1015661c)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_1015661c(int param_1)

{
  char *pcVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  uVar2 = (_LAB_10156674 - _DAT_10156670) * 0x20 & 0xff00;
  iVar4 = param_1;
  FUN_1014c818(_LAB_10156678 | uVar2,_LAB_1015667c,param_1,_DAT_10156670,param_1);
  pcVar1 = _LAB_10156680;
  do {
    if (*(int *)(param_1 + 0x30) != 0) {
      iVar3 = 0;
      break;
    }
    iVar3 = func_0x10156284(param_1);
    if (*pcVar1 != '\0') {
      return -5;
    }
  } while (iVar3 == 0);
  FUN_1014c818(_LAB_10156684 | uVar2,_LAB_10156688,param_1,iVar3,iVar4);
  return iVar3;
}


