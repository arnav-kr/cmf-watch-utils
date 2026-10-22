// entry 0x100eb2a0  FUN_100eb2a0  (100eb2a0)

undefined4 FUN_100eb2a0(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  
  iVar1 = DAT_100eb330;
  iVar5 = DAT_100eb32c;
  iVar7 = *DAT_100eb328;
  iVar3 = *(int *)(iVar7 + 8);
  *(undefined1 *)(iVar7 + 0x10) = 0;
  uVar2 = DAT_100eb338;
  uVar8 = *(uint *)(iVar1 + 1) - iVar3;
  if (0xbff < uVar8) {
    uVar8 = 0xc00;
  }
  uVar6 = (uint)(iVar3 * 100) / *(uint *)(iVar1 + 1);
  iVar5 = iVar5 - DAT_100eb334;
  *(undefined4 *)(iVar7 + 0xc) = 0;
  FUN_1014c818(uVar2 | iVar5 * 0x20 & 0xff00U,DAT_100eb33c,iVar3,uVar8,uVar6,param_2);
  iVar5 = DAT_100eb340;
  uVar4 = *(undefined4 *)(iVar7 + 8);
  *(char *)(DAT_100eb340 + 0xb) = (char)((uint)uVar4 >> 0x18);
  *(char *)(iVar5 + 0xc) = (char)((uint)uVar4 >> 0x10);
  *(char *)(iVar5 + 0xd) = (char)((uint)uVar4 >> 8);
  *(char *)(iVar5 + 0xe) = (char)uVar4;
  *(undefined1 *)(iVar5 + 0xf) = 0;
  *(undefined1 *)(iVar5 + 0x10) = 0;
  *(char *)(iVar5 + 0x11) = (char)(uVar8 >> 8);
  *(char *)(iVar5 + 0x12) = (char)uVar8;
  *(char *)(iVar5 + 0x13) = (char)uVar6;
  uVar4 = FUN_100eb074(9,0xa05f);
  FUN_101be22e(iVar7 + 0x50,3000);
  return uVar4;
}


