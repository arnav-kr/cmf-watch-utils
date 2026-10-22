// entry 0x1007f468  FUN_1007f468

undefined1 FUN_1007f468(void)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 in_r3;
  
  iVar2 = *DAT_1007f49c;
  if (iVar2 < 1) {
    uVar1 = *DAT_1007f4ac;
  }
  else {
    uVar1 = *(undefined1 *)(DAT_1007f4a8 + iVar2 + -1);
  }
  FUN_1014c818(DAT_1007f4a4 | (DAT_1007f4a0 - DAT_1007f4b0) * 0x20 & 0xff00U,DAT_1007f4b4,iVar2,
               uVar1,in_r3);
  return uVar1;
}


