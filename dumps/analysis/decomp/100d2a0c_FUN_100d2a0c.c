// 0x10222a58 FUN_100d2a0c :: gps version[%d] < 5 do not request epo !!!\n
// entry 0x100d2a0c  FUN_100d2a0c

void FUN_100d2a0c(void)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 local_20;
  undefined4 local_1c;
  undefined2 local_18;
  undefined1 auStack_14 [3];
  char local_11;
  byte local_10;
  short local_e;
  
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  uVar2 = thunk_FUN_101978c0();
  if ((uVar2 < 5) || (iVar3 = FUN_100ec57c(), iVar3 == 0)) {
    iVar3 = thunk_FUN_101978c0();
    iVar6 = DAT_100d2b04 - DAT_100d2b00;
    uVar2 = DAT_100d2b0c;
    uVar4 = DAT_100d2b08;
  }
  else {
    uVar4 = FUN_10145a58();
    uVar5 = FUN_101bf36a(DAT_100d2b10);
    iVar3 = FUN_101bf39a(uVar4,DAT_100d2b10,uVar5);
    if (iVar3 == 0) {
      FUN_1014c818(DAT_100d2b14 | (DAT_100d2b04 - DAT_100d2b00) * 0x20 & 0xff00U,DAT_100d2b18);
      return;
    }
    FUN_101b5932(*(undefined4 *)(DAT_100d2b20 + 0x469c),
                 *DAT_100d2b1c * 0xe10 + DAT_100d2b1c[1] * 0x3c,&local_20);
    FUN_100c14b4(auStack_14);
    if ((((local_1c._2_2_ == local_e) && ((local_1c & 0xff) == (uint)local_10)) &&
        (local_20._3_1_ == local_11)) && (1 < local_20._2_1_)) {
      return;
    }
    iVar6 = thunk_FUN_1019b7e0();
    piVar1 = DAT_100d2b24;
    iVar3 = *DAT_100d2b24;
    if ((DAT_100d2b28 < (uint)(iVar6 - iVar3)) || (iVar3 == 0)) {
      iVar3 = thunk_FUN_1019b7e0();
      *piVar1 = iVar3;
      FUN_100d2978();
      return;
    }
    iVar6 = DAT_100d2b04 - DAT_100d2b00;
    uVar2 = DAT_100d2b2c;
    uVar4 = DAT_100d2b30;
  }
  FUN_1014c818(uVar2 | iVar6 * 0x20 & 0xff00U,uVar4,iVar3);
  return;
}


