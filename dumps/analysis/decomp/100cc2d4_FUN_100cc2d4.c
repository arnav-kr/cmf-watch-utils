// entry 0x100cc2d4  FUN_100cc2d4

undefined4 FUN_100cc2d4(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  uVar3 = (DAT_100cc338 - DAT_100cc334) * 0x20 & 0xff00;
  FUN_1014c818(uVar3 | 0xb30031,DAT_100cc33c,param_2);
  iVar1 = FUN_10147890(*(undefined4 *)(*DAT_100cc340 + 0x18),param_1,param_2);
  FUN_1014c818(uVar3 | 0xb70036,DAT_100cc344,*(undefined1 *)(param_1 + 3),
               *(undefined1 *)(param_1 + 4),*(undefined1 *)(param_1 + 9),
               *(undefined1 *)(param_1 + 10),param_2,iVar1);
  if (param_2 == iVar1) {
    uVar2 = 0;
  }
  else {
    uVar2 = 0xfffffffb;
  }
  return uVar2;
}


