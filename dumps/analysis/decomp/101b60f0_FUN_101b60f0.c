// entry 0x101b60f0  FUN_101b60f0

/* WARNING: Removing unreachable block (ram,0x100d1020) */
/* WARNING: Removing unreachable block (ram,0x100d1024) */
/* WARNING: Removing unreachable block (ram,0x100d10c2) */
/* WARNING: Removing unreachable block (ram,0x100d10ca) */
/* WARNING: Removing unreachable block (ram,0x100d10f2) */
/* WARNING: Removing unreachable block (ram,0x100d10d2) */
/* WARNING: Removing unreachable block (ram,0x100d10da) */
/* WARNING: Removing unreachable block (ram,0x100d10e0) */
/* WARNING: Removing unreachable block (ram,0x100d10e2) */
/* WARNING: Removing unreachable block (ram,0x100d10e8) */
/* WARNING: Removing unreachable block (ram,0x100d10ec) */
/* WARNING: Removing unreachable block (ram,0x100d10aa) */
/* WARNING: Removing unreachable block (ram,0x100d10b2) */
/* WARNING: Removing unreachable block (ram,0x100d1004) */
/* WARNING: Removing unreachable block (ram,0x100d100c) */
/* WARNING: Removing unreachable block (ram,0x100d100e) */
/* WARNING: Removing unreachable block (ram,0x100d0fe4) */
/* WARNING: Removing unreachable block (ram,0x100d0fe8) */
/* WARNING: Removing unreachable block (ram,0x100d0fec) */
/* WARNING: Removing unreachable block (ram,0x100d0ff0) */
/* WARNING: Removing unreachable block (ram,0x100d0ff4) */
/* WARNING: Removing unreachable block (ram,0x100d0ff8) */
/* WARNING: Removing unreachable block (ram,0x100d0ffc) */
/* WARNING: Removing unreachable block (ram,0x100d10ba) */
/* WARNING: Removing unreachable block (ram,0x100d10b6) */
/* WARNING: Removing unreachable block (ram,0x100d10ae) */
/* WARNING: Removing unreachable block (ram,0x100d10a6) */
/* WARNING: Removing unreachable block (ram,0x100d10a2) */
/* WARNING: Removing unreachable block (ram,0x100d10c6) */
/* WARNING: Removing unreachable block (ram,0x100d10be) */

undefined1 FUN_101b60f0(void)

{
  char *pcVar1;
  uint *puVar2;
  uint *puVar3;
  uint *puVar4;
  undefined1 uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  undefined4 uVar9;
  
  uVar9 = 1;
  uVar5 = 0;
  if (*DAT_100d1104 != '\0') {
    uVar8 = (DAT_100d110c - DAT_100d1108) * 0x20 & 0xff00;
    FUN_1014c818(DAT_100d1110 | uVar8,DAT_100d1114,0x53,1,0x53,1);
    puVar2 = DAT_100d111c;
    pcVar1 = DAT_100d1118;
    if (*DAT_100d1118 != '\0') {
      *DAT_100d1118 = '\0';
      iVar6 = FUN_100c61f0(3);
      uVar7 = iVar6 / 0x19;
      if (iVar6 != uVar7 * 0x19) {
        uVar7 = uVar7 + 1;
      }
      *puVar2 = uVar7;
      FUN_1014c818(DAT_100d1120 | uVar8,DAT_100d1124,iVar6,*puVar2,3,uVar9);
    }
    uVar9 = DAT_100d1138;
    uVar7 = FUN_101b60be(0x53,1,DAT_100d1138,200);
    puVar3 = DAT_100d1128;
    *DAT_100d1128 = uVar7;
    FUN_1014c818(DAT_100d112c | uVar8,DAT_100d1130,uVar7);
    puVar4 = DAT_100d1134;
    if ((int)*puVar3 < 1) {
      uVar5 = 0;
      *puVar2 = 1;
      *puVar4 = 1;
      *pcVar1 = '\x01';
    }
    else {
      FUN_100cd324(0x53,1,(short)*puVar2,(short)*DAT_100d1134,*puVar3 & 0xffff,uVar9);
      uVar7 = *puVar4;
      uVar8 = *puVar2;
      uVar5 = 1;
      *puVar4 = uVar7 + 1;
      if (uVar8 < uVar7 + 1) {
        *puVar4 = 1;
      }
    }
  }
  return uVar5;
}


