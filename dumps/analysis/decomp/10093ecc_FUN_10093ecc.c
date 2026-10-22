// entry 0x10093ecc  FUN_10093ecc  (10093ecc)

void FUN_10093ecc(undefined4 param_1)

{
  uint *puVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint local_20c;
  uint local_208;
  uint local_204;
  
  uVar3 = FUN_101c4c96();
  iVar4 = FUN_101c4c9a(param_1);
  uVar5 = FUN_101c4ca2(param_1);
  bVar2 = lv_obj_tree_14(uVar3);
  if (iVar4 == 7) {
    iVar4 = *DAT_10093f9c;
    uVar6 = (uint)bVar2;
    FUN_1014c818(DAT_10093fa8 | (DAT_10093fa4 - DAT_10093fa0) * 0x20 & 0xff00U,DAT_10093fac,uVar6);
    if (uVar6 == 1) {
      FUN_100e644c();
      FUN_101bacb4(1);
      ux2sys_api_7(0,&local_20c);
      puVar1 = DAT_10093fb0;
      *DAT_10093fb0 = local_20c;
      puVar1[1] = local_208;
      local_20c = 0;
      local_208 = 0xf;
      puVar1[2] = local_204;
      ux2sys_api_6(0,&local_20c);
      local_208 = 0xf;
      local_20c = uVar6;
      ux2sys_api_6(0,&local_20c);
      local_20c = 2;
      local_208 = 0xf;
      ux2sys_api_6(0,&local_20c);
      local_20c = 3;
      local_208 = 0xf;
      ux2sys_api_6(0,&local_20c);
      local_20c = CONCAT31(local_20c._1_3_,bVar2);
      ux2sys_api_6(0x38,&local_20c);
      *DAT_10093fb4 = bVar2;
      lv_tileview_1(*(undefined4 *)(iVar4 + 4),1,0,0);
    }
    else if (uVar6 == 2) {
      FUN_101c4fd4(uVar5);
      FUN_101897fc();
    }
  }
  return;
}


