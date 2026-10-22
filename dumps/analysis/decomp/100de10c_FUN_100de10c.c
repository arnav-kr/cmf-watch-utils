// entry 0x100de10c  FUN_100de10c  (100de10c)

void FUN_100de10c(void)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  uint uVar4;
  byte *pbVar5;
  undefined4 uVar6;
  byte bVar7;
  byte bVar8;
  uint uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  uint uVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  byte *pbVar19;
  undefined4 local_a8;
  undefined1 auStack_a4 [60];
  undefined4 local_68;
  undefined1 auStack_64 [64];
  
  pbVar1 = DAT_100de3f4;
  uVar15 = 0;
  uVar17 = (DAT_100de3c0 - DAT_100de3bc) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100de3c4 | uVar17,DAT_100de3c8,DAT_100de3f4[0x98]);
  bVar7 = pbVar1[0x98];
  pbVar1[0x9c] = 0;
  pbVar1[0x9d] = 0;
  pbVar1[0x9e] = 0;
  pbVar1[0x9f] = 0;
  if (bVar7 == 0) {
    return;
  }
  FUN_101be218(pbVar1 + 0x84,0xffffffff);
  bVar7 = pbVar1[0x98];
  if (bVar7 == 2) {
    uVar9 = 0;
    uVar13 = 0;
    bVar7 = pbVar1[0xa4];
    bVar3 = pbVar1[8];
    bVar8 = pbVar1[0xa5];
    pbVar19 = pbVar1;
    for (; uVar15 < bVar7; uVar15 = uVar15 + 1) {
      uVar4 = 0;
      pbVar5 = pbVar1;
      do {
        if (bVar3 <= uVar4) goto LAB_100de250;
        pbVar5 = pbVar5 + 0x10;
        uVar4 = uVar4 + 1;
      } while (*(int *)pbVar5 != *(int *)(pbVar19 + 0xac));
      pbVar5 = pbVar1 + uVar4 * 0x10;
      uVar11 = *(undefined4 *)(pbVar5 + 4);
      uVar6 = *(undefined4 *)(pbVar5 + 8);
      uVar10 = *(undefined4 *)(pbVar5 + 0xc);
      *(undefined4 *)(pbVar1 + uVar13 * 0x10 + 0xac) = *(undefined4 *)pbVar5;
      *(undefined4 *)(pbVar1 + uVar13 * 0x10 + 0xb0) = uVar11;
      *(undefined4 *)(pbVar1 + uVar13 * 0x10 + 0xb4) = uVar6;
      *(undefined4 *)(pbVar1 + uVar13 * 0x10 + 0xb8) = uVar10;
      if (uVar15 == bVar8) {
        uVar9 = uVar13;
      }
      uVar13 = uVar13 + 1 & 0xff;
LAB_100de250:
      pbVar19 = pbVar19 + 0x10;
    }
    if (uVar13 == 0) {
      FUN_1014c818(DAT_100de3d4 | uVar17,DAT_100de3d8,0);
      FUN_100ddfa4(DAT_100de3e0,DAT_100de3dc);
      iVar12 = 0;
      goto LAB_100de1da;
    }
    FUN_101bf3aa(DAT_100de3ec + -0x9c,DAT_100de3ec,0x70);
    pbVar1[8] = (byte)uVar13;
    uVar11 = *(undefined4 *)(pbVar1 + uVar9 * 0x10 + 0x10);
    bVar7 = pbVar1[uVar9 * 0x10 + 0x14];
    pbVar1[9] = (char)uVar9 - (byte)uVar13 * (char)(uVar9 / uVar13);
    *(undefined4 *)(pbVar1 + 0xc) = uVar11;
    pbVar1[10] = bVar7;
    bVar7 = FUN_100dc534();
    uVar11 = DAT_100de3cc;
    pbVar1[0xb] = bVar7;
    uVar6 = DAT_100de3f0;
  }
  else if (bVar7 == 3) {
    iVar16 = *(int *)(pbVar1 + 0xbc);
    iVar18 = *(int *)(pbVar1 + 0xac);
    if ((iVar16 != 0) && (iVar12 = FUN_100de874(iVar16,iVar18 != 0), iVar12 != 0))
    goto LAB_100de1da;
    bVar7 = pbVar1[8];
    uVar15 = (uint)bVar7;
    if (iVar18 != 0) {
      if (iVar16 == 0) {
        if (uVar15 < 2) {
LAB_100de384:
          iVar12 = -5;
          goto LAB_100de1da;
        }
        uVar9 = 0;
        iVar12 = DAT_100de5d0;
        do {
          if (iVar18 == *(int *)(iVar12 + 0x10)) {
            if ((int)uVar9 < (int)(uVar15 - 1)) {
              iVar12 = (uVar9 + 1) * 0x10;
              FUN_101bf3a2(pbVar1 + iVar12,pbVar1 + iVar12 + 0x10,((uVar15 - 1) - uVar9) * 0x10);
            }
            uVar15 = (uint)(byte)(bVar7 - 1);
            pbVar1[8] = bVar7 - 1;
            if (uVar15 <= pbVar1[9]) {
              pbVar1[9] = bVar7 - 2;
            }
            uVar15 = (uint)pbVar1[9] - uVar15 * (pbVar1[9] / uVar15);
            uVar9 = uVar15 & 0xff;
            pbVar1[9] = (byte)uVar15;
            bVar7 = pbVar1[uVar9 * 0x10 + 0x14];
            *(undefined4 *)(pbVar1 + 0xc) = *(undefined4 *)(pbVar1 + uVar9 * 0x10 + 0x10);
            pbVar1[10] = bVar7;
            bVar7 = FUN_100dc534();
            uVar11 = DAT_100de5bc;
            pbVar1[0xb] = bVar7;
            FUN_100ddfa4(DAT_100de5d4,uVar11);
            FUN_101b9192(iVar18);
            goto LAB_100de19c;
          }
          uVar9 = uVar9 + 1;
          iVar12 = iVar12 + 0x10;
        } while (uVar9 < uVar15);
      }
      else {
        uVar9 = 0;
        pbVar19 = DAT_100de3f4;
        if (iVar16 == -1) {
          for (; uVar9 < uVar15; uVar9 = uVar9 + 1) {
            pbVar19 = pbVar19 + 0x10;
            if (*(int *)pbVar19 == -1) goto LAB_100de3ac;
          }
          if (6 < uVar15) goto LAB_100de384;
          pbVar19 = pbVar1 + uVar15 * 0x10 + 0x10;
          pbVar19[0] = 0xff;
          pbVar19[1] = 0xff;
          pbVar19[2] = 0xff;
          pbVar19[3] = 0xff;
          iVar12 = uVar15 * 0x10;
          pbVar19 = pbVar1 + iVar12 + 0x16;
          pbVar19[0] = 0;
          pbVar19[1] = 0;
          pbVar19[2] = 0;
          pbVar19[3] = 0;
          pbVar19 = pbVar1 + iVar12 + 0x1a;
          pbVar19[0] = 0;
          pbVar19[1] = 0;
          pbVar19[2] = 0;
          pbVar19[3] = 0;
          (pbVar1 + iVar12 + 0x1e)[0] = 0;
          (pbVar1 + iVar12 + 0x1e)[1] = 0;
          pbVar1[8] = pbVar1[8] + 1;
          uVar15 = uVar15 + 1 & 0xff;
        }
        else {
          for (; uVar9 < uVar15; uVar9 = uVar9 + 1) {
            pbVar19 = pbVar19 + 0x10;
            if (iVar16 == *(int *)pbVar19) goto LAB_100de5b4;
          }
        }
LAB_100de3ac:
        pbVar19 = DAT_100de3f4;
        for (uVar9 = 0; uVar9 < uVar15; uVar9 = uVar9 + 1) {
          pbVar19 = pbVar19 + 0x10;
          if (iVar18 == *(int *)pbVar19) {
            *(int *)(pbVar1 + uVar9 * 0x10 + 0x10) = iVar16;
            bVar3 = FUN_100dc6b0(iVar16);
            bVar7 = pbVar1[8];
            iVar14 = uVar9 * 0x10;
            pbVar1[uVar9 * 0x10 + 0x14] = bVar3;
            if (bVar7 != 0) {
              bVar7 = (char)uVar9 - bVar7 * (char)(uVar9 / bVar7);
            }
            pbVar1[9] = bVar7;
            uVar11 = *(undefined4 *)(pbVar1 + iVar14 + 0x10);
            pbVar1[10] = bVar3;
            *(undefined4 *)(pbVar1 + 0xc) = uVar11;
            bVar7 = FUN_100dc534(uVar11);
            pbVar1[0xb] = bVar7;
            iVar12 = FUN_100dc534(iVar16);
            FUN_100ddfa4(DAT_100de5c0,DAT_100de5bc);
            if ((iVar12 == 1) || (FUN_101b9192(iVar18), iVar12 != 0)) {
              local_a8 = 0;
              FUN_101bf3b2(auStack_a4,0,0x3c);
              local_68 = 0;
              FUN_101bf3b2(auStack_64,0,0x3c);
              FUN_100de99c(iVar16,&local_a8,0x40,1);
              FUN_100de99c(iVar16,&local_68,0x40,0);
              FUN_100dc4ec(&local_68);
              iVar18 = FUN_10107704(&local_a8,&local_68);
              FUN_1014c818(DAT_100de5c4 | uVar17,DAT_100de5c8,iVar18,&local_a8,&local_68);
              if (iVar18 == 0) {
                if (iVar12 == 1) {
                  FUN_100dea3c(iVar16,&local_a8,0x40,1);
                  FUN_100dea3c(iVar16,&local_68,0x40,0);
                  FUN_100dc4ec(&local_68);
                  uVar11 = FUN_10107704(&local_a8,&local_68);
                  FUN_1014c818(DAT_100de5cc | uVar17,DAT_100de5c8,uVar11,&local_a8,&local_68);
LAB_100de514:
                  FUN_100ded80(pbVar1 + iVar14 + 0x16,1);
                  goto LAB_100de19c;
                }
              }
              else if (iVar12 == 1) goto LAB_100de514;
            }
            else {
              FUN_100dc58c(iVar16,1);
            }
            iVar12 = 0;
            pbVar19 = pbVar1 + iVar14 + 0x16;
            pbVar19[0] = 0;
            pbVar19[1] = 0;
            pbVar19[2] = 0;
            pbVar19[3] = 0;
            pbVar19 = pbVar1 + iVar14 + 0x1a;
            pbVar19[0] = 0;
            pbVar19[1] = 0;
            pbVar19[2] = 0;
            pbVar19[3] = 0;
            pbVar1[iVar14 + 0x1e] = 0;
            goto LAB_100de1da;
          }
        }
      }
      iVar12 = -3;
      goto LAB_100de1da;
    }
    if (iVar16 == 0) {
      iVar12 = -0x12;
      goto LAB_100de1da;
    }
    pbVar19 = DAT_100de3f4;
    for (uVar9 = 0; uVar9 < uVar15; uVar9 = uVar9 + 1) {
      pbVar19 = pbVar19 + 0x10;
      if (iVar16 == *(int *)pbVar19) goto LAB_100de5b4;
    }
    if ((6 < uVar15) || (uVar9 = FUN_100dcfa8(), 5 < uVar9)) {
      iVar12 = -0xc;
      goto LAB_100de1da;
    }
    iVar12 = FUN_100dc534(iVar16);
    iVar18 = uVar15 * 0x10;
    pbVar19 = pbVar1 + iVar18 + 0x16;
    pbVar19[0] = 0;
    pbVar19[1] = 0;
    pbVar19[2] = 0;
    pbVar19[3] = 0;
    pbVar19 = pbVar1 + iVar18 + 0x1a;
    pbVar19[0] = 0;
    pbVar19[1] = 0;
    pbVar19[2] = 0;
    pbVar19[3] = 0;
    if (iVar12 == 1) {
      (pbVar1 + iVar18 + 0x1e)[0] = 0;
      (pbVar1 + iVar18 + 0x1e)[1] = 0;
    }
    else {
      pbVar1[iVar18 + 0x1e] = 0;
    }
    *(int *)(pbVar1 + iVar18 + 0x10) = iVar16;
    bVar2 = FUN_100dc6b0(iVar16);
    bVar3 = pbVar1[8];
    pbVar1[iVar18 + 0x14] = bVar2;
    bVar3 = bVar3 + 1;
    pbVar1[8] = bVar3;
    bVar8 = 0;
    if (bVar3 != 0) {
      bVar8 = bVar7 - bVar3 * (bVar7 / bVar3);
    }
    pbVar1[9] = bVar8;
    uVar11 = *(undefined4 *)(pbVar1 + iVar18 + 0x10);
    pbVar1[10] = bVar2;
    *(undefined4 *)(pbVar1 + 0xc) = uVar11;
    bVar7 = FUN_100dc534(uVar11);
    pbVar1[0xb] = bVar7;
    uVar6 = DAT_100de3f8;
    uVar11 = DAT_100de3cc;
    if (iVar12 == 0) {
      FUN_100dc58c(iVar16,0);
      uVar6 = DAT_100de3f8;
      uVar11 = DAT_100de3cc;
    }
  }
  else {
    if (bVar7 != 1) {
      iVar12 = -0x10;
      goto LAB_100de1da;
    }
    bVar7 = pbVar1[8];
    uVar9 = (uint)bVar7;
    pbVar19 = pbVar1;
    for (; uVar6 = DAT_100de3d0, uVar11 = DAT_100de3cc, uVar15 < uVar9; uVar15 = uVar15 + 1) {
      pbVar19 = pbVar19 + 0x10;
      if (*(int *)pbVar19 == *(int *)(pbVar1 + 0xa8)) {
        if (uVar9 != 0) {
          bVar7 = (char)uVar15 - bVar7 * (char)(uVar15 / uVar9);
        }
        pbVar1[9] = bVar7;
        bVar7 = pbVar1[uVar15 * 0x10 + 0x14];
        *(int *)(pbVar1 + 0xc) = *(int *)(pbVar1 + 0xa8);
        pbVar1[10] = bVar7;
        bVar7 = FUN_100dc534();
        pbVar1[0xb] = bVar7;
        uVar6 = DAT_100de3d0;
        uVar11 = DAT_100de3cc;
        break;
      }
    }
  }
  FUN_100ddfa4(uVar6,uVar11);
LAB_100de19c:
  iVar12 = 0;
LAB_100de1da:
  FUN_1014c818(DAT_100de3e4 | uVar17,DAT_100de3e8,iVar12);
  if (iVar12 == 0) {
    FUN_100dd16c();
    *pbVar1 = *pbVar1 | 0x20;
    FUN_101d7326(0x10,0,0);
    FUN_101d7326(0x11,0,0);
  }
  *(int *)(pbVar1 + 0x9c) = iVar12;
  pbVar1[0x98] = 0;
  FUN_100dc648();
  return;
LAB_100de5b4:
  iVar12 = -0x11;
  goto LAB_100de1da;
}


