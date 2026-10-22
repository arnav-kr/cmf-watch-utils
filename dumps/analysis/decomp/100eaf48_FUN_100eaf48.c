// entry 0x100eaf48  FUN_100eaf48  (100eaf48)

undefined4 FUN_100eaf48(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *DAT_100eaf84;
  while( true ) {
    if (param_2 == 0) {
      return 0;
    }
    iVar1 = FUN_10147748(*(undefined4 *)(iVar2 + 0x18),param_1,param_2);
    if (iVar1 < 1) break;
    param_2 = param_2 - iVar1;
  }
  FUN_1014c818(DAT_100eaf90 | (DAT_100eaf8c - DAT_100eaf88) * 0x20 & 0xff00U,DAT_100eaf94);
  return 5;
}


