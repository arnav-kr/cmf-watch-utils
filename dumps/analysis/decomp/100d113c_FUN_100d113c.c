// 0x10223210 FUN_100d113c :: UpdateRecordDataSleep
// entry 0x100d113c  FUN_100d113c

uint FUN_100d113c(void)

{
  byte *pbVar1;
  ushort *puVar2;
  int *piVar3;
  byte *pbVar4;
  ushort *puVar5;
  uint *puVar6;
  undefined4 *puVar7;
  ushort uVar8;
  int iVar9;
  uint uVar10;
  undefined4 uVar11;
  uint uVar12;
  undefined4 *puVar13;
  undefined4 uVar14;
  byte bVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  undefined4 *puVar20;
  undefined4 *puVar21;
  uint uVar23;
  undefined4 *puVar22;
  
  puVar6 = DAT_100d1498;
  puVar5 = DAT_100d1494;
  piVar3 = DAT_100d1424;
  puVar2 = DAT_100d141c;
  pbVar1 = DAT_100d1418;
  uVar16 = (uint)*DAT_100d1494;
  uVar23 = (DAT_100d1414 - DAT_100d1410) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100d1420 | uVar23,DAT_100d142c,DAT_100d1428,*DAT_100d141c,*DAT_100d1418,
               *DAT_100d1498,uVar16);
  if (*piVar3 == 0) {
    iVar9 = thunk_FUN_10146a3c(0x350,DAT_100d1428);
    uVar14 = DAT_100d1428;
    *piVar3 = iVar9;
    if (iVar9 == 0) {
      FUN_1014c818(DAT_100d1430 | uVar23,DAT_100d1434,uVar14);
      return 0xffffffff;
    }
    FUN_1014c818(uVar23 | 0x3d40011,DAT_100d1438,uVar14);
  }
  if (*puVar2 == 0) {
    uVar8 = FUN_100d0d00(0x58,1,*piVar3,0x350);
    *puVar2 = uVar8;
    if ((uVar8 == 0) && (*piVar3 != 0)) {
      FUN_101ce04a();
      uVar11 = DAT_100d1440;
      uVar14 = DAT_100d1428;
      uVar10 = DAT_100d143c | uVar23;
      *piVar3 = 0;
      FUN_1014c818(uVar10,uVar11,uVar14);
    }
    uVar10 = 1;
    uVar8 = *puVar5;
    *DAT_100d1444 = 1;
    uVar12 = DAT_100d1448;
    *pbVar1 = 0;
    FUN_1014c818(uVar12 | uVar23,DAT_100d144c,DAT_100d1428,*puVar2,0,uVar8,uVar16);
  }
  else {
    uVar10 = 0;
  }
  pbVar4 = DAT_100d1450;
  uVar17 = (uint)*puVar2;
  uVar18 = (uint)*DAT_100d1450;
  uVar12 = uVar10;
  FUN_1014c818(DAT_100d1454 | uVar23,DAT_100d1458,DAT_100d1428,uVar17,uVar10,uVar18,uVar16);
  uVar19 = (uint)*puVar2;
  if (uVar19 == 0) goto LAB_100d1316;
  if (uVar10 != 0) {
    if (uVar19 < 0xc3) {
      uVar10 = 1;
      *pbVar4 = 1;
    }
    else {
      *pbVar4 = 2;
      uVar12 = (uVar19 - 0xc2) / 200;
      uVar10 = uVar12 + 1;
      if ((uVar19 - 0xc2) % 200 != 0) {
        uVar10 = uVar12 + 2;
      }
    }
    *puVar6 = uVar10;
    uVar12 = *puVar6;
    FUN_1014c818(uVar23 | 0x3fc0033,DAT_100d1470,DAT_100d1428,*pbVar4,uVar12,uVar18,uVar16);
    if (*puVar2 == 0) goto LAB_100d1316;
  }
  puVar7 = DAT_100d149c;
  bVar15 = *pbVar4;
  if (bVar15 == 1) {
    iVar9 = *piVar3;
    puVar13 = DAT_100d149c;
    puVar21 = (undefined4 *)(iVar9 + 0xc);
    do {
      puVar20 = puVar21 + 1;
      *puVar13 = *puVar21;
      puVar13 = puVar13 + 1;
      puVar21 = puVar20;
    } while (puVar20 != (undefined4 *)(iVar9 + 0x20));
    bVar15 = *(byte *)(iVar9 + 0x20);
    *(byte *)((int)puVar7 + 0x11) = bVar15;
    FUN_101ddb66(DAT_100d145c,iVar9 + 0x24,(uint)bVar15 << 3,0xb6);
    uVar11 = DAT_100d1464;
    uVar14 = DAT_100d1428;
    bVar15 = *(char *)(iVar9 + 0x20) * '\b' + 0x12;
    uVar16 = DAT_100d1460 | uVar23;
    *pbVar1 = bVar15;
    FUN_1014c818(uVar16,uVar11,uVar14,bVar15,uVar12,uVar18);
    *puVar5 = 0;
    *pbVar4 = 0;
  }
  else {
    if (bVar15 == 2) {
      iVar9 = *piVar3;
      puVar13 = DAT_100d149c;
      puVar21 = (undefined4 *)(iVar9 + 0xc);
      do {
        puVar22 = puVar21 + 1;
        puVar20 = puVar13 + 1;
        *puVar13 = *puVar21;
        puVar13 = puVar20;
        puVar21 = puVar22;
      } while (puVar22 != (undefined4 *)(iVar9 + 0x1c));
      *(undefined1 *)puVar20 = *(undefined1 *)puVar22;
      uVar14 = DAT_100d145c;
      *(undefined1 *)((int)puVar7 + 0x11) = *(undefined1 *)(iVar9 + 0x20);
      FUN_101bf3aa(uVar14,iVar9 + 0x24,0xb0);
      uVar19 = 0xc2;
      bVar15 = *(byte *)(iVar9 + 0x20);
      *pbVar4 = 3;
      uVar8 = bVar15 - 0x16;
      uVar12 = (uint)uVar8;
      *pbVar1 = 0xc2;
      *puVar5 = uVar8;
      uVar10 = DAT_100d1480;
      uVar11 = DAT_100d147c;
      uVar14 = DAT_100d1428;
    }
    else {
      if (bVar15 != 3) goto LAB_100d12a6;
      uVar16 = (uint)*puVar5;
      iVar9 = *piVar3 + (*(byte *)(*piVar3 + 0x20) - uVar16) * 8 + 0x24;
      if (uVar16 < 0x1a) {
        uVar16 = uVar16 << 3;
        FUN_101ddb66(DAT_100d149c,iVar9,uVar16,200);
        uVar19 = uVar16 & 0xff;
        *pbVar4 = 0;
        *pbVar1 = (byte)uVar16;
        *puVar5 = 0;
        uVar12 = 0;
        uVar10 = DAT_100d1490;
        uVar11 = DAT_100d148c;
        uVar14 = DAT_100d1428;
      }
      else {
        uVar19 = 200;
        FUN_101bf3aa(DAT_100d149c,iVar9,200);
        uVar12 = uVar16 - 0x19 & 0xffff;
        *pbVar4 = 3;
        *pbVar1 = 200;
        uVar10 = DAT_100d1488;
        uVar11 = DAT_100d1484;
        uVar14 = DAT_100d1428;
        *puVar5 = (ushort)(uVar16 - 0x19);
      }
    }
    FUN_1014c818(uVar10 | uVar23,uVar11,uVar14,uVar19,uVar12,uVar18);
  }
LAB_100d12a6:
  piVar3 = DAT_100d1444;
  FUN_100cd324(0x58,1,(short)*puVar6,(short)*DAT_100d1444,*pbVar1,puVar7);
  uVar11 = DAT_100d146c;
  uVar14 = DAT_100d1428;
  uVar12 = (uint)*pbVar1;
  uVar8 = *puVar2;
  uVar18 = (uint)*puVar5;
  uVar16 = DAT_100d1468 | uVar23;
  *puVar2 = (ushort)(uVar8 - uVar12);
  FUN_1014c818(uVar16,uVar11,uVar14,uVar8 - uVar12 & 0xffff,uVar12,uVar18);
  *piVar3 = *piVar3 + 1;
LAB_100d1316:
  FUN_1014c818(DAT_100d1474 | uVar23,DAT_100d1478,DAT_100d1428,uVar17,uVar12,uVar18);
  return uVar17;
}


