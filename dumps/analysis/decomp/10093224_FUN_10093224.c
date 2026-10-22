// entry 0x10093224  FUN_10093224  (10093224)

void FUN_10093224(undefined4 param_1)

{
  char cVar1;
  undefined1 *puVar2;
  byte bVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined1 uVar8;
  int iVar9;
  uint uVar10;
  
  uVar4 = FUN_101c4c96();
  iVar5 = FUN_101c4c9a(param_1);
  bVar3 = lv_obj_tree_14(uVar4);
  if (iVar5 == 7) {
    iVar5 = *DAT_10093338;
    uVar10 = (uint)bVar3;
    FUN_1014c818(DAT_10093344 | (DAT_10093340 - DAT_1009333c) * 0x20 & 0xff00U,DAT_10093348,uVar10);
    uVar6 = lv_obj_tree_9(uVar4);
    if (uVar10 == 1) {
      iVar9 = *(int *)(iVar5 + 0xb0);
      *DAT_1009334c = bVar3;
      if (iVar9 == 0) {
        uVar7 = FUN_100c247c(0x10,0);
        *(undefined4 *)(iVar5 + 0xb0) = uVar7;
      }
      FUN_100c22f4(*(undefined4 *)(iVar5 + 0xb0),DAT_10093350);
      uVar7 = FUN_100c148c();
      *DAT_10093354 = uVar7;
      uVar7 = lv_obj_tree_12(uVar4,0);
      lv_label_14(uVar7,DAT_10093358);
      FUN_101c7358(uVar4,0xf800,0);
      lv_obj_14(uVar4,0x80);
      uVar4 = lv_obj_tree_12(uVar6,2);
      lv_obj_14(uVar4,0x80);
      uVar4 = lv_obj_tree_12(uVar6,3);
      lv_obj_14(uVar4,0x80);
      return;
    }
    if (uVar10 - 2 < 2) {
      uVar4 = lv_obj_tree_12(uVar6,1);
      FUN_101c7358(uVar4,0x7e0,0);
      uVar4 = lv_obj_tree_12(uVar6,1);
      uVar4 = lv_obj_tree_12(uVar4,0);
      lv_label_14(uVar4,DAT_1009335c);
      cVar1 = *(char *)(iVar5 + 0xb4);
      *DAT_10093360 = 0;
      *(bool *)(DAT_10093364 + 0xc) = uVar10 == 2;
      puVar2 = DAT_10093368;
      if (cVar1 == '\0') {
        FUN_100924ec();
        uVar8 = *puVar2;
      }
      else {
        uVar8 = 0x12;
        *DAT_10093368 = 0x12;
      }
      lv_tileview_1(*(undefined4 *)(iVar5 + 4),uVar8,0,0);
      return;
    }
  }
  return;
}


