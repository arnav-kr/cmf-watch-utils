// entry 0x100bf5c4  FUN_100bf5c4  (100bf5c4)

void FUN_100bf5c4(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  uint uVar6;
  uint uVar7;
  undefined4 *puVar8;
  undefined4 local_24;
  undefined4 uStack_20;
  
  uVar4 = param_1;
  local_24 = param_2;
  uStack_20 = param_3;
  iVar2 = FUN_101c4c96();
  uVar7 = *(uint *)(iVar2 + 0x10);
  puVar3 = (undefined4 *)FUN_101c4ca6(param_1);
  FUN_1014c818(((int)PTR_DAT_100bf664 - (int)PTR_LAB_100bf660) * 0x20 & 0xff00U | 0x660031,
               PTR_s_click_btn__d_100bf668,uVar7,PTR_LAB_100bf660,uVar4);
  if (*(byte *)(puVar3 + 0x28) != uVar7) {
    uVar6 = 0;
    *(char *)(puVar3 + 0x28) = (char)uVar7;
    puVar8 = puVar3;
    do {
      puVar8 = puVar8 + 1;
      if (uVar7 == uVar6) {
        uVar4 = lv_obj_tree_12(*puVar8,1);
        puVar5 = puVar3 + 5;
      }
      else {
        uVar4 = lv_obj_tree_12(*puVar8,1);
        puVar5 = puVar3 + 8;
      }
      uVar6 = uVar6 + 1;
      lv_img_2(uVar4,puVar5);
    } while (uVar6 != 3);
    cVar1 = *(char *)(puVar3 + 0x28);
    if (cVar1 == '\0') {
      local_24 = 0x1e;
    }
    else if (cVar1 == '\x01') {
      local_24 = 0x78;
    }
    else if (cVar1 == '\x02') {
      local_24 = 0x708;
    }
    else {
      local_24 = 0;
    }
    ux2sys_api_8(0x9c,&local_24);
  }
  return;
}


