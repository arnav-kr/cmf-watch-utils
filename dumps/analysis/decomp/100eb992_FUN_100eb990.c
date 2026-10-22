// entry 0x100eb992  FUN_100eb990  (100eb990)

int FUN_100eb990(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *DAT_100eb9fc;
  *(undefined1 *)(iVar2 + 0x12) = 0;
  iVar2 = FUN_100eaf48(DAT_100eba00,*(undefined2 *)(param_1 + 1),iVar2,0,param_1,param_2);
  if (iVar2 != 0) {
    FUN_1014c818(DAT_100eba10 | (DAT_100eba08 - DAT_100eba04) * 0x20 & 0xff00U,DAT_100eba0c,iVar2);
  }
  iVar1 = FUN_101b608c(&stack0xffffffee,DAT_100eba00,*(undefined2 *)(param_1 + 1),0x9065);
  if (iVar1 == 0) {
    FUN_1014c818(DAT_100eba14 | (DAT_100eba08 - DAT_100eba04) * 0x20 & 0xff00U,DAT_100eba1c,
                 DAT_100eba18);
    iVar2 = -1;
  }
  return iVar2;
}


