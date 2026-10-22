// entry 0x10094800  FUN_10094800  (10094800)

void FUN_10094800(undefined4 param_1)

{
  undefined4 *puVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  undefined1 uVar5;
  int iVar6;
  uint local_204;
  undefined4 local_200;
  
  uVar3 = FUN_101c4c96();
  iVar4 = FUN_101c4c9a(param_1);
  cVar2 = lv_obj_tree_14(uVar3);
  if (iVar4 == 7) {
    iVar6 = *DAT_10094988;
    FUN_1014c818(((int)PTR_DAT_1009498c - (int)PTR_LAB_10094990) * 0x20 & 0xff00U | 0x840031,
                 PTR_s__test_testmode_clicked_event_han_10094994,cVar2);
    iVar4 = *(int *)(iVar6 + 0xb0);
    if (cVar2 == '\x04') {
      if (iVar4 != 0) {
        FUN_100c242c(iVar4);
        *(undefined4 *)(iVar6 + 0xb0) = 0;
      }
      lv_obj_23(*(undefined4 *)(iVar6 + 4),1);
      FUN_10094334(iVar6,*(undefined4 *)(iVar6 + 0x54));
    }
    else {
      if (iVar4 == 0) {
        uVar3 = FUN_100c247c(0x10,0);
        *(undefined4 *)(iVar6 + 0xb0) = uVar3;
      }
      if (cVar2 == '\0') {
        lv_obj_23(*(undefined4 *)(iVar6 + 4),1);
        FUN_1009446c(iVar6,*(undefined4 *)(iVar6 + 0x54));
      }
      else if (cVar2 == '\x01') {
        lv_obj_23(*(undefined4 *)(iVar6 + 4),1);
        FUN_1008df30(iVar6,*(undefined4 *)(iVar6 + 0x54));
      }
      else if (cVar2 == '\x02') {
        lv_obj_23(*(undefined4 *)(iVar6 + 4),1);
        FUN_100941f8(iVar6,*(undefined4 *)(iVar6 + 0x54));
      }
      else if (cVar2 == '\x05') {
        FUN_100e6078();
        puVar1 = DAT_10094998;
        FUN_101bacba(1);
        FUN_100c1f2c(6,0);
        local_200 = *puVar1;
        local_204 = 0;
        ux2sys_api_6(0,&local_204);
        local_200 = puVar1[1];
        local_204 = 1;
        ux2sys_api_6(0,&local_204);
        local_204 = 2;
        local_200 = puVar1[2];
        ux2sys_api_6(0,&local_204);
        local_204 = 3;
        local_200 = puVar1[3];
        ux2sys_api_6(0,&local_204);
        local_204 = local_204 & 0xffffff00;
        ux2sys_api_6(0x38,&local_204);
        ux2sys_api_5(1,0);
      }
      else {
        if (cVar2 == '\x03') {
          uVar5 = 0x12;
          *(undefined1 *)(iVar6 + 0xb4) = 1;
        }
        else if (cVar2 == '\x06') {
          uVar5 = 0x1d;
        }
        else {
          if (cVar2 == '\a') {
            lv_obj_23(*(undefined4 *)(iVar6 + 4),1);
            FUN_1009458c(iVar6,*(undefined4 *)(iVar6 + 0x54));
            return;
          }
          if (cVar2 == '\b') {
            lv_obj_23(*(undefined4 *)(iVar6 + 4),1);
            FUN_100946c8(iVar6,*(undefined4 *)(iVar6 + 0x54));
            return;
          }
          if (cVar2 == '\t') {
            uVar5 = 0x1c;
          }
          else {
            if (cVar2 != '\n') {
              return;
            }
            uVar5 = 0x1e;
          }
        }
        uVar3 = *(undefined4 *)(iVar6 + 4);
        *DAT_1009499c = uVar5;
        lv_tileview_1(uVar3,uVar5,0);
      }
    }
  }
  return;
}


