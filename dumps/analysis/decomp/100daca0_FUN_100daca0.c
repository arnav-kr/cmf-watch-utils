// 0x10223afa FUN_100daca0 :: WFDispItemListUpdateMatch
// entry 0x100daca0  FUN_100daca0

int * FUN_100daca0(undefined4 *param_1,int *param_2,int param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  undefined4 extraout_r2;
  int *piVar3;
  
  if ((param_1 != (undefined4 *)0x0) && (param_3 != 0)) {
    piVar2 = (int *)FUN_101b79b8(*param_1,*(undefined2 *)(param_3 + 4),param_3,param_4,param_4);
    if (piVar2 != (int *)0x0) {
      FUN_101b79f0(piVar2,extraout_r2);
      return piVar2;
    }
    piVar2 = (int *)FUN_10188234(1,0x80,DAT_100dad20);
    if (piVar2 != (int *)0x0) {
      FUN_101bf3aa(piVar2,param_3,0x80);
      *(byte *)((int)piVar2 + 6) = *(byte *)((int)piVar2 + 6) | 0x80;
      *(byte *)(param_3 + 6) = *(byte *)(param_3 + 6) | 0x80;
      if (param_2 == (int *)0x0) {
        piVar1 = (int *)*param_1;
        piVar3 = param_2;
        do {
          param_2 = piVar3;
          piVar3 = piVar1;
          if (piVar3 == (int *)0x0) break;
          piVar1 = (int *)*piVar3;
        } while (*(ushort *)(piVar3 + 1) <= *(ushort *)(param_3 + 4));
        if (param_2 == (int *)0x0) {
          FUN_101b90e2(param_1,piVar2);
          return piVar2;
        }
      }
      if (param_2 == piVar2) {
        return piVar2;
      }
      *piVar2 = *param_2;
      *param_2 = (int)piVar2;
      return piVar2;
    }
  }
  return (int *)0x0;
}


