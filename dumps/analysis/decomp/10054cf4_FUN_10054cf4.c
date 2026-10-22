// 0x1020189a FUN_10054cf4 :: system_app_launch: ota_fg = %d, res_chk = %d ver_chk = %d\n
// entry 0x10054cf4  FUN_10054cf4

undefined4 FUN_10054cf4(int param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  
  if (param_1 == 2) {
    FUN_101488f4(DAT_10054da0);
    uVar4 = 1;
  }
  else {
    FUN_101488f4(DAT_10054da0);
    iVar3 = FUN_10058508();
    piVar2 = DAT_10054da8;
    piVar1 = DAT_10054da4;
    if ((iVar3 != 0) || (*DAT_10054da4 != 0 || *DAT_10054da8 != 0)) {
      uVar4 = FUN_10058508();
      FUN_1014c818((DAT_10054dac - DAT_10054db0) * 0x20 & 0xff00U | 0x980033,DAT_10054db4,uVar4,
                   *piVar1,*piVar2);
      FUN_10054c94();
      FUN_1015a14c();
      return 0;
    }
    iVar3 = FUN_100585bc();
    if (iVar3 != 0) {
      FUN_10148a14(0xca,0,0);
      return 0;
    }
    FUN_1014c818((DAT_10054dac - DAT_10054db0) * 0x20 & 0xff00U | 0xab0031,DAT_10054db8,DAT_10054da0
                );
    iVar3 = FUN_101486e4(DAT_10054da0,8,0);
    if (iVar3 != 0xff) {
      return 0;
    }
    uVar4 = 8;
  }
  FUN_101486e4(DAT_10054da0,uVar4,0);
  return 0;
}


