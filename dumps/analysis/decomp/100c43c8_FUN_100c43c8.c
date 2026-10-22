// 0x1021eba2 FUN_100c43c8 :: %s update alarm count=%d\n
// entry 0x100c43c8  FUN_100c43c8

undefined4 FUN_100c43c8(short *param_1,int param_2)

{
  byte *pbVar1;
  byte *pbVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  short *psVar10;
  short *psVar11;
  undefined4 uVar12;
  undefined4 *puVar13;
  short *psVar14;
  undefined4 *puVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  undefined4 *puVar19;
  int iVar20;
  short *psVar21;
  int iVar23;
  int iVar24;
  int iVar25;
  undefined1 *puVar26;
  undefined4 local_54 [2];
  undefined2 local_4c [16];
  undefined4 auStack_2c [2];
  undefined4 *puVar9;
  short *psVar22;
  
  FUN_101bf3b2(DAT_100c4578,0,0xd4);
  puVar4 = DAT_100c459c;
  iVar18 = DAT_100c457c;
  uVar17 = 0;
  psVar14 = param_1 + 1;
  puVar5 = DAT_100c459c;
  puVar26 = DAT_100c45a0;
  do {
    *puVar5 = 0;
    *puVar26 = 0;
    if (param_2 == 0) {
      if (*(char *)((int)psVar14 + 9) != '\0') {
        psVar10 = (short *)(uVar17 * 0x2a + iVar18 + 0x932);
        psVar21 = psVar14;
        do {
          psVar22 = psVar21 + 2;
          psVar11 = psVar10 + 2;
          *(undefined4 *)psVar10 = *(undefined4 *)psVar21;
          psVar10 = psVar11;
          psVar21 = psVar22;
        } while (psVar22 != psVar14 + 0x14);
        *psVar11 = *psVar22;
        *(char *)(uVar17 * 0x2a + iVar18 + 0x939) = (char)uVar17;
LAB_100c443e:
        uVar17 = uVar17 + 1 & 0xff;
      }
    }
    else if (*param_1 == 0) {
      uVar17 = 0;
    }
    else if ((uint)*(byte *)((int)psVar14 + 7) == (int)puVar5 - (int)puVar4) {
      psVar10 = (short *)(uVar17 * 0x2a + iVar18 + 0x932);
      psVar21 = psVar14;
      do {
        psVar22 = psVar21 + 2;
        psVar11 = psVar10 + 2;
        *(undefined4 *)psVar10 = *(undefined4 *)psVar21;
        psVar10 = psVar11;
        psVar21 = psVar22;
      } while (psVar22 != psVar14 + 0x14);
      *psVar11 = *psVar22;
      *(undefined1 *)(uVar17 * 0x2a + iVar18 + 0x93b) = 1;
      goto LAB_100c443e;
    }
    iVar3 = DAT_100c457c;
    psVar14 = psVar14 + 0x15;
    puVar5 = puVar5 + 1;
    puVar26 = puVar26 + 1;
    if (psVar14 == param_1 + 0x6a) {
      *(short *)(DAT_100c457c + 0x930) = (short)uVar17;
      if (uVar17 != 0) {
        iVar18 = 0;
        iVar20 = iVar3;
        while (iVar18 < (int)(*(ushort *)(iVar3 + 0x930) - 1)) {
          iVar23 = iVar18 + 1;
          iVar24 = iVar20;
          for (iVar16 = iVar23; iVar16 < (int)(uint)*(ushort *)(iVar3 + 0x930); iVar16 = iVar16 + 1)
          {
            iVar25 = iVar18 * 0x2a + iVar3;
            pbVar1 = (byte *)(iVar24 + 0x961);
            pbVar2 = (byte *)(iVar24 + 0x960);
            iVar24 = iVar24 + 0x2a;
            if ((uint)*pbVar2 * 0x3c + (uint)*pbVar1 <
                (uint)*(byte *)(iVar25 + 0x936) * 0x3c + (uint)*(byte *)(iVar25 + 0x937)) {
              iVar18 = iVar16;
            }
          }
          puVar15 = local_54;
          iVar24 = iVar18 * 0x2a + iVar3;
          puVar19 = (undefined4 *)(iVar24 + 0x932);
          puVar8 = puVar15;
          do {
            uVar6 = *puVar19;
            uVar12 = puVar19[1];
            puVar19 = puVar19 + 2;
            *puVar8 = uVar6;
            puVar8[1] = uVar12;
            puVar8 = puVar8 + 2;
          } while (puVar19 != (undefined4 *)(iVar24 + 0x95a));
          puVar7 = (undefined4 *)(iVar20 + 0x932);
          *(undefined2 *)puVar8 = *(undefined2 *)puVar19;
          puVar8 = puVar7;
          puVar19 = (undefined4 *)(iVar18 * 0x2a + iVar3 + 0x932);
          do {
            puVar9 = puVar8 + 1;
            puVar13 = puVar19 + 1;
            *puVar19 = *puVar8;
            puVar8 = puVar9;
            puVar19 = puVar13;
          } while (puVar9 != (undefined4 *)(iVar20 + 0x95a));
          *(undefined2 *)puVar13 = *(undefined2 *)puVar9;
          do {
            uVar6 = *puVar15;
            uVar12 = puVar15[1];
            puVar15 = puVar15 + 2;
            *puVar7 = uVar6;
            puVar7[1] = uVar12;
            puVar7 = puVar7 + 2;
          } while (puVar15 != auStack_2c);
          iVar20 = iVar20 + 0x2a;
          *(undefined2 *)puVar7 = *(undefined2 *)puVar15;
          iVar18 = iVar23;
        }
      }
      uVar6 = DAT_100c458c;
      if (param_2 != 0) {
        uVar6 = DAT_100c4588;
      }
      FUN_1014c818(DAT_100c4590 | (DAT_100c4584 - DAT_100c4580) * 0x20 & 0xff00U,DAT_100c4594,uVar6,
                   *(undefined2 *)(iVar3 + 0x930));
      if (*(short *)(iVar3 + 0x930) != 0) {
        FUN_1014bd9c(DAT_100c4598);
      }
      return 0;
    }
  } while( true );
}


