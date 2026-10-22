// entry 0x100dc6b0  FUN_100dc6b0  (100dc6b0)

uint FUN_100dc6b0(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  undefined1 auStack_a0 [23];
  byte local_89;
  undefined1 auStack_7c [44];
  undefined4 local_50;
  undefined1 auStack_4c [60];
  
  uVar2 = 0;
  local_50 = 0;
  FUN_101bf3b2(auStack_4c,0,0x3c);
  FUN_101bf3b2(auStack_7c,0,0x2c);
  iVar1 = FUN_100dc534(param_1);
  if (iVar1 != 1) {
    FUN_100de99c(param_1,&local_50,0x40,0);
    uVar2 = FUN_10107030(auStack_7c,&local_50,1);
    if (uVar2 == 0) {
      iVar1 = FUN_10107238(auStack_7c,auStack_a0);
      uVar2 = 0;
      if (iVar1 == 0x24) {
        uVar2 = (uint)local_89;
        FUN_1014c818((DAT_100dc758 - DAT_100dc754) * 0x20 & 0xff00U | 0x1c00032,DAT_100dc760,uVar2,
                     param_1);
      }
      FUN_101071e0(auStack_7c);
    }
    else {
      FUN_1014c818((DAT_100dc758 - DAT_100dc754) * 0x20 & 0xff00U | 0x1b60012,DAT_100dc75c,&local_50
                   ,uVar2);
      uVar2 = uVar2 & 0xff;
    }
  }
  return uVar2;
}


