// entry 0x100ec358  FUN_100ec358

int FUN_100ec358(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *DAT_100ec3dc;
  iVar1 = FUN_100eaf48(DAT_100ec3e0,*(undefined2 *)(param_1 + 1));
  iVar2 = DAT_100ec3f4;
  if (iVar1 != 0) {
    FUN_1014c818(DAT_100ec3ec | (DAT_100ec3e4 - DAT_100ec3e8) * 0x20 & 0xff00U,DAT_100ec3f0);
    return iVar1;
  }
  if (*(char *)(iVar3 + 0x12) != '\x03') {
    if (*(char *)(iVar3 + 0x12) == '\x02') {
      FUN_101071e0(iVar3 + 0x1c);
      *(undefined4 *)(iVar3 + 0x1c) = 0;
      *(undefined1 *)(iVar3 + 0x12) = 0;
    }
    iVar1 = FUN_1015cfc0();
    if (iVar1 == 0) {
      *(undefined1 *)(iVar2 + 0xb) = 1;
      *(undefined1 *)(iVar3 + 0x12) = 1;
      goto LAB_100ec392;
    }
  }
  *(undefined1 *)(iVar2 + 0xb) = 10;
LAB_100ec392:
  iVar2 = FUN_100eb074(1,0x52);
  if (iVar2 != 0) {
    FUN_1014c818(DAT_100ec3f8 | (DAT_100ec3e4 - DAT_100ec3e8) * 0x20 & 0xff00U,DAT_100ec3fc,0x52,
                 iVar2);
  }
  return iVar2;
}


