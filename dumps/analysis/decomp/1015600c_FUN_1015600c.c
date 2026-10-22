// entry 0x1015600c  FUN_1015600c

int FUN_1015600c(undefined1 *param_1,undefined4 param_2)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = (DAT_10156074 - DAT_10156070) * 0x20 & 0xff00;
  FUN_1014c818(DAT_10156078 | uVar1,DAT_10156080,DAT_1015607c,param_2);
  if (*(int *)(param_1 + 0x30) == 0) {
    FUN_1014c818(DAT_10156084 | uVar1,DAT_10156088);
    iVar2 = -5;
  }
  else {
    **(undefined1 **)(param_1 + 8) = (char)param_2;
    *(undefined4 *)(param_1 + 4) = 1;
    iVar2 = FUN_10155d84(param_1,0xa041);
    if (iVar2 == 0) {
      *param_1 = 0;
    }
    else {
      FUN_1014c818(DAT_1015608c | uVar1,DAT_10156090,0xa041,iVar2);
    }
  }
  return iVar2;
}


