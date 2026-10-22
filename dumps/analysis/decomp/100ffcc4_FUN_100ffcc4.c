// entry 0x100ffcc4  FUN_100ffcc4  (100ffcc4)

int FUN_100ffcc4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5,undefined4 param_6,undefined4 param_7,int param_8,int param_9)

{
  char *pcVar1;
  int iVar2;
  
  pcVar1 = DAT_100ffd3c;
  if (*DAT_100ffd3c == '\0') {
    iVar2 = FUN_100ffc90(DAT_100ffd38);
    if (iVar2 != 0) {
      return iVar2;
    }
    *pcVar1 = '\x01';
  }
  iVar2 = FUN_101be68a(DAT_100ffd38);
  if (iVar2 == 0) {
    iVar2 = FUN_100ffa70(DAT_100ffd38,param_1,param_2,param_3,param_4,param_5,param_6,param_7,
                         param_8,param_9);
    FUN_101be692(DAT_100ffd38,0x32);
    FUN_101be698(DAT_100ffd38);
    if (iVar2 == 0) {
      iVar2 = param_9 * param_8 * 2;
    }
  }
  return iVar2;
}


