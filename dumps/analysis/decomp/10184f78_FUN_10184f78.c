// entry 0x10184f78  FUN_10184f78  (10184f78)

/* WARNING: Type propagation algorithm not settling */

undefined4
FUN_10184f78(int param_1,int param_2,int param_3,int *param_4,undefined4 *param_5,int param_6,
            undefined4 *param_7)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  int *piVar8;
  uint uVar9;
  int iVar10;
  uint local_4c [3];
  undefined1 auStack_40 [16];
  int *local_30;
  int iStack_2c;
  
  iVar10 = 0;
  local_4c[2] = *param_5;
  local_4c[0] = 0;
  local_4c[1] = 0;
  iVar1 = FUN_101d78b6();
  uVar7 = (DAT_1018510c - DAT_10185110) * 0x20 & 0xff00;
  if (param_1 == 0) {
    FUN_1014c818(DAT_10185114 | uVar7,DAT_10185118);
    uVar5 = 0xffffffff;
  }
  else {
    uVar7 = uVar7 | DAT_10185130;
LAB_10184fb2:
    do {
      piVar2 = (int *)FUN_101868bc(param_1,param_4,local_4c,local_4c + 1,param_2,param_3);
      if (piVar2 != (int *)0x0) {
        piVar8 = (int *)*piVar2;
        iVar4 = *piVar8;
        if (iVar4 == 3) {
          if (piVar8[7] != 0) {
            if (param_3 == 0) {
              uVar5 = *(undefined4 *)(*param_4 + 4);
            }
            else {
              uVar5 = 0xffffffff;
            }
            FUN_10184f78(param_1,param_2,uVar5,piVar2,local_4c + 2,param_6,param_7);
          }
        }
        else if (iVar4 == 4) {
          iVar4 = FUN_1018433c(*param_7,piVar2);
          if (iVar4 != 0) {
            if (piVar2[2] == 0) {
              iVar4 = *piVar2;
              if ((((uint)*(ushort *)(iVar4 + 0x14) != *DAT_10185120) ||
                  (*DAT_10185124 != (uint)*(ushort *)(iVar4 + 0x16))) ||
                 (iVar6 = iVar1, *(ushort *)(iVar4 + 0x18) != 2)) {
                iVar6 = (uint)*(ushort *)(iVar4 + 0x18) *
                        (uint)*(ushort *)(iVar4 + 0x14) * (uint)*(ushort *)(iVar4 + 0x16) + iVar1;
              }
              FUN_10184f30(param_2,local_4c + 2,iVar6);
            }
            if (param_6 != 0) {
              puVar3 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10185128);
              if (puVar3 == (undefined4 *)0x0) {
                FUN_1014c818(uVar7,DAT_1018512c);
              }
              else {
                if (param_2 == 0) {
                  *puVar3 = 1;
                }
                else {
                  puVar3[2] = param_2;
                  *puVar3 = 2;
                }
                puVar3[5] = param_1;
                puVar3[6] = 0;
                puVar3[1] = piVar2;
                FUN_101d76f6(param_7);
              }
            }
            goto LAB_10184fb2;
          }
        }
        else {
          if (iVar4 != 1) goto LAB_10184fb2;
          local_30 = piVar2;
          iStack_2c = param_1;
          if (param_6 != 0) {
            iVar10 = FUN_10184e4c(param_2,auStack_40,0,piVar8[6] + -1,param_7);
          }
          if ((0 < iVar10) && (local_30[1] == 0)) {
            iVar4 = *local_30;
            uVar9 = *DAT_1018511c;
            FUN_10184f30(param_2,local_4c + 2,
                         iVar10 * uVar9 * (((uint)*(ushort *)(iVar4 + 0x16) *
                                            (uint)*(ushort *)(iVar4 + 0x10) *
                                            (uint)*(ushort *)(iVar4 + 0x12) + (uVar9 - 1) + iVar1) /
                                          uVar9));
          }
        }
        FUN_101d7876(piVar2);
        goto LAB_10184fb2;
      }
    } while (local_4c[0] < *(uint *)(*param_4 + 0x1c));
    *param_5 = local_4c[2];
    uVar5 = 0;
  }
  return uVar5;
}


