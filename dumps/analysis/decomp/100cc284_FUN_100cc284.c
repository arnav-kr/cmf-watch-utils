// entry 0x100cc284  FUN_100cc284

undefined4 FUN_100cc284(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *DAT_100cc2c4;
  while( true ) {
    if (param_2 == 0) {
      return 0;
    }
    iVar1 = FUN_10147748(*(undefined4 *)(iVar2 + 0x18),param_1,param_2);
    if (iVar1 < 1) break;
    param_2 = param_2 - iVar1;
  }
  FUN_1014c818((DAT_100cc2cc - DAT_100cc2c8) * 0x20 & 0xff00U | 0xa80011,DAT_100cc2d0);
  return 1;
}


