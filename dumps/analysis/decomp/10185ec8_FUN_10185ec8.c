// entry 0x10185ec8  FUN_10185ec8  (10185ec8)

int FUN_10185ec8(int param_1,int param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(DAT_10185eec + 4);
  while( true ) {
    if (piVar1 == (int *)0x0) {
      return 0;
    }
    if ((*piVar1 == param_1) && (piVar1[1] == param_2)) break;
    piVar1 = (int *)piVar1[0xf];
  }
  piVar1[3] = piVar1[3] + 1;
  return piVar1[8];
}


