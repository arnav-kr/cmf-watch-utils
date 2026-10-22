// 0x10245217 FUN_10157e84 :: same file %s,needn't do upgrade
// entry 0x10157e84  FUN_10157e84

int FUN_10157e84(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  int iVar16;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  
  uVar10 = (DAT_10158174 - DAT_10158170) * 0x20 & 0xff00;
  FUN_1014c818(DAT_10158178 | uVar10,DAT_1015817c,*(undefined4 *)(param_1 + 0xac));
  iVar13 = 0;
  local_34 = 2;
  local_3c = 0;
  local_38 = 0;
  uVar9 = DAT_10158180 | uVar10;
  local_40 = 0;
LAB_10157ebc:
  iVar11 = param_1 + 0xb8;
  iVar12 = iVar11;
  for (iVar14 = 0; uVar5 = DAT_101581d8, iVar14 < *(int *)(param_1 + 0xac); iVar14 = iVar14 + 1) {
    iVar8 = iVar13;
    iVar16 = local_40;
    iVar1 = local_3c;
    iVar2 = local_38;
    if ((*(int *)(param_1 + 8) << 0x1e < 0) || (iVar4 = FUN_1015a480(iVar12), iVar4 != 0)) {
      iVar4 = partition_7(*(undefined1 *)(iVar12 + 0xe));
      if (iVar4 == 0) {
        FUN_1014c818(DAT_1015818c | uVar10,DAT_10158190,*(undefined1 *)(iVar12 + 0xe));
        if (-1 < *(int *)(param_1 + 8) << 0x1f) {
          return -0x16;
        }
        uVar5 = FUN_101bbde0();
        iVar4 = partition_6(*(undefined1 *)(iVar12 + 0xe));
        if ((uVar5 == *(byte *)(iVar12 + 0xe)) || (iVar4 == 0)) {
          FUN_1014c818(DAT_10158194 | uVar10,DAT_10158198);
          goto LAB_10157fa6;
        }
        FUN_1014c818(DAT_1015819c | uVar10,DAT_101581a0);
      }
      uVar5 = FUN_101bbdb8(iVar4);
      iVar16 = iVar14 * 0x25 + param_1;
      uVar15 = *(uint *)(iVar16 + 0xd0);
      if (uVar5 < uVar15) {
        FUN_1014c818(DAT_101581a4 | uVar10,DAT_101581a8,iVar4,uVar15,uVar5);
        return -0x16;
      }
      FUN_1014c818(uVar9,DAT_101581ac,iVar14,iVar12,*(undefined1 *)(iVar12 + 0xe),
                   *(undefined4 *)(iVar4 + 0x14));
      *(undefined4 *)(iVar16 + 200) = *(undefined4 *)(iVar4 + 0x14);
      iVar6 = partition_4(iVar4);
      iVar16 = iVar12;
      iVar1 = iVar4;
      if (((iVar6 == 0) &&
          (iVar6 = partition_3(iVar4), iVar8 = iVar12, iVar16 = local_40, iVar1 = local_3c,
          iVar2 = iVar4, iVar6 == 0)) &&
         (iVar4 = FUN_101577a0(param_1,iVar4,iVar12,0), iVar8 = iVar13, iVar2 = local_38, iVar4 != 0
         )) {
        return iVar4;
      }
    }
    else {
      FUN_1014c818(DAT_10158184 | uVar10,DAT_10158188,iVar12);
    }
LAB_10157fa6:
    local_38 = iVar2;
    local_3c = iVar1;
    local_40 = iVar16;
    iVar12 = iVar12 + 0x25;
    iVar13 = iVar8;
  }
  iVar12 = 0;
  uVar15 = uVar10 | DAT_101581d4;
  do {
    if (*(int *)(param_1 + 0xac) <= iVar12) {
      if ((local_40 != 0) && (local_3c != 0)) {
        iVar12 = FUN_101577a0(param_1,local_3c,local_40,1);
        if (iVar12 != 0) {
          return iVar12;
        }
        uVar3 = partition_6(*(undefined1 *)(local_40 + 0xe));
        FUN_101572b8(param_1,uVar3,0);
        FUN_1015a97c(param_1 + 0x2e4,*(undefined1 *)(local_40 + 0xe),1);
      }
      if ((iVar13 != 0) && (local_38 != 0)) {
        iVar12 = FUN_101577a0(param_1,local_38,iVar13,1);
        if (iVar12 != 0) {
          return iVar12;
        }
        uVar3 = partition_6(*(undefined1 *)(iVar13 + 0xe));
        FUN_101572b8(param_1,uVar3,0);
        FUN_1015a97c(param_1 + 0x2e4,*(undefined1 *)(iVar13 + 0xe),1);
      }
      iVar13 = 0;
      while( true ) {
        if (*(int *)(param_1 + 0xac) <= iVar13) {
          return 0;
        }
        if (*(byte *)(param_1 + 0xc6 + iVar13 * 0x25) - 10 < 3) break;
        iVar13 = iVar13 + 1;
      }
      FUN_101576f8(param_1);
      return 0;
    }
    iVar14 = partition_7(*(undefined1 *)(iVar11 + 0xe));
    if (iVar14 == 0) {
      FUN_1014c818(DAT_101581b0 | uVar10,DAT_10158190,*(undefined1 *)(iVar11 + 0xe));
      if (-1 < *(int *)(param_1 + 8) << 0x1f) break;
      uVar7 = FUN_101bbde0();
      iVar14 = partition_6(*(undefined1 *)(iVar11 + 0xe));
      if ((uVar7 != *(byte *)(iVar11 + 0xe)) && (iVar14 != 0)) {
        FUN_1014c818(DAT_101581b8 | uVar10,DAT_101581a0);
        goto LAB_101580d0;
      }
      FUN_1014c818(DAT_101581b4 | uVar10,DAT_10158198);
    }
    else {
LAB_101580d0:
      iVar8 = partition_4(iVar14);
      if ((iVar8 == 0) && (iVar14 = partition_3(iVar14), iVar14 == 0)) {
        if ((*(int *)(param_1 + 8) << 0x1e < 0) || (iVar14 = FUN_1015a480(iVar11), iVar14 != 0)) {
          iVar14 = FUN_101573fc(param_1,iVar11);
          if (iVar14 != 0) {
            FUN_1014c818(DAT_101581c0 | uVar10,DAT_101581c4,iVar11);
            FUN_101d09fa(param_1 + 0x2e4,iVar11,0xf,0,0,0);
            iVar12 = -1;
            goto LAB_1015812e;
          }
          if (*(code **)(param_1 + 0x10) != (code *)0x0) {
            (**(code **)(param_1 + 0x10))(*(undefined1 *)(iVar11 + 0xe));
          }
          FUN_1014c818(uVar15,DAT_101581d0,iVar11);
          FUN_101d09fa(param_1 + 0x2e4,iVar11,7,0,0,0);
        }
        else {
          FUN_1014c818(uVar10 | uVar5,DAT_101581bc,iVar11);
        }
      }
    }
    iVar12 = iVar12 + 1;
    iVar11 = iVar11 + 0x25;
  } while( true );
  iVar12 = -0x16;
LAB_1015812e:
  if (local_34 == 1) {
    return iVar12;
  }
  FUN_1014c818(DAT_101581c8 | uVar10,DAT_101581cc);
  FUN_101d0988(*(undefined4 *)(param_1 + 0x14));
  local_34 = 1;
  goto LAB_10157ebc;
}


