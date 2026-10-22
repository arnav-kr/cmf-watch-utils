// entry 0x10107030  FUN_10107030  (10107030)

int FUN_10107030(int param_1,char *param_2,uint param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  code *pcVar3;
  int iVar4;
  uint uVar5;
  int local_14;
  
  *(char *)(param_1 + 8) = (char)param_3;
  *(undefined1 *)(param_1 + 9) = 0;
  local_14 = param_4;
  if (((param_2 == (char *)0x0) ||
      (iVar4 = param_1, uVar5 = param_3, uVar1 = FUN_101bf36a(param_2), uVar1 < 2)) ||
     (*param_2 != '/')) {
    FUN_1014c818((DAT_10107114 - DAT_10107110) * 0x20 & 0xff00U | 0x940011,DAT_10107118);
    return -0x16;
  }
  if (*(int *)(param_1 + 4) != 0) {
    return -0x10;
  }
  iVar2 = FUN_10106fa0(&local_14);
  if (iVar2 < 0) {
    FUN_1014c818((DAT_10107114 - DAT_10107110) * 0x20 & 0xff00U | 0x9e0011,DAT_1010711c);
    return iVar2;
  }
  if (((int)((uint)*(byte *)(local_14 + 0x20) << 0x1e) < 0) && ((param_3 & 0x12) != 0)) {
    return -0x1e;
  }
  pcVar3 = (code *)**(undefined4 **)(local_14 + 0x1c);
  if (pcVar3 == (code *)0x0) {
    return -0x86;
  }
  *(int *)(param_1 + 4) = local_14;
  iVar2 = (*pcVar3)(param_1,param_2,param_3);
  if (iVar2 < 0) {
    FUN_1014c818((DAT_10107114 - DAT_10107110) * 0x20 & 0xff00U | 0xae0011,DAT_10107120,iVar2,
                 DAT_10107110,iVar4,param_2,uVar5);
    *(undefined4 *)(param_1 + 4) = 0;
    return iVar2;
  }
  FUN_101bf352(param_1 + 9,param_2,0x20);
  *(undefined1 *)(param_1 + 0x28) = 0;
  return iVar2;
}


