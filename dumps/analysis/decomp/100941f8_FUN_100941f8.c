// entry 0x100941f8  FUN_100941f8  (100941f8)

void FUN_100941f8(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined4 uVar2;
  
  uVar2 = FUN_1013f5f0(param_2);
  lv_label_18(uVar2,3);
  lv_obj_pos_27(uVar2,DAT_10094320);
  FUN_1012bb14(uVar2,param_1 + 0xa0,0);
  lv_label_14(uVar2,PTR_DAT_10094324);
  FUN_101c5de4(uVar2,2,0,0x14);
  uVar2 = FUN_1013d388(param_2);
  lv_obj_pos_29(uVar2,0x50);
  FUN_101c7358(uVar2,0x7e0,0);
  puVar1 = PTR_LAB_10093770_1_10094330;
  FUN_101c7378(uVar2,0xff,0);
  FUN_101c5de4(uVar2,9,0x50,0x78);
  lv_event_11(uVar2,puVar1,7,0);
  uVar2 = FUN_1013f5f0(uVar2);
  FUN_101c74ce(uVar2,param_1 + 0x7c,0);
  FUN_101c74a4(uVar2,0xffff,0);
  FUN_101c5de4(uVar2,9,0);
  lv_label_14(uVar2,PTR_DAT_10094328);
  uVar2 = FUN_1013d388(param_2);
  lv_obj_pos_29(uVar2,0x50);
  FUN_101c7358(uVar2,0xf800,0);
  FUN_101c7378(uVar2,0xff,0);
  FUN_101c5de4(uVar2,9,0xffffffb0,0x78);
  lv_event_11(uVar2,puVar1,7,0);
  uVar2 = FUN_1013f5f0(uVar2);
  FUN_101c74ce(uVar2,param_1 + 0x7c,0);
  FUN_101c74a4(uVar2,0xffff,0);
  FUN_101c5de4(uVar2,9,0);
  lv_label_14(uVar2,PTR_DAT_1009432c);
  return;
}


