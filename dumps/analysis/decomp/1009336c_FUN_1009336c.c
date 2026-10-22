// entry 0x1009336c  FUN_1009336c  (1009336c)

void FUN_1009336c(undefined4 param_1)

{
  char cVar1;
  byte *pbVar2;
  char *pcVar3;
  undefined1 *puVar4;
  byte bVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 uVar8;
  undefined1 uVar9;
  uint uVar10;
  
  uVar6 = FUN_101c4c96();
  iVar7 = FUN_101c4c9a(param_1);
  bVar5 = lv_obj_tree_14(uVar6);
  if (iVar7 == 7) {
    iVar7 = *DAT_10093488;
    uVar10 = (uint)bVar5;
    FUN_1014c818(DAT_10093494 | (DAT_10093490 - DAT_1009348c) * 0x20 & 0xff00U,DAT_10093498,uVar10);
    uVar8 = lv_obj_tree_9(uVar6);
    pbVar2 = DAT_1009349c;
    if (uVar10 == 1) {
      bVar5 = *DAT_1009349c;
      *DAT_1009349c = bVar5 ^ 1;
      if ((bVar5 ^ 1) == 0) {
        FUN_101c7358(uVar6,0x7e0,0);
        uVar8 = lv_obj_tree_12(uVar6,0);
        uVar6 = DAT_100934a8;
      }
      else {
        FUN_101c7358(uVar6,0xf800,0);
        uVar8 = lv_obj_tree_12(uVar6,0);
        uVar6 = DAT_100934a0;
      }
      pcVar3 = DAT_100934a4;
      lv_label_14(uVar8,uVar6);
      if ((*pcVar3 == '\0') || (*pbVar2 != 0)) {
        FUN_101b59de(6,0xff);
      }
      else {
        FUN_100c1f2c(6);
      }
      *pcVar3 = *pcVar3 + '\x01';
    }
    else if (uVar10 - 2 < 2) {
      FUN_100c1f2c(6,0);
      *DAT_100934a4 = '\0';
      *DAT_1009349c = 0;
      uVar6 = lv_obj_tree_12(uVar8,1);
      FUN_101c7358(uVar6,0x7e0,0);
      uVar6 = lv_obj_tree_12(uVar8,1);
      uVar6 = lv_obj_tree_12(uVar6,0);
      lv_label_14(uVar6,DAT_100934a8);
      cVar1 = *(char *)(iVar7 + 0xb4);
      *(bool *)(DAT_100934ac + 0xd) = uVar10 == 2;
      puVar4 = DAT_100934b0;
      if (cVar1 == '\0') {
        FUN_100924ec();
        uVar9 = *puVar4;
      }
      else {
        uVar9 = 0x12;
        *DAT_100934b0 = 0x12;
      }
      lv_tileview_1(*(undefined4 *)(iVar7 + 4),uVar9,0,0);
      return;
    }
  }
  return;
}


