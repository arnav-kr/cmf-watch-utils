// 0x10240006 lv_arc_3 :: value_update
// entry 0x1013bf44  lv_arc_3

void lv_arc_3(int param_1,uint param_2)

{
  undefined2 uVar1;
  short sVar2;
  short sVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  bool bVar12;
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_1013c034,0xa0,DAT_1013c038,DAT_1013c030,DAT_1013c02c,DAT_1013c028);
    FUN_101ba194(DAT_1013c040,DAT_1013c03c,DAT_1013c034,0xa0);
    FUN_101ba194(DAT_1013c044);
    FUN_100eeb30(DAT_1013c034,0xa0);
  }
  iVar6 = FUN_101c54c2(param_1,DAT_1013c048);
  if (iVar6 == 0) {
    FUN_1013a098(3,DAT_1013c034,0xa0,DAT_1013c038,DAT_1013c030,DAT_1013c050,DAT_1013c04c);
    FUN_101ba194(DAT_1013c040,DAT_1013c03c,DAT_1013c034,0xa0);
    FUN_101ba194(DAT_1013c044);
    FUN_100eeb30(DAT_1013c034,0xa0);
  }
  uVar8 = (uint)*(ushort *)(param_1 + 0x34);
  uVar10 = param_2;
  if (0x168 < param_2) {
    uVar10 = param_2 - 0x168;
  }
  sVar2 = *(ushort *)(param_1 + 0x34) - *(short *)(param_1 + 0x32);
  iVar6 = (int)sVar2;
  if (0x168 < param_2) {
    uVar10 = uVar10 & 0xffff;
  }
  sVar3 = (short)uVar10 - *(short *)(param_1 + 0x32);
  iVar9 = (int)sVar3;
  if (iVar6 < 0) {
    iVar6 = (int)(short)(sVar2 + 0x168);
  }
  if (iVar9 < 0) {
    iVar9 = (int)(short)(sVar3 + 0x168);
  }
  iVar7 = iVar9 - iVar6;
  if (iVar7 < 0) {
    iVar7 = -iVar7;
  }
  if (iVar7 < 0xb5) {
    uVar11 = uVar10;
    if ((iVar9 < iVar6) || (uVar11 = uVar8, uVar8 = uVar10, iVar6 < iVar9)) {
      lv_arc(param_1,uVar11,uVar8,0);
    }
  }
  else {
    lv_obj_pos_18(param_1);
  }
  *(short *)(param_1 + 0x34) = (short)uVar10;
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_1013be10,0x337,DAT_1013be14,DAT_1013be0c,DAT_1013be08,DAT_1013be04);
    FUN_101ba194(DAT_1013be1c,DAT_1013be18,DAT_1013be10,0x337);
    FUN_101ba194(DAT_1013be20);
    FUN_100eeb30(DAT_1013be10,0x337);
  }
  iVar6 = FUN_101c54c2(param_1,DAT_1013be24);
  if (iVar6 == 0) {
    FUN_1013a098(3,DAT_1013be10,0x337,DAT_1013be14,DAT_1013be0c,DAT_1013be2c,DAT_1013be28);
    FUN_101ba194(DAT_1013be1c,DAT_1013be18,DAT_1013be10,0x337);
    FUN_101ba194(DAT_1013be20);
    FUN_100eeb30(DAT_1013be10,0x337);
  }
  iVar6 = (int)*(short *)(param_1 + 0x36);
  if (iVar6 != -0x8000) {
    uVar8 = (uint)*(ushort *)(param_1 + 0x32);
    iVar9 = (int)*(short *)(param_1 + 0x34);
    bVar12 = *(ushort *)(param_1 + 0x34) < uVar8;
    if (bVar12) {
      iVar9 = iVar9 + 0x168;
    }
    uVar10 = (*(byte *)(param_1 + 0x3c) & 7) >> 1;
    if (bVar12) {
      iVar9 = (int)(short)iVar9;
    }
    if (uVar10 == 1) {
      iVar7 = (int)*(short *)(param_1 + 0x38) + (int)*(short *)(param_1 + 0x3a);
      uVar11 = (uVar8 + iVar9) - ((int)(uVar8 + iVar9) >> 0x1f);
      uVar10 = (uVar11 & 0x1ffff) >> 1;
      if (iVar6 < (iVar7 - (iVar7 >> 0x1f)) * 0x8000 >> 0x10) {
        uVar5 = FUN_101cc2b6(iVar6,(int)*(short *)(param_1 + 0x38),iVar7 / 2,uVar8,
                             (int)(uVar11 * 0x8000) >> 0x10);
        lv_arc_1(param_1,uVar5);
      }
      else {
        uVar8 = FUN_101cc2b6();
        uVar5 = (undefined2)uVar8;
        lv_arc_1(param_1,uVar10);
        uVar10 = uVar8 & 0xffff;
      }
      lv_arc_2(param_1,uVar10);
    }
    else {
      if (uVar10 == 2) {
        uVar5 = FUN_101cc2b6(iVar6,(int)*(short *)(param_1 + 0x38),(int)*(short *)(param_1 + 0x3a),
                             iVar9,uVar8);
        uVar1 = *(undefined2 *)(param_1 + 0x34);
        uVar4 = uVar5;
      }
      else {
        if (uVar10 != 0) {
          FUN_1013a098(2,DAT_1013be10,0x35b,DAT_1013be14,DAT_1013be30,3);
          return;
        }
        uVar5 = FUN_101cc2b6(iVar6,(int)*(short *)(param_1 + 0x38),(int)*(short *)(param_1 + 0x3a),
                             uVar8,iVar9);
        uVar4 = *(undefined2 *)(param_1 + 0x32);
        uVar1 = uVar5;
      }
      FUN_101ccc6a(param_1,uVar4,uVar1);
    }
    *(undefined2 *)(param_1 + 0x44) = uVar5;
  }
  return;
}


