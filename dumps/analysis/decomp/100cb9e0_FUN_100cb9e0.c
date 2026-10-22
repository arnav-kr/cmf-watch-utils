// entry 0x100cb9e0  FUN_100cb9e0

undefined4 FUN_100cb9e0(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_10147890(*(undefined4 *)(*DAT_100cba20 + 0x18),param_1,param_2);
  FUN_1014c818((DAT_100cba24 - DAT_100cba28) * 0x20 & 0xff00U | 0x8f0032,DAT_100cba2c,param_2,iVar1)
  ;
  if (param_2 == iVar1) {
    uVar2 = 0;
  }
  else {
    uVar2 = 0xfffffffb;
  }
  return uVar2;
}


