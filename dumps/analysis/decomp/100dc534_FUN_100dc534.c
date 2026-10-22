// entry 0x100dc534  FUN_100dc534  (100dc534)

undefined4 FUN_100dc534(int param_1)

{
  int *piVar1;
  int iVar2;
  
  if (param_1 == -1) {
    return 1;
  }
  iVar2 = 0;
  piVar1 = DAT_100dc558;
  do {
    if (param_1 == *piVar1) {
      return 0;
    }
    iVar2 = iVar2 + 1;
    piVar1 = piVar1 + 1;
  } while (iVar2 != 6);
  return 2;
}


