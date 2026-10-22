// entry 0x100eb024  FUN_100eb024  (100eb024)

undefined4 FUN_100eb024(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_10147890(*(undefined4 *)(*DAT_100eb060 + 0x18),param_1,param_2,*DAT_100eb060,param_1);
  if (param_2 == iVar1) {
    uVar2 = 0;
  }
  else {
    FUN_1014c818(DAT_100eb070 | (DAT_100eb068 - DAT_100eb064) * 0x20 & 0xff00U,DAT_100eb06c,param_2,
                 iVar1);
    uVar2 = 0xfffffffb;
  }
  return uVar2;
}


