// entry 0x10185134  FUN_10185134  (10185134)

/* WARNING: Type propagation algorithm not settling */

undefined4
FUN_10185134(undefined4 param_1,undefined4 *param_2,int param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,char param_9
            )

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  undefined4 *puVar6;
  int *piVar7;
  undefined4 *puVar8;
  undefined4 uVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int *piVar14;
  uint uVar15;
  undefined4 local_50;
  uint local_4c [3];
  undefined1 auStack_40 [16];
  int *local_30;
  int iStack_2c;
  
  local_4c[0] = 0;
  local_4c[1] = 0;
  local_4c[2] = 0;
  iVar2 = FUN_101d78b6();
  iVar3 = FUN_10184674(param_6,param_7,param_8,0,1);
  if (iVar3 == 0) {
    uVar4 = DAT_1018541c | (DAT_10185414 - DAT_10185418) * 0x20 & 0xff00U;
    uVar9 = DAT_10185420;
  }
  else {
    iVar5 = FUN_10186440(iVar3,param_1);
    if (iVar5 == 0) {
      return 0xffffffff;
    }
    local_50 = 0;
    puVar6 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10185424);
    uVar15 = (DAT_10185414 - DAT_10185418) * 0x20 & 0xff00;
    uVar4 = DAT_1018542c;
    if (puVar6 != (undefined4 *)0x0) {
      FUN_101bf3b2(puVar6 + 1,0,0x18);
      puVar6[3] = param_4;
      puVar6[5] = 0;
      puVar6[6] = 0;
      puVar6[4] = param_5;
      puVar6[2] = param_1;
      *puVar6 = 3;
      FUN_101d76f6(&local_50,puVar6);
      puVar1 = DAT_1018543c;
      if (param_2 == (undefined4 *)0x0) {
        uVar4 = uVar15 | DAT_1018544c;
LAB_101851fa:
        do {
          piVar7 = (int *)FUN_101869e8(iVar3,iVar5,local_4c,local_4c + 1);
          if (piVar7 != (int *)0x0) {
            piVar14 = (int *)*piVar7;
            iVar12 = *piVar14;
            if (iVar12 == 3) {
              if (piVar14[7] != 0) {
                FUN_10184f78(iVar3,param_1,0,piVar7,local_4c + 2,1,&local_50);
              }
            }
            else if (iVar12 == 4) {
              iVar12 = FUN_1018433c(local_50,piVar7);
              if (iVar12 != 0) {
                if (piVar7[2] == 0) {
                  iVar13 = *piVar7;
                  if ((((uint)*(ushort *)(iVar13 + 0x14) != *puVar1) ||
                      (*DAT_1018552c != (uint)*(ushort *)(iVar13 + 0x16))) ||
                     (iVar11 = iVar2, *(ushort *)(iVar13 + 0x18) != 2)) {
                    iVar11 = (uint)*(ushort *)(iVar13 + 0x18) *
                             (uint)*(ushort *)(iVar13 + 0x14) * (uint)*(ushort *)(iVar13 + 0x16) +
                             iVar2;
                  }
                  FUN_10184f30(param_1,local_4c + 2,iVar11);
                }
                puVar6 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10185530);
                if (puVar6 == (undefined4 *)0x0) {
                  FUN_1014c818(uVar4,DAT_10185534);
                }
                else {
                  *puVar6 = 2;
                  puVar6[1] = piVar7;
                  puVar6[2] = param_1;
                  puVar6[5] = iVar3;
                  puVar6[6] = 0;
                  *(undefined4 **)(iVar12 + 0x18) = puVar6;
                }
                goto LAB_101851fa;
              }
            }
            else {
              if (iVar12 != 1) goto LAB_101851fa;
              local_30 = piVar7;
              iStack_2c = iVar3;
              iVar12 = FUN_10184e4c(param_1,auStack_40,0,piVar14[6] + -1,&local_50);
              if ((0 < iVar12) && (local_30[1] == 0)) {
                iVar13 = *local_30;
                uVar10 = *DAT_10185438;
                FUN_10184f30(param_1,local_4c + 2,
                             iVar12 * uVar10 * (((uint)*(ushort *)(iVar13 + 0x16) *
                                                 (uint)*(ushort *)(iVar13 + 0x12) *
                                                 (uint)*(ushort *)(iVar13 + 0x10) + (uVar10 - 1) +
                                                iVar2) / uVar10));
              }
            }
            FUN_101d7876(piVar7);
            goto LAB_101851fa;
          }
        } while (local_4c[0] < *(uint *)(iVar5 + 0x10));
        puVar6 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10185424);
        uVar4 = DAT_10185430;
        if (puVar6 != (undefined4 *)0x0) {
          FUN_101bf3b2(puVar6 + 1,0,0x18);
          puVar6[3] = param_4;
          puVar6[2] = param_1;
          puVar6[4] = param_5;
          puVar6[6] = 0;
          *puVar6 = 4;
          FUN_101d76f6(&local_50,puVar6);
          uVar4 = local_4c[2];
          if (local_4c[2] == 0) {
            uVar4 = iVar2;
          }
          FUN_10186ce4(param_1,uVar4);
          if (param_9 == '\0') {
            FUN_101842e0(local_50);
            uVar4 = DAT_1018553c;
            uVar9 = DAT_10185538;
          }
          else {
            FUN_101843b8();
            uVar4 = DAT_1018553c;
            uVar9 = DAT_10185538;
          }
LAB_10185398:
          FUN_1014c818(uVar4 | uVar15,uVar9,param_1,local_4c[2]);
          return 0;
        }
      }
      else {
        uVar4 = uVar15 | DAT_10185450;
        puVar6 = param_2;
LAB_1018523e:
        if (param_2 + param_3 != puVar6) {
          puVar8 = puVar6 + 1;
          piVar7 = (int *)FUN_101866f8(iVar3,iVar5,*puVar6);
          puVar6 = puVar8;
          if (piVar7 != (int *)0x0) {
            piVar14 = (int *)*piVar7;
            iVar12 = *piVar14;
            if (iVar12 == 3) {
              if (piVar14[7] != 0) {
                FUN_10184f78(iVar3,param_1,0,piVar7,local_4c + 2,1,&local_50);
              }
            }
            else if (iVar12 == 4) {
              iVar12 = FUN_1018433c(local_50,piVar7);
              if (iVar12 != 0) {
                if (piVar7[2] == 0) {
                  iVar13 = *piVar7;
                  if ((((uint)*(ushort *)(iVar13 + 0x14) != *DAT_1018543c) ||
                      (*DAT_10185440 != (uint)*(ushort *)(iVar13 + 0x16))) ||
                     (iVar11 = iVar2, *(ushort *)(iVar13 + 0x18) != 2)) {
                    iVar11 = (uint)*(ushort *)(iVar13 + 0x18) *
                             (uint)*(ushort *)(iVar13 + 0x14) * (uint)*(ushort *)(iVar13 + 0x16) +
                             iVar2;
                  }
                  FUN_10184f30(param_1,local_4c + 2,iVar11);
                }
                puVar8 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10185424);
                if (puVar8 == (undefined4 *)0x0) {
                  FUN_1014c818(uVar4,DAT_10185428);
                }
                else {
                  *puVar8 = 2;
                  puVar8[1] = piVar7;
                  puVar8[2] = param_1;
                  puVar8[5] = iVar3;
                  puVar8[6] = 0;
                  *(undefined4 **)(iVar12 + 0x18) = puVar8;
                }
                goto LAB_1018523e;
              }
            }
            else {
              if (iVar12 != 1) goto LAB_1018523e;
              local_30 = piVar7;
              iStack_2c = iVar3;
              iVar12 = FUN_10184e4c(param_1,auStack_40,0,piVar14[6] + -1,&local_50);
              if ((0 < iVar12) && (local_30[1] == 0)) {
                iVar13 = *local_30;
                uVar10 = *DAT_10185438;
                FUN_10184f30(param_1,local_4c + 2,
                             iVar12 * uVar10 * (((uint)*(ushort *)(iVar13 + 0x16) *
                                                 (uint)*(ushort *)(iVar13 + 0x12) *
                                                 (uint)*(ushort *)(iVar13 + 0x10) + (uVar10 - 1) +
                                                iVar2) / uVar10));
              }
            }
            FUN_101d7876(piVar7);
          }
          goto LAB_1018523e;
        }
        puVar6 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10185424);
        uVar4 = DAT_10185434;
        if (puVar6 != (undefined4 *)0x0) {
          FUN_101bf3b2(puVar6 + 1,0,0x18);
          puVar6[3] = param_4;
          puVar6[2] = param_1;
          puVar6[4] = param_5;
          *puVar6 = 4;
          FUN_101d76f6(&local_50,puVar6);
          uVar4 = local_4c[2];
          if (local_4c[2] == 0) {
            uVar4 = iVar2;
          }
          FUN_10186ce4(param_1,uVar4);
          if (param_9 == '\0') {
            FUN_101842e0(local_50);
            uVar4 = DAT_10185448;
            uVar9 = DAT_10185444;
          }
          else {
            FUN_101843b8();
            uVar4 = DAT_10185448;
            uVar9 = DAT_10185444;
          }
          goto LAB_10185398;
        }
      }
    }
    uVar4 = uVar4 | uVar15;
    uVar9 = DAT_10185428;
  }
  FUN_1014c818(uVar4,uVar9);
  return 0xffffffff;
}


