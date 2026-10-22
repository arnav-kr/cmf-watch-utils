// entry 0x100eb21a  FUN_100eb218  (100eb218)

int FUN_100eb218(int param_1,undefined2 param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_100eaf48(DAT_100eb284,*(undefined2 *)(param_1 + 1),param_3,0,param_1,param_2);
  if (iVar1 != 0) {
    FUN_1014c818(DAT_100eb294 | (DAT_100eb28c - DAT_100eb288) * 0x20 & 0xff00U,DAT_100eb290,iVar1);
  }
  iVar2 = FUN_101b608c(&stack0xffffffee,DAT_100eb284,*(undefined2 *)(param_1 + 1),0x9060);
  if (iVar2 == 0) {
    FUN_1014c818((DAT_100eb28c - DAT_100eb288) * 0x20 & 0xff00U | 0x2d40011,DAT_100eb29c,
                 DAT_100eb298);
    iVar1 = -1;
  }
  return iVar1;
}


