// 0x1021fd78 FUN_100c9d40 :: [sleep_total_update]==================================
// entry 0x100c9d40  FUN_100c9d40

void FUN_100c9d40(void)

{
  int *piVar1;
  uint *puVar2;
  char cVar3;
  ushort uVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  char cVar8;
  int iVar9;
  bool bVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  uint *puVar14;
  uint uVar15;
  int *piVar16;
  uint uVar17;
  uint uVar18;
  byte bVar19;
  undefined4 uVar20;
  uint uVar21;
  int iVar22;
  int iVar23;
  uint local_3f8;
  undefined4 local_3f4;
  undefined4 local_3f0;
  uint auStack_3ec [2];
  undefined4 local_3e4;
  int local_3dc;
  int local_3d8;
  int local_3d4;
  int local_3d0;
  int aiStack_3cc [6];
  int aiStack_3b4 [4];
  uint auStack_3a4 [7];
  int aiStack_388 [5];
  uint auStack_374 [4];
  uint auStack_364 [6];
  byte local_34c;
  uint uStack_348;
  ushort auStack_344 [2];
  uint uStack_340;
  short sStack_33c;
  undefined1 auStack_33a [790];
  
  bVar19 = 0;
  FUN_101bf3b2(auStack_3ec,0,0x3c4);
  uVar18 = (DAT_100ca01c - DAT_100ca020) * 0x20 & 0xff00;
  local_3f8 = 0;
  local_3f4 = 0;
  local_3f0 = (uint)local_3f0._2_2_ << 0x10;
  FUN_1014c818(DAT_100ca024 | uVar18,DAT_100ca028);
  FUN_1014c818(DAT_100ca02c | uVar18,DAT_100ca028);
  FUN_101bf3aa(auStack_3ec,DAT_100ca030,0x3c4);
  FUN_100c14b4(&local_3f8);
  FUN_1014c818(DAT_100ca034 | uVar18,DAT_100ca038,local_3f4._2_2_,local_3f4 & 0xff,local_3f8 >> 0x18
               ,local_3f8 >> 0x10 & 0xff,local_3f8 >> 8 & 0xff,local_3f8 & 0xff);
  local_3f8 = local_3f8 & 0xff000000;
  iVar9 = FUN_101b5950(&local_3f8,*DAT_100ca03c * 0xe10 + DAT_100ca03c[1] * 0x3c);
  if ((((local_3e4 & 0xffff00) == 0) || (5 < local_3e4._1_1_)) || (5 < local_3e4._2_1_)) {
    FUN_101bf3b2(auStack_3ec,0,0x3c4);
  }
  else {
    piVar16 = aiStack_3b4;
    do {
      if (local_3e4._1_1_ <= bVar19) goto LAB_100c9e1e;
      iVar13 = *piVar16;
      bVar19 = bVar19 + 1;
      piVar1 = piVar16 + 5;
      piVar16 = piVar16 + 1;
    } while ((uint)(iVar9 << 1) <= (uint)(iVar13 + *piVar1));
    FUN_101bf3b2(auStack_3ec,0,0x3c4);
    FUN_1014c818(DAT_100ca040 | uVar18,DAT_100ca044);
LAB_100c9e1e:
    bVar19 = 0;
    puVar14 = auStack_374;
    do {
      if (local_3e4._2_1_ <= bVar19) goto LAB_100c9e5e;
      uVar11 = *puVar14;
      bVar19 = bVar19 + 1;
      puVar2 = puVar14 + 5;
      puVar14 = puVar14 + 1;
    } while ((uint)(iVar9 << 1) <= uVar11 + *puVar2);
    FUN_101bf3b2(auStack_3ec,0,0x3c4);
    FUN_1014c818(DAT_100ca048 | uVar18,DAT_100ca04c);
  }
LAB_100c9e5e:
  puVar2 = DAT_100ca054;
  puVar14 = DAT_100ca050;
  uVar11 = *DAT_100ca050;
  uVar17 = *DAT_100ca054;
  if ((uVar17 < uVar11 + 600) || (uVar11 + 0x15180 < uVar17)) {
    FUN_100c9a2c(auStack_3ec);
    uVar11 = DAT_100ca05c;
    uVar20 = DAT_100ca058;
    goto LAB_100c9e84;
  }
  uVar15 = local_3e4 >> 8 & 0xff;
  if ((uVar15 != 0) && (uVar11 <= auStack_3a4[uVar15])) {
    FUN_100c9a2c(auStack_3ec);
    uVar11 = DAT_100ca064;
    uVar20 = DAT_100ca060;
    goto LAB_100c9e84;
  }
  uVar15 = local_3e4 >> 0x10 & 0xff;
  if ((uVar15 != 0) && (uVar11 <= auStack_364[uVar15])) {
    FUN_100c9a2c(auStack_3ec);
    uVar11 = DAT_100ca06c;
    uVar20 = DAT_100ca068;
    goto LAB_100c9e84;
  }
  if (uVar17 + uVar11 < (uint)(iVar9 << 1)) {
    FUN_100c9a2c(auStack_3ec);
    uVar11 = DAT_100ca074;
    uVar20 = DAT_100ca070;
    goto LAB_100c9e84;
  }
  FUN_100c6f44(uVar11,&local_3f8);
  iVar9 = FUN_100c9cc0(local_3f8,local_3f4,local_3f0);
  if (iVar9 == 0) {
LAB_100c9f26:
    uVar17 = local_3e4 >> 8 & 0xff;
    uVar11 = *puVar14;
    uVar15 = (uint)*(byte *)(DAT_100ca084 + 0x20);
    if (uVar17 == 0) {
LAB_100ca08c:
      auStack_3ec[uVar17 + 0xe] = uVar11;
      auStack_3a4[uVar17 + 1] = *puVar2;
      uVar11 = uVar17 + 1 & 0xff;
      bVar6 = false;
      bVar5 = false;
      bVar10 = false;
      bVar7 = false;
      local_3e4._0_2_ = CONCAT11((char)(uVar17 + 1),(char)local_3e4);
      iVar13 = local_3d0;
      iVar12 = local_3d4;
      iVar22 = local_3dc;
      iVar23 = local_3d8;
      for (iVar9 = DAT_100ca1e4; iVar9 != DAT_100ca1e4 + uVar15 * 8; iVar9 = iVar9 + 8) {
        cVar8 = *(char *)(iVar9 + 0x2a);
        if (cVar8 == '\x01') {
          iVar22 = iVar22 + (uint)*(ushort *)(iVar9 + 0x28);
          bVar7 = true;
LAB_100ca104:
          aiStack_3cc[uVar11] = aiStack_3cc[uVar11] + (uint)*(ushort *)(iVar9 + 0x28);
        }
        else {
          if (cVar8 == '\x02') {
            bVar10 = true;
            iVar23 = iVar23 + (uint)*(ushort *)(iVar9 + 0x28);
            goto LAB_100ca104;
          }
          if (cVar8 == '\x03') {
            bVar5 = true;
            iVar12 = iVar12 + (uint)*(ushort *)(iVar9 + 0x28);
            goto LAB_100ca104;
          }
          if (cVar8 == '\x04') {
            iVar13 = iVar13 + (uint)*(ushort *)(iVar9 + 0x28);
            bVar6 = true;
          }
        }
      }
      if (bVar7) {
        local_3dc = iVar22;
      }
      if (bVar10) {
        local_3d8 = iVar23;
      }
      if (bVar5) {
        local_3d4 = iVar12;
      }
      if (bVar6) {
        local_3d0 = iVar13;
      }
    }
    else {
      uVar21 = auStack_3ec[uVar17 + 0x12];
      if (uVar11 < uVar21 + 0x1c20) {
        auStack_3ec[uVar17 + 0x12] = *puVar2;
        bVar6 = false;
        bVar5 = false;
        bVar10 = false;
        bVar7 = false;
        local_3d0 = (local_3d0 + uVar11) - uVar21;
        iVar9 = local_3d0;
        iVar12 = local_3d8;
        iVar22 = local_3dc;
        iVar23 = local_3d4;
        for (iVar13 = DAT_100ca084; iVar13 != DAT_100ca084 + uVar15 * 8; iVar13 = iVar13 + 8) {
          cVar8 = *(char *)(iVar13 + 0x2a);
          if (cVar8 == '\x01') {
            iVar22 = iVar22 + (uint)*(ushort *)(iVar13 + 0x28);
            bVar7 = true;
LAB_100c9fe6:
            aiStack_3cc[uVar17] = aiStack_3cc[uVar17] + (uint)*(ushort *)(iVar13 + 0x28);
          }
          else {
            if (cVar8 == '\x02') {
              bVar10 = true;
              iVar12 = iVar12 + (uint)*(ushort *)(iVar13 + 0x28);
              goto LAB_100c9fe6;
            }
            if (cVar8 == '\x03') {
              bVar5 = true;
              iVar23 = iVar23 + (uint)*(ushort *)(iVar13 + 0x28);
              goto LAB_100c9fe6;
            }
            if (cVar8 == '\x04') {
              iVar9 = iVar9 + (uint)*(ushort *)(iVar13 + 0x28);
              bVar6 = true;
            }
          }
        }
        if (bVar7) {
          local_3dc = iVar22;
        }
        if (bVar10) {
          local_3d8 = iVar12;
        }
        if (bVar5) {
          local_3d4 = iVar23;
        }
        if (bVar6) {
          local_3d0 = iVar9;
        }
      }
      else if (uVar17 < 4) goto LAB_100ca08c;
    }
    bVar5 = false;
    bVar10 = false;
    iVar13 = 0;
    uVar11 = *(uint *)(DAT_100ca084 + 0x24);
    iVar9 = DAT_100ca084;
    cVar8 = (char)local_3e4;
    bVar19 = local_34c;
    while ((uVar17 = (uint)bVar19, iVar13 < (int)uVar15 && (uVar17 < 100))) {
      if ((uVar17 != 0) && (iVar13 == 0)) {
        uVar4 = *(ushort *)(&local_3e4 + (uVar17 + 0x13) * 2);
        uVar21 = auStack_3ec[(uVar17 + 0x13) * 2 + 1] + (uint)uVar4;
        if (uVar21 < uVar11) {
          iVar12 = uVar17 + 0x14;
          auStack_3ec[iVar12 * 2 + 1] = uVar21;
          uVar20 = *(undefined4 *)(auStack_344 + (uVar17 - 1) * 4 + -2);
          uVar17 = uVar17 + 1 & 0xff;
          *(undefined1 *)((int)&local_3e4 + iVar12 * 8 + 2) = 0;
          *(ushort *)(&local_3e4 + iVar12 * 2) = ((short)uVar11 - uVar4) - (short)uVar20;
        }
      }
      cVar3 = *(char *)(iVar9 + 0x2a);
      iVar12 = uVar17 + 0x14;
      *(char *)((int)&local_3e4 + iVar12 * 8 + 2) = cVar3;
      if (cVar3 == '\x04') {
        cVar8 = cVar8 + '\x01';
      }
      auStack_3ec[iVar12 * 2 + 1] = *(uint *)(iVar9 + 0x24);
      bVar19 = (char)uVar17 + 1;
      *(undefined2 *)(&local_3e4 + iVar12 * 2) = *(undefined2 *)(iVar9 + 0x28);
      if (cVar3 == '\x04') {
        bVar5 = true;
      }
      bVar10 = true;
      iVar13 = iVar13 + 1;
      iVar9 = iVar9 + 8;
    }
    if (bVar10) {
      local_34c = bVar19;
    }
    if (bVar5) {
      local_3e4 = CONCAT31(local_3e4._1_3_,cVar8);
    }
  }
  else {
    uVar17 = *puVar2;
    uVar11 = *puVar14;
    iVar9 = uVar17 - uVar11;
    if (0x27d8 < iVar9 - 600U) goto LAB_100c9f26;
    uVar15 = (uint)local_3e4._2_1_;
    if (uVar15 < 4) {
      aiStack_388[uVar15] = iVar9;
      auStack_3ec[uVar15 + 0x1e] = uVar11;
      cVar8 = local_3e4._2_1_ + 1;
      auStack_364[uVar15 + 1] = uVar17;
      local_3e4._0_3_ = CONCAT12(cVar8,(undefined2)local_3e4);
    }
  }
  FUN_100c9a2c(auStack_3ec);
  FUN_1014c818(DAT_100ca078 | uVar18,DAT_100ca028);
  uVar11 = DAT_100ca080;
  uVar20 = DAT_100ca07c;
LAB_100c9e84:
  FUN_1014c818(uVar11 | uVar18,uVar20);
  return;
}


