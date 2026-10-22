// entry 0x100eba22  FUN_100eba22  (100eba22)

undefined4 FUN_100eba22(undefined4 param_1,undefined4 param_2,undefined4 param_3,int *param_4)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  
  iVar7 = *param_4;
  *(undefined1 *)(iVar7 + 0x10) = 0;
  *(undefined4 *)(iVar7 + 0xc) = 0;
  puVar1 = DAT_100ebaac;
  iVar5 = *(int *)(iVar7 + 8);
  uVar3 = *(uint *)(DAT_100ebaa8 + 1);
  uVar6 = uVar3 - iVar5;
  if (0xbff < uVar6) {
    uVar6 = 0xc00;
  }
  if (uVar3 != 0) {
    *DAT_100ebaac = (uint)(iVar5 * 100) / uVar3;
  }
  iVar2 = DAT_100ebab0;
  *(char *)(DAT_100ebab0 + 0xb) = (char)((uint)iVar5 >> 0x18);
  *(char *)(iVar2 + 0xc) = (char)((uint)iVar5 >> 0x10);
  *(char *)(iVar2 + 0xd) = (char)((uint)iVar5 >> 8);
  *(undefined1 *)(iVar2 + 0xf) = 0;
  *(undefined1 *)(iVar2 + 0x10) = 0;
  *(char *)(iVar2 + 0x11) = (char)(uVar6 >> 8);
  uVar3 = *puVar1;
  *(char *)(iVar2 + 0xe) = (char)iVar5;
  *(char *)(iVar2 + 0x12) = (char)uVar6;
  *(char *)(iVar2 + 0x13) = (char)uVar3;
  FUN_1014c818(DAT_100ebabc | (DAT_100ebab4 - DAT_100ebab8) * 0x20 & 0xff00U,DAT_100ebac0,iVar5,
               uVar6,uVar3,param_2,param_3);
  uVar4 = FUN_100eb074(9,0xa064);
  FUN_101be22e(iVar7 + 0x50,1000);
  return uVar4;
}


