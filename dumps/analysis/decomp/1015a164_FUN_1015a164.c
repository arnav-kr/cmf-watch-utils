// entry 0x1015a164  FUN_1015a164

int FUN_1015a164(void)

{
  undefined1 *puVar1;
  int iVar2;
  uint uVar3;
  
  puVar1 = DAT_1015a1d4;
  FUN_101bf3b2(DAT_1015a1d4,0,0x233);
  *puVar1 = 0xa4;
  puVar1[1] = 0xa3;
  puVar1[2] = 0xa2;
  iVar2 = DAT_1015a1d8;
  puVar1[3] = 0xa1;
  uVar3 = (iVar2 - DAT_1015a1dc) * 0x20 & 0xff00;
  FUN_1014c818(uVar3 | 0x1d40031,DAT_1015a1e0);
  FUN_101072f0(DAT_1015a1e4,0);
  iVar2 = FUN_10107294(DAT_1015a1e4,puVar1,0x233);
  if (iVar2 != 0x233) {
    FUN_1014c818(DAT_1015a1e8 | uVar3,DAT_1015a1ec,iVar2);
  }
  return iVar2;
}


