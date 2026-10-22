// 0x1021eb84 FUN_100c4318 :: alarms_update close!!!\n
// entry 0x100c4318  FUN_100c4318

undefined4 FUN_100c4318(uint param_1)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  if (param_1 < 5) {
    FUN_1014c818(DAT_100c43b4 | (DAT_100c43b0 - DAT_100c43ac) * 0x20 & 0xff00U,DAT_100c43b8);
    iVar3 = DAT_100c43c4;
    iVar2 = DAT_100c43c0;
    iVar7 = 0;
    bVar1 = false;
    iVar6 = param_1 * 0x2a + DAT_100c43bc;
    iVar8 = DAT_100c43bc;
    do {
      if (((*(char *)(iVar8 + 0x938) == '\x01') &&
          (*(char *)(iVar8 + 0x936) == *(char *)(iVar6 + 0x936))) &&
         (*(char *)(iVar8 + 0x937) == *(char *)(iVar6 + 0x937))) {
        if (*(char *)(iVar8 + 0x93a) == '\0') {
          *(undefined1 *)(iVar8 + 0x938) = 0;
          bVar1 = true;
        }
        *(undefined1 *)(iVar7 + iVar2) = 0;
        *(undefined1 *)(iVar7 + iVar3) = 0;
      }
      iVar7 = iVar7 + 1;
      iVar8 = iVar8 + 0x2a;
    } while (iVar7 != 5);
    cVar4 = FUN_100c3980(param_1,1);
    if (bVar1) {
      uVar5 = 0;
    }
    else {
      uVar5 = 0;
      if (cVar4 != '\0') {
        uVar5 = 1;
      }
    }
  }
  else {
    uVar5 = 0xffffffff;
  }
  return uVar5;
}


