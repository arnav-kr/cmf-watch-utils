// entry 0x100930f0  FUN_100930f0  (100930f0)

void FUN_100930f0(undefined4 param_1)

{
  undefined1 *puVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined1 uVar6;
  uint uVar7;
  uint local_20c [126];
  
  uVar3 = FUN_101c4c96();
  iVar4 = FUN_101c4c9a(param_1);
  bVar2 = lv_obj_tree_14(uVar3);
  if (iVar4 == 7) {
    iVar4 = *DAT_100931fc;
    uVar7 = (uint)bVar2;
    FUN_1014c818(DAT_10093208 | (DAT_10093204 - DAT_10093200) * 0x20 & 0xff00U,DAT_1009320c,uVar7);
    if (uVar7 == 1) {
      bVar2 = *DAT_10093210;
      *DAT_10093210 = bVar2 ^ 1;
      if (bVar2 == 1) {
        local_20c[0] = 0;
        ux2sys_api_6(0x2a,local_20c);
        FUN_101c7358(uVar3,0x7e0,0);
        uVar5 = lv_obj_tree_12(uVar3,0);
        uVar3 = DAT_10093218;
      }
      else {
        local_20c[0] = uVar7;
        ux2sys_api_6(0x2a,local_20c);
        FUN_101c7358(uVar3,0xf800,0);
        uVar5 = lv_obj_tree_12(uVar3,0);
        uVar3 = DAT_10093214;
      }
      lv_label_14(uVar5,uVar3);
    }
    else if (uVar7 - 2 < 2) {
      *(bool *)(DAT_1009321c + 0xb) = uVar7 == 2;
      *DAT_10093210 = 0;
      local_20c[0] = 0;
      ux2sys_api_6(0x2a,local_20c);
      uVar3 = lv_obj_tree_9(uVar3);
      uVar5 = lv_obj_tree_12(uVar3,1);
      FUN_101c7358(uVar5,0x7e0,0);
      uVar3 = lv_obj_tree_12(uVar3,1);
      uVar3 = lv_obj_tree_12(uVar3,0);
      lv_label_14(uVar3,DAT_10093218);
      puVar1 = DAT_10093220;
      if (*(char *)(iVar4 + 0xb4) == '\0') {
        FUN_100924ec();
        uVar6 = *puVar1;
      }
      else {
        uVar6 = 0x12;
        *DAT_10093220 = 0x12;
      }
      lv_tileview_1(*(undefined4 *)(iVar4 + 4),uVar6,0,0);
    }
  }
  return;
}


