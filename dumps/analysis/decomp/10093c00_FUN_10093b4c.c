// entry 0x10093c00  FUN_10093b4c  (10093b4c)

void FUN_10093b4c(undefined4 param_1)

{
  undefined4 *puVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  uint local_204;
  undefined4 local_200;
  
  uVar3 = FUN_101c4c96();
  iVar4 = FUN_101c4c9a(param_1);
  bVar2 = lv_obj_tree_14(uVar3);
  if (iVar4 == 7) {
    iVar4 = *DAT_10093c10;
    uVar5 = (uint)bVar2;
    FUN_1014c818(DAT_10093c1c | (DAT_10093c18 - DAT_10093c14) * 0x20 & 0xff00U,DAT_10093c20,uVar5);
    if (uVar5 == 1) {
      FUN_100e6078();
      puVar1 = DAT_10093c24;
      FUN_101bacba(1);
      FUN_100c1f2c(6,0);
      local_200 = *puVar1;
      local_204 = 0;
      ux2sys_api_6(0,&local_204);
      local_200 = puVar1[1];
      local_204 = uVar5;
      ux2sys_api_6(0,&local_204);
      local_204 = 2;
      local_200 = puVar1[2];
      ux2sys_api_6(0,&local_204);
      local_204 = 3;
      local_200 = puVar1[3];
      ux2sys_api_6(0,&local_204);
      ux2sys_api_5(6,0);
    }
    else if (uVar5 == 2) {
      *DAT_10093c28 = 1;
      lv_tileview_1(*(undefined4 *)(iVar4 + 4),1,0);
      lv_obj_tree_16(*(undefined4 *)(iVar4 + 0x54));
      lv_obj_13(*(undefined4 *)(iVar4 + 4),1);
    }
  }
  return;
}


