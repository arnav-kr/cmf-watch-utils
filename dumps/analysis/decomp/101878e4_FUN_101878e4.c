// entry 0x101878e4  FUN_101878e4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_101878e4(int param_1,int *param_2,uint param_3)

{
  ushort uVar1;
  undefined2 uVar2;
  uint *puVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  int *piVar11;
  int iVar12;
  int iVar13;
  undefined4 uVar14;
  
  if ((param_1 == 0) || (param_2 == (int *)0x0)) {
    FUN_1014c818(DAT_10187b8c | (DAT_10187b84 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187b90);
    return 0xffffffff;
  }
  FUN_101be218(DAT_10187b94,0xffffffff);
  iVar5 = DAT_10187b98;
  iVar9 = *param_2;
  piVar11 = *(int **)(DAT_10187b98 + 4);
  for (piVar6 = piVar11; piVar6 != (int *)0x0; piVar6 = (int *)piVar6[0xf]) {
    if ((param_1 == *piVar6) && (piVar6[1] == *(int *)(iVar9 + 0xc))) {
      param_2[2] = piVar6[9];
      if (param_3 == 1) {
        piVar6[3] = piVar6[3] + 1;
      }
      iVar12 = piVar6[8];
      param_2[1] = iVar12;
      if (iVar12 == 0) goto LAB_1018792e;
      goto LAB_10187d54;
    }
  }
  param_2[1] = 0;
LAB_1018792e:
  puVar3 = DAT_10187b9c;
  iVar13 = (uint)*(ushort *)(iVar9 + 0x18) *
           (uint)*(ushort *)(iVar9 + 0x16) * (uint)*(ushort *)(iVar9 + 0x14);
  uVar7 = *DAT_10187b9c;
  iVar12 = uVar7 * (((uVar7 - 1) + iVar13) / uVar7);
  if (param_3 < 2) {
    for (; piVar11 != (int *)0x0; piVar11 = (int *)piVar11[0xf]) {
      if ((param_1 == *piVar11) && (piVar11[1] == *(int *)(iVar9 + 0xc))) {
        FUN_1014c818(DAT_10187d84 | (DAT_10187d60 - DAT_10187d5c) * 0x20 & 0xff00U,DAT_10187d88);
        if (param_3 != 0) {
          piVar11[3] = piVar11[3] + 1;
        }
        iVar9 = piVar11[8];
        goto LAB_10187966;
      }
    }
    if (iVar12 - *DAT_10187bb4 == 0) {
      iVar9 = FUN_10188234(0,iVar12,DAT_10187bb8);
      piVar11 = DAT_10187bdc;
      if (iVar9 == 0) {
        FUN_1014c818(DAT_10187bbc | (DAT_10187b84 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187bc0);
      }
      else {
        *DAT_10187bdc = *DAT_10187bdc + 1;
        piVar6 = (int *)FUN_101d782c(0,*puVar3,0x40,DAT_10187bb8);
        if (piVar6 != (int *)0x0) {
          piVar6[8] = iVar9;
          piVar6[2] = 0x40;
          goto LAB_10187afe;
        }
        FUN_10188380(0,iVar9);
        iVar5 = DAT_10187b84;
        *piVar11 = *piVar11 + -1;
        FUN_1014c818(DAT_10187bcc | (iVar5 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187bd0,
                     DAT_10187bb8,0x2d7,iVar12);
      }
    }
    else {
      piVar6 = (int *)FUN_101d782c(0,uVar7,iVar12 + 0x40,DAT_10187bb8);
      if (piVar6 != (int *)0x0) {
        piVar6[2] = iVar12 + 0x40;
        piVar6[8] = (int)(piVar6 + 0x10);
LAB_10187afe:
        iVar12 = *param_2;
        *piVar6 = param_1;
        iVar9 = piVar6[8];
        piVar6[1] = *(int *)(iVar12 + 0xc);
        uVar1 = *(ushort *)(iVar12 + 0x14);
        piVar6[3] = param_3;
        piVar6[4] = (uint)uVar1;
        piVar6[5] = (uint)*(ushort *)(iVar12 + 0x16);
        uVar1 = *(ushort *)(iVar12 + 0x1a);
        piVar6[6] = (uint)*(ushort *)(iVar12 + 0x18);
        piVar6[7] = (uint)uVar1;
        piVar6[0xf] = *(int *)(iVar5 + 4);
        *(int **)(iVar5 + 4) = piVar6;
        piVar6[9] = param_2[2];
        goto LAB_10187966;
      }
      FUN_1014c818(DAT_10187bd4 | (DAT_10187b84 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187bd8,iVar12
                  );
    }
    FUN_10186b34();
    param_2[1] = 0;
  }
  else {
    iVar9 = FUN_10186f50(param_1,param_2,iVar12,param_3);
LAB_10187966:
    param_2[1] = iVar9;
    if (iVar9 != 0) {
      iVar5 = FUN_101d7840();
      if (iVar5 != 0) {
        iVar5 = *param_2;
        FUN_1014c818(DAT_10187ba0 | (DAT_10187b84 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187ba4,
                     *(undefined4 *)(iVar5 + 4),*(undefined4 *)(iVar5 + 0xc),
                     *(undefined2 *)(iVar5 + 0x14),*(undefined2 *)(iVar5 + 0x16),param_2[1]);
      }
      uVar7 = *(uint *)(*param_2 + 0xc);
      if (*(int *)(param_1 + 0xa0) != 0) {
        iVar5 = 0;
        uVar8 = *(int *)(param_1 + 0xa4) - 1;
        uVar10 = uVar8 >> 1;
        while( true ) {
          iVar9 = uVar10 * 0x3c + *(int *)(param_1 + 0xa0);
          if (uVar7 < *(uint *)(iVar9 + 0x2c)) {
            uVar8 = uVar10 - 1;
          }
          else {
            if (uVar7 <= *(uint *)(iVar9 + 0x30)) {
              iVar5 = uVar7 - *(uint *)(iVar9 + 0x2c);
              uVar14 = *(undefined4 *)(*(int *)(iVar9 + 0x34) + iVar5 * 4);
              iVar5 = *(int *)(*(int *)(iVar9 + 0x38) + iVar5 * 4);
              if (0 < (int)uVar10) goto LAB_101879e8;
              goto LAB_101879e4;
            }
            iVar5 = uVar10 + 1;
          }
          if ((int)uVar8 < iVar5) break;
          uVar10 = (int)(iVar5 + uVar8) / 2;
        }
      }
      uVar14 = 0;
      iVar5 = 0;
LAB_101879e4:
      iVar9 = param_1 + 0x2c;
LAB_101879e8:
      FUN_101072f0(iVar9,uVar14,0);
      if (iVar5 < 1) {
        iVar9 = FUN_10107238(iVar9,param_2[1],iVar13);
        piVar6 = (int *)0x0;
      }
      else {
        piVar6 = (int *)FUN_101d7820(0,iVar5,DAT_10187ba8);
        if (piVar6 == (int *)0x0) {
          FUN_1014c818(DAT_10187bac | (DAT_10187b84 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187bb0);
          FUN_10185db0();
          FUN_10186b34();
          return 0xffffffff;
        }
        iVar9 = FUN_10107238(iVar9,piVar6,iVar5);
        if (iVar9 < iVar5) {
          FUN_1014c818(DAT_10187d68 | (DAT_10187d60 - DAT_10187d5c) * 0x20 & 0xff00U,DAT_10187d64,
                       iVar9);
        }
        else if (iVar9 == iVar5) {
          if (*piVar6 == DAT_10187d74) {
            uVar2 = *(undefined2 *)(*param_2 + 0x14);
            iVar9 = FUN_100ffcc4(piVar6,iVar5,param_2[1],1,uVar2,0,0,uVar2,
                                 *(undefined2 *)(*param_2 + 0x16));
          }
          else {
            iVar9 = (*_DAT_00007090)(piVar6,param_2[1],iVar5,iVar13);
          }
        }
        if (iVar9 < iVar13) {
          uVar7 = (DAT_10187d60 - DAT_10187d5c) * 0x20 & 0xff00;
          FUN_1014c818(uVar7 | DAT_10187d78,DAT_10187d7c,*(undefined4 *)(param_1 + 0x94),uVar14);
          uVar4 = DAT_10187d8c;
          iVar12 = 0;
          uVar7 = uVar7 | DAT_10187d80;
          piVar11 = piVar6;
          do {
            FUN_1014c818(uVar7,uVar4,iVar12,(char)*piVar11,*(undefined1 *)((int)piVar11 + 1),
                         *(undefined1 *)((int)piVar11 + 2),*(undefined1 *)((int)piVar11 + 3),
                         (char)piVar11[1],*(undefined1 *)((int)piVar11 + 5),
                         *(undefined1 *)((int)piVar11 + 6),*(undefined1 *)((int)piVar11 + 7));
            iVar12 = iVar12 + 1;
            piVar11 = piVar11 + 2;
          } while (iVar12 != 4);
        }
        FUN_101d7826(0,piVar6);
      }
      if (iVar9 < iVar13) {
        iVar12 = *param_2;
        FUN_1014c818(DAT_10187d6c | (DAT_10187d60 - DAT_10187d5c) * 0x20 & 0xff00U,DAT_10187d70,
                     iVar9,piVar6,iVar5,*(undefined4 *)(iVar12 + 4),*(undefined4 *)(iVar12 + 0xc),
                     *(undefined2 *)(iVar12 + 0x14),*(undefined2 *)(iVar12 + 0x16),iVar13,param_2[1]
                     ,uVar14);
      }
      FUN_101bee00(param_2[1],iVar13);
LAB_10187d54:
      FUN_10185db0();
      return 0;
    }
  }
  FUN_1014c818(DAT_10187bc4 | (DAT_10187b84 - DAT_10187b88) * 0x20 & 0xff00U,DAT_10187bc8);
  FUN_10185db0();
  return 0xffffffff;
}


