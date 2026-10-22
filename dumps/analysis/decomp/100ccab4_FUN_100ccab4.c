// entry 0x100ccab4  FUN_100ccab4

undefined4 FUN_100ccab4(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  
  if ((param_1 == 0x804b) || (uVar1 = DAT_100ccaf8, (param_1 & 0x7fff) == 0x49)) {
    FUN_1014c818((DAT_100ccaec - DAT_100ccae8) * 0x20 & 0xff00U | 0x7d0031,DAT_100ccaf0,param_3,
                 DAT_100ccae8,param_4);
    uVar1 = DAT_100ccaf4;
  }
  return uVar1;
}


