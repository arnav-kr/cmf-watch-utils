// entry 0x1018433c  FUN_1018433c  (1018433c)

int FUN_1018433c(int param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  
  if (param_1 == 0) {
    FUN_1014c818(DAT_101843a4 | (DAT_1018439c - DAT_101843a0) * 0x20 & 0xff00U,DAT_101843a8);
  }
  else if ((*(int **)(param_1 + 4) == (int *)0x0) ||
          (*(int *)(**(int **)(param_1 + 4) + 0xc) != *(int *)(*param_2 + 0xc))) {
    do {
      iVar1 = *(int *)(param_1 + 0x18);
      piVar2 = *(int **)(param_1 + 4);
      if (iVar1 == 0) {
        if (piVar2 == (int *)0x0) {
          return param_1;
        }
        if (*(int *)(*piVar2 + 0xc) == *(int *)(*param_2 + 0xc)) {
          return 0;
        }
        return param_1;
      }
      param_1 = iVar1;
    } while ((piVar2 == (int *)0x0) || (*(int *)(*piVar2 + 0xc) != *(int *)(*param_2 + 0xc)));
  }
  return 0;
}


