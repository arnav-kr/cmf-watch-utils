// entry 0x10093700  FUN_10093610  (10093610)

void FUN_10093610(undefined4 param_1)

{
  char cVar1;
  char *pcVar2;
  undefined1 *puVar3;
  byte bVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 uVar7;
  undefined1 uVar8;
  ushort uVar9;
  ushort *puVar10;
  ushort uVar11;
  uint uVar12;
  uint in_fpscr;
  float fVar13;
  undefined4 uVar14;
  float fVar15;
  
  uVar5 = FUN_101c4c96();
  iVar6 = FUN_101c4c9a(param_1);
  bVar4 = lv_obj_tree_14(uVar5);
  if (iVar6 == 7) {
    iVar6 = *DAT_1009373c;
    uVar12 = (uint)bVar4;
    FUN_1014c818(DAT_10093748 | (DAT_10093744 - DAT_10093740) * 0x20 & 0xff00U,DAT_1009374c,uVar12);
    puVar10 = DAT_10093768;
    pcVar2 = DAT_10093750;
    if (uVar12 - 1 < 2) {
      cVar1 = *DAT_10093750;
      *(bool *)(DAT_10093754 + 0xe) = uVar12 == 1;
      if (cVar1 != '\0') {
        *pcVar2 = '\0';
      }
      *DAT_10093758 = 0;
      *DAT_1009375c = 0;
      FUN_10196510(0x65,0);
      puVar3 = DAT_10093760;
      if (*(char *)(iVar6 + 0xb4) == '\0') {
        FUN_100924ec();
        uVar8 = *puVar3;
      }
      else {
        uVar8 = 0x12;
        *DAT_10093760 = 0x12;
      }
      lv_tileview_1(*(undefined4 *)(iVar6 + 4),uVar8,0,0);
      return;
    }
    if (uVar12 == 3) {
      uVar9 = *(ushort *)(DAT_10093764 + 0xc);
      puVar10 = DAT_10093758;
    }
    else {
      if (uVar12 != 4) {
        if (uVar12 != 9) {
          return;
        }
        uVar9 = *DAT_1009375c;
        uVar11 = *DAT_10093758;
        if (uVar9 < uVar11) {
          uVar11 = uVar11 - uVar9;
        }
        else {
          uVar11 = uVar9 - uVar11;
        }
        *DAT_10093768 = uVar11;
        fVar13 = (float)FUN_10196fa8();
        fVar15 = (float)VectorSignedToFloat((uint)uVar11,(byte)(in_fpscr >> 0x16) & 3);
        uVar14 = VectorFloatToUnsigned(fVar15 * fVar13,3);
        *puVar10 = (ushort)uVar14;
        FUN_10196fe8();
        uVar12 = FUN_10197010();
        if ((*puVar10 == uVar12) && (uVar12 != 0)) {
          uVar14 = lv_obj_tree_9(uVar5);
          uVar7 = lv_obj_tree_12(uVar14,10);
          uVar14 = 0x7e0;
        }
        else {
          uVar14 = lv_obj_tree_9(uVar5);
          uVar7 = lv_obj_tree_12(uVar14,10);
          uVar14 = 0xffffffff;
        }
        FUN_101c74a4(uVar7,uVar14,0);
        uVar5 = lv_obj_tree_9(uVar5);
        uVar5 = lv_obj_tree_12(uVar5,10);
        lv_label_15(uVar5,DAT_1009376c,uVar12);
        return;
      }
      uVar9 = *(ushort *)(DAT_10093764 + 0xc);
      puVar10 = DAT_1009375c;
    }
    *puVar10 = uVar9;
  }
  return;
}


