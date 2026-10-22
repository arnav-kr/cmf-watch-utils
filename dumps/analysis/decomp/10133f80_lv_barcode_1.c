// 0x1023e6b7 lv_barcode_1 :: lv_barcode_update
// entry 0x10133f80  lv_barcode_1

undefined4 lv_barcode_1(undefined4 param_1,undefined4 param_2,uint param_3,undefined1 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int local_44;
  uint local_34 [2];
  
  lv_canvas_5(param_1,0,0xff);
  if (param_3 < 0x1f) {
    iVar1 = FUN_1013a14c(0x1e,DAT_1013410c);
    if (iVar1 == 0) {
      FUN_1013a098(3,DAT_1013411c,0x56,DAT_1013410c,DAT_10134118,DAT_10134114,DAT_10134110);
      FUN_101ba194(DAT_10134124,DAT_10134120,DAT_1013411c,0x56);
      FUN_101ba194(DAT_10134128);
      FUN_100eeb30(DAT_1013411c,0x56);
    }
    iVar2 = FUN_1013a14c(0x14,DAT_1013410c);
    if (iVar2 == 0) {
      FUN_1013a098(3,DAT_1013411c,0x58,DAT_1013410c,DAT_10134118,DAT_1013412c,DAT_10134110);
      FUN_101ba194(DAT_10134124,DAT_10134120,DAT_1013411c,0x58);
      FUN_101ba194(DAT_10134128);
      FUN_100eeb30(DAT_1013411c,0x58);
    }
    FUN_101bf3aa(iVar2,param_2,param_3);
    iVar3 = FUN_101cb018(iVar2,param_3,iVar1,param_4,local_34);
    if (iVar3 != 0) {
      puVar4 = (uint *)lv_canvas_3(param_1);
      uVar5 = puVar4[2];
      iVar6 = uVar5 + 8;
      uVar7 = (*puVar4 & 0x1fffff) >> 10;
      uVar9 = uVar7 / local_34[0];
      local_44 = (int)(uVar7 - uVar9 * local_34[0]) / 2;
      iVar3 = (int)(uVar7 + 7) >> 3;
      for (uVar7 = 0; uVar7 < local_34[0]; uVar7 = uVar7 + 1) {
        FUN_101cb056(iVar1,uVar7);
        for (iVar8 = 0; iVar8 < (int)uVar9; iVar8 = iVar8 + 1) {
          lv_canvas_1(param_1,local_44 + iVar8,0);
        }
        local_44 = local_44 + uVar9;
      }
      for (iVar8 = 1; uVar5 = uVar5 + iVar3, iVar8 < (int)(uint)(*(ushort *)((int)puVar4 + 2) >> 5);
          iVar8 = iVar8 + 1) {
        FUN_101bf3aa(uVar5 + 8,iVar6,iVar3);
      }
      lv_obj_pos_18(param_1);
      FUN_1013a160(iVar1,DAT_1013410c);
      FUN_1013a160(iVar2,DAT_1013410c);
      return 1;
    }
    FUN_1013a160(iVar1,DAT_1013410c);
    FUN_1013a160(iVar2,DAT_1013410c);
  }
  return 0;
}


