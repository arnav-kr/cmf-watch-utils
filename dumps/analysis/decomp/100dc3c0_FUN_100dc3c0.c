// entry 0x100dc3c0  FUN_100dc3c0

undefined4 FUN_100dc3c0(undefined4 param_1,int *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  undefined1 uStack_40;
  ushort local_3f;
  undefined1 auStack_3c [36];
  
  uVar5 = (DAT_100dc484 - DAT_100dc480) * 0x20 & 0xff00;
  FUN_1014c818(uVar5 | 0x1400031,DAT_100dc48c,DAT_100dc488);
  iVar3 = FUN_100db468(param_1,0,auStack_3c,0x24);
  if ((iVar3 == 0x24) && (iVar3 = FUN_100db468(param_1,0x24,&uStack_40,3), iVar3 == 3)) {
    iVar6 = local_3f + 0x27;
    iVar3 = FUN_10188234(1,iVar6,DAT_100dc488);
    if (iVar3 != 0) {
      iVar4 = FUN_100db468(param_1,0,iVar3,iVar6);
      if (iVar4 == iVar6) {
        FUN_1014c818(uVar5 | 0x1800031,DAT_100dc490,iVar3 + 8);
        FUN_1014c818(DAT_100dc494 | uVar5,DAT_100dc498,*(undefined1 *)(iVar3 + 0x17));
        uVar2 = DAT_100dc49c;
        uVar1 = DAT_100dc488;
        *param_2 = iVar3;
        FUN_1014c818(uVar5 | 0x18c0031,uVar2,uVar1);
        return 0;
      }
      FUN_10188380(1,iVar3);
    }
  }
  FUN_1014c818(DAT_100dc4a0 | uVar5,DAT_100dc4a4,DAT_100dc488);
  return 0xffffffed;
}


