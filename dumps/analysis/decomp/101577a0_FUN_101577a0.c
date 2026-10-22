// 0x10244acf FUN_101577a0 :: storage not init, update failed\n
// entry 0x101577a0  FUN_101577a0

int FUN_101577a0(int param_1,int param_2,int param_3,int param_4)

{
  undefined1 uVar1;
  byte bVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  undefined4 extraout_r1;
  undefined4 uVar13;
  int iVar14;
  int iVar15;
  uint uVar16;
  int iVar17;
  int iVar18;
  uint uVar19;
  uint local_74;
  int local_68;
  uint local_60;
  int local_50;
  uint local_4c;
  int local_48 [2];
  uint local_40;
  uint local_3c;
  
  iVar17 = param_1 + 0x2e4;
  iVar5 = FUN_1015a9e8(iVar17,*(undefined1 *)(param_3 + 0xe));
  uVar19 = (DAT_10157a28 - DAT_10157a24) * 0x20 & 0xff00;
  iVar15 = iVar5;
  FUN_1014c818(uVar19 | DAT_10157a2c,DAT_10157a30,param_3,*(undefined1 *)(param_3 + 0xe),iVar5);
  iVar6 = FUN_10156e44(*(undefined1 *)(param_3 + 0xf));
  if (iVar6 == 0) {
    FUN_1014c818(uVar19 | DAT_10157a34,DAT_10157a38);
    goto LAB_101577f4;
  }
  uVar1 = *(undefined1 *)(param_3 + 0xe);
  switch(iVar5) {
  case 1:
    uVar8 = uVar19 | 0x3b40000;
    uVar13 = DAT_10157a48;
    goto LAB_10157820;
  default:
    local_74 = 0;
    FUN_1014c818(uVar19 | DAT_10157a58,DAT_10157a5c,param_3,uVar1,iVar15);
    bVar3 = true;
    uVar8 = local_74;
    break;
  case 3:
    iVar15 = *(int *)(param_1 + 0x317);
    FUN_1014c818(uVar19 | DAT_10157a50,DAT_10157a54,param_3,uVar1,iVar15,
                 *(undefined4 *)(param_1 + 0x31b));
    bVar3 = true;
    uVar8 = *(uint *)(param_1 + 0x317);
    local_74 = *(uint *)(param_1 + 0x31b);
    break;
  case 5:
    iVar15 = *(int *)(param_1 + 0x317);
    FUN_1014c818(uVar19 | 0x3b80034,DAT_10157a4c,param_3,uVar1,iVar15,
                 *(undefined4 *)(param_1 + 0x31b));
    bVar3 = false;
    uVar8 = *(uint *)(param_1 + 0x317);
    local_74 = *(uint *)(param_1 + 0x31b);
    break;
  case 6:
  case 7:
    uVar8 = uVar19 | 0x3b00000;
    uVar13 = DAT_10157a3c;
LAB_10157820:
    FUN_1014c818(uVar8 | 0x32,uVar13,param_3,uVar1,iVar15);
    bVar3 = false;
    local_74 = 0;
    uVar8 = 0;
  }
  uVar7 = FUN_101d0972(iVar6);
  bVar2 = *(byte *)(param_2 + 10) >> 4;
  if (uVar7 == *(byte *)(param_2 + 10) >> 4) {
    if (((*(uint *)(param_1 + 8) & 9) != 0) && (bVar3)) {
      if ((*(uint *)(param_1 + 8) & 10) == 0 && bVar2 == 0) {
        FUN_1014c818(uVar19 | DAT_10157a60,DAT_10157a64,0,*(undefined1 *)(param_3 + 0xe),iVar15);
      }
      else {
        uVar7 = local_74 + *(int *)(param_3 + 0x10) & 0xfffff000;
        local_74 = uVar7 - *(int *)(param_3 + 0x10);
        FUN_101572b8(param_1,param_2,uVar7 - *(int *)(param_2 + 0xc));
        FUN_1014c818(uVar19 | DAT_10157a6c,DAT_10157a70,*(undefined1 *)(param_3 + 0xe),
                     *(undefined4 *)(param_1 + 0x31b),local_74);
        *(uint *)(param_1 + 0x31b) = local_74;
      }
    }
    if (iVar5 - 6U < 2) {
LAB_10157bc4:
      if (param_4 == 0) {
        return 0;
      }
      iVar15 = FUN_101573fc(param_1,param_3);
      if (iVar15 == 0) {
        FUN_1014c818(uVar19 | DAT_10157e7c,DAT_10157e80,param_3);
        uVar13 = 7;
        goto LAB_10157e68;
      }
      FUN_1014c818(uVar19 | DAT_10157dd0,DAT_10157dd4,param_3);
      FUN_101d09fa(iVar17,param_3,0xf,0,0,0);
    }
    else {
      FUN_101d09fa(iVar17,param_3,2,uVar8,local_74,0);
      FUN_1015aa38(iVar17,*(undefined1 *)(param_3 + 0xe),*(undefined4 *)(param_3 + 0x21));
      uVar13 = *(undefined4 *)(param_1 + 0x14);
      FUN_101bf3b2(local_48,0,0x10);
      iVar6 = *(int *)(param_3 + 0x14);
      iVar14 = *(int *)(param_3 + 0x18);
      uVar7 = local_74;
      FUN_1014c818(uVar19 | 0x2cc0036,DAT_10157a68,param_3,iVar6,iVar14,
                   *(undefined4 *)(param_3 + 0x10),uVar8,local_74);
      iVar15 = FUN_101d097a();
      iVar5 = FUN_10156e44(*(undefined1 *)(param_3 + 0xf));
      if (iVar5 == 0) {
        uVar8 = uVar19 | 0x2d40031;
        uVar13 = DAT_10157a38;
LAB_1015796c:
        FUN_1014c818(uVar8,uVar13);
      }
      else if ((uVar8 < *(uint *)(param_3 + 0x14)) && (local_74 < *(uint *)(param_3 + 0x18))) {
        iVar9 = FUN_101bf36a(param_3);
        iVar10 = 0;
        if (iVar9 != 0) {
          iVar10 = param_3;
        }
        iVar10 = FUN_101591c4(uVar13,iVar10);
        if (-1 < iVar10) {
          local_68 = *(int *)(param_3 + 0x18) - local_74;
          uVar13 = FUN_101d099c(uVar13);
          local_50 = 0x1000;
          FUN_101573bc(uVar13,DAT_10157dbc,&local_50);
          iVar18 = local_50 * (*(int *)(param_1 + 0x20) / local_50);
          thunk_FUN_1010d79c(0x1000);
          iVar9 = *(int *)(param_3 + 0x14);
          *(uint *)(param_1 + 0x3d4) = iVar10 + uVar8;
          *(uint *)(param_1 + 0x3d8) = iVar9 - uVar8;
          *(int *)(param_1 + 0x3dc) = iVar18;
          *(bool *)(param_1 + 0x3e0) = iVar6 == iVar14;
          *(undefined4 *)(param_1 + 0x3e4) = 0;
          FUN_1019a060(param_1 + 0x380);
          iVar10 = param_1 + 0x398;
          FUN_1019a060(iVar10);
          iVar9 = param_1 + 0x3b0;
          *(undefined4 *)(param_1 + 0x3b0) = 0;
          *(undefined4 *)(param_1 + 0x3b4) = 0;
          uVar13 = DAT_10157dc0;
          *(undefined4 *)(param_1 + 0x3b8) = 0;
          *(undefined4 *)(param_1 + 0x3bc) = 0;
          uVar13 = FUN_101be1a2(*(int *)(param_1 + 0x364) + 7U & 0xfffffff8,0x600,uVar13,param_1,0,0
                                ,3,0,0);
          *(undefined4 *)(param_1 + 0x3d0) = uVar13;
          FUN_101994cc(uVar13,DAT_10157dc4);
          bVar3 = false;
          bVar4 = false;
LAB_10157b34:
          if (local_68 < 1) goto code_r0x10157b3a;
          if ((!bVar3) && (FUN_101be202(param_1 + 0x380,0xffffffff), *(int *)(param_1 + 0x3e4) != 0)
             ) {
            FUN_101d09fa(iVar17,param_3,3,uVar8,local_74,1);
            iVar15 = *(int *)(param_1 + 0x3e4);
            if (iVar15 != 0) goto LAB_101579d2;
            goto LAB_10157ba8;
          }
          uVar11 = FUN_101ba470(iVar9);
          uVar16 = *(uint *)(param_1 + 0x374);
          uVar7 = uVar16;
          if (uVar16 >= uVar11) {
            uVar7 = uVar11;
          }
          bVar3 = uVar16 < uVar11 && uVar16 << 1 <= uVar11;
          if (iVar6 != iVar14) {
            if (local_48[0] != DAT_10157de0) goto code_r0x10157c8e;
            goto LAB_10157cf8;
          }
          local_60 = uVar7;
          if (local_68 <= (int)uVar7) goto LAB_10157c2e;
          if ((int)uVar7 < 0x1000) goto LAB_10157b34;
          local_60 = uVar7 & 0xfffff000;
LAB_10157c2e:
          FUN_101d09fa(iVar17,param_3,3,uVar8,local_74,!bVar4);
          uVar11 = ring_buffer_1(iVar9,*(undefined4 *)(param_1 + 0x370),local_60);
          FUN_10199f88(iVar10);
          uVar7 = DAT_10157ddc;
          uVar13 = DAT_10157dd8;
          if (local_60 == uVar11) {
            if (iVar6 == iVar14) {
              uVar13 = *(undefined4 *)(param_1 + 0x370);
              local_4c = local_60;
LAB_10157d22:
              iVar18 = FUN_101d097a();
              iVar12 = FUN_10156948(iVar5,*(int *)(param_3 + 0x10) + local_74,uVar13,local_4c);
              if (iVar12 != 0) {
                FUN_1014c818(uVar19 | 0x3740011,DAT_10157dfc,local_74);
                iVar15 = -5;
                goto LAB_101579d2;
              }
              iVar12 = FUN_101d097a();
              FUN_101be274(DAT_10157e78,uVar8,local_74,local_4c,iVar12 - iVar18);
              bVar4 = true;
              if (*(int *)(param_3 + 0x14) != *(int *)(param_3 + 0x18)) {
                local_60 = local_60 + 0x10;
              }
              uVar8 = uVar8 + local_60;
              local_74 = local_74 + local_4c;
              local_68 = local_68 - local_4c;
              goto LAB_10157b34;
            }
            iVar18 = FUN_101d097a();
            if (*(int *)(param_1 + 0x378) == 0) {
              uVar8 = uVar19 | DAT_10157e00;
              uVar13 = DAT_10157e04;
              goto LAB_1015796c;
            }
            local_4c = 0x8000;
            iVar12 = FUN_1015b6f0(*(undefined4 *)(param_1 + 0x370),local_60,
                                  *(int *)(param_1 + 0x378),&local_4c);
            uVar7 = DAT_10157e0c;
            uVar13 = DAT_10157e08;
            uVar11 = local_4c;
            if (iVar12 == 0) goto LAB_10157c6c;
            iVar12 = FUN_101d097a();
            FUN_101be274(DAT_10157e10,local_60,local_4c,(1 - iVar18) + iVar12);
            if (local_3c == local_4c) {
              uVar13 = *(undefined4 *)(param_1 + 0x378);
              local_48[0] = 0;
              goto LAB_10157d22;
            }
            uVar7 = uVar19 | 0x3680011;
            uVar13 = DAT_10157e14;
            uVar11 = local_4c;
          }
          else {
LAB_10157c6c:
            uVar7 = uVar19 | uVar7;
          }
          FUN_1014c818(uVar7,uVar13,uVar11);
LAB_10157cc8:
          iVar15 = -0xb;
          goto LAB_101579d2;
        }
        FUN_1014c818(uVar19 | 0x2e80011,DAT_10157a84,param_3);
      }
      else {
        FUN_1014c818(uVar19 | DAT_10157a74,DAT_10157a78,param_3,uVar8,local_74,
                     *(uint *)(param_3 + 0x14),*(uint *)(param_3 + 0x18),uVar7);
      }
LAB_101579ce:
      iVar15 = -0x16;
LAB_101579d2:
      FUN_1014c818(uVar19 | DAT_10157a7c,DAT_10157a80,param_3);
    }
    if (iVar15 == -5) {
      return -5;
    }
    if (iVar15 == -0xb) {
      return -0xb;
    }
  }
  else {
    FUN_1014c818(uVar19 | DAT_10157a40,DAT_10157a44,*(undefined1 *)(param_2 + 9),bVar2,uVar7);
LAB_101577f4:
    iVar15 = -0x16;
  }
  uVar13 = 0xe;
LAB_10157e68:
  FUN_101d09fa(iVar17,param_3,uVar13,0,0,0);
  return iVar15;
code_r0x10157b3a:
  iVar5 = FUN_101d097a();
  uVar8 = iVar5 + (1 - iVar15);
  FUN_1014c818(uVar19 | DAT_10157dcc,DAT_10157dc8,param_3,*(uint *)(param_3 + 0x14) >> 10,uVar8,
               *(uint *)(param_3 + 0x14) / uVar8);
  sched_14(*(undefined4 *)(param_1 + 0x3d0),extraout_r1,5000,0);
LAB_10157ba8:
  FUN_1015a314(param_1 + 0xb8,0xf,param_3);
  FUN_101d09fa(iVar17,param_3,6,0,0,0);
  goto LAB_10157bc4;
code_r0x10157c8e:
  if (uVar7 < 0x10) goto LAB_10157b34;
  ring_buffer_1(iVar9,local_48,0x10);
  FUN_10199f88(iVar10);
  if (local_48[0] != DAT_10157de0) {
    FUN_1014c818(uVar19 | DAT_10157de4,DAT_10157de8);
    FUN_101d0a72(iVar17);
    FUN_1015a8d4(iVar17);
    goto LAB_10157cc8;
  }
  uVar11 = DAT_10157df0;
  uVar13 = DAT_10157dec;
  if ((*(uint *)(param_1 + 0x374) < local_40) ||
     (uVar11 = DAT_10157df8, uVar13 = DAT_10157df4, *(uint *)(param_1 + 0x37c) < local_3c)) {
    FUN_1014c818(uVar19 | uVar11,uVar13);
    goto LAB_101579ce;
  }
  uVar7 = uVar7 - 0x10;
LAB_10157cf8:
  if (uVar7 < local_40) goto LAB_10157b34;
  local_60 = local_40;
  if (local_40 + 0x10 <= uVar7) {
    bVar3 = true;
  }
  goto LAB_10157c2e;
}


