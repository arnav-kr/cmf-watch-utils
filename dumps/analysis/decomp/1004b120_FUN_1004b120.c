// entry 0x1004b120  FUN_1004b120

undefined4 FUN_1004b120(undefined4 param_1,undefined4 *param_2,uint param_3)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined *puVar7;
  undefined4 *puVar8;
  undefined8 uVar9;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_44;
  undefined4 uStack_40;
  int local_3c;
  undefined4 local_38;
  undefined4 local_28;
  undefined2 local_24;
  
  piVar1 = DAT_1004b4c8;
  if (param_3 == 0x81) {
    puVar8 = (undefined4 *)param_2[3];
    if (puVar8 == (undefined4 *)0x0) {
      return 0;
    }
    FUN_101b03f0(puVar8);
    FUN_10084020(puVar8);
    FUN_100df934(*puVar8);
  }
  else if (param_3 < 0x82) {
    if (param_3 == 3) {
      iVar3 = thunk_FUN_10146a3c(0x80,DAT_1004b4b0);
      if (iVar3 != 0) {
        param_2[3] = iVar3;
        FUN_101bf3b2(iVar3,0,0x80);
        *DAT_1004b4b4 = iVar3;
        local_3c = FUN_10084020(iVar3);
        if (local_3c == 0) {
          iVar4 = iVar3 + 0x50;
          lv_style_6();
          FUN_101cc792(iVar4,2);
          FUN_101cc770(iVar4,0xffffffff);
          FUN_101cc78a(iVar4,iVar3 + 0x5c);
          lv_disp_7(*param_2);
          puVar8 = (undefined4 *)param_2[3];
          uVar2 = FUN_10127cfc();
          *puVar8 = uVar2;
          lv_obj_pos_29(uVar2,0x1d2);
          FUN_101c5de4(*puVar8,9,0,0);
          uVar2 = FUN_1013ed38(*puVar8);
          lv_img_2(uVar2,puVar8 + 7);
          FUN_101c5de4(uVar2,2,0,0x14);
          uVar5 = FUN_1013ed38(*puVar8);
          lv_img_2(uVar5,puVar8 + 0xd);
          lv_obj_pos_46(uVar5,uVar2,1,0x16,0x22);
          uVar5 = FUN_1013ed38(*puVar8);
          lv_img_2(uVar5,puVar8 + 10);
          lv_obj_pos_46(uVar5,uVar2,1,0x34,0x14);
          uVar2 = FUN_10127cfc(*puVar8);
          lv_obj_pos_29(uVar2,0x182,0xc0);
          FUN_101c5de4(uVar2,2,0,0x90);
          FUN_101c6474(uVar2,0xc);
          uVar2 = FUN_1005b578(uVar2);
          lv_obj_pos_29(uVar2,0x17a,DAT_1004b4b8);
          FUN_101c5de4(uVar2,2,0,0x24);
          FUN_1012bb14(uVar2,puVar8 + 0x14,0);
          FUN_1005b70c(uVar2,puVar8 + 0x12);
          uVar2 = FUN_10127cfc(*puVar8);
          puVar8[3] = uVar2;
          FUN_101c5de4(uVar2,5,0,0xffffffe8);
          lv_obj_pos_29(puVar8[3],0x68);
          FUN_101c7378(puVar8[3],0xff,0);
          FUN_101c7358(puVar8[3],0x18c3,0);
          FUN_101c74ec(puVar8[3],0x34,0);
          uVar2 = FUN_1013ba54(puVar8[3]);
          lv_arc_12(uVar2,0x10e);
          lv_arc_16(uVar2,0,0x168);
          lv_obj_pos_29(uVar2,0x68);
          FUN_101c7470(uVar2,0xffffffff,0);
          FUN_101c7470(uVar2,0xffffffff,0x20000);
          FUN_10138dcc();
          FUN_10138dd8();
          uVar9 = FUN_10045bc4();
          FUN_10045c98((int)uVar9,(int)((ulonglong)uVar9 >> 0x20),DAT_1004b488,DAT_1004b48c);
          uVar5 = FUN_100461cc();
          FUN_101c7448(uVar2,uVar5,0);
          FUN_10138dcc();
          FUN_10138dd8();
          uVar9 = FUN_10045bc4();
          FUN_10045c98((int)uVar9,(int)((ulonglong)uVar9 >> 0x20),DAT_1004b488,DAT_1004b48c);
          uVar5 = FUN_100461cc();
          FUN_101c7448(uVar2,uVar5,0x20000);
          FUN_101c7490(uVar2,0,0);
          FUN_101c7452(uVar2,1,0);
          FUN_101c7452(uVar2,1,0x20000);
          FUN_1012bd5c(uVar2,0,0x30000);
          lv_obj_13(uVar2,2);
          FUN_101c5de4(uVar2,9,0,0);
          FUN_101391e4(&local_64);
          local_60 = DAT_1004b4bc;
          local_38 = 3000;
          local_24 = 0xffff;
          local_28 = 1000;
          local_44 = 100;
          uStack_40 = 100;
          local_64 = uVar2;
          lv_anim_1(&local_64);
          FUN_101c76a6(*param_2);
          FUN_1014c818(DAT_1004b4c0 |
                       ((int)PTR_DAT_1004b490 - (int)PTR_LAB_1004b494) * 0x20 & 0xff00U,DAT_1004b4c4
                      );
          return 0;
        }
        FUN_101ce04a(iVar3);
        param_2[3] = 0;
      }
      return 0xffffffff;
    }
    if (param_3 == 4) {
      puVar8 = (undefined4 *)param_2[3];
      if (puVar8 == (undefined4 *)0x0) {
        FUN_101d779c(DAT_1004b4a8);
      }
      else {
        if (*DAT_1004b4c8 != 0) {
          FUN_1013b1a4();
          *piVar1 = 0;
        }
        FUN_10139210(0,DAT_1004b4bc);
        FUN_101b03f0(puVar8);
        lv_obj_tree_15(*puVar8);
        lv_style_7(puVar8 + 0x14);
        FUN_101ce04a(puVar8);
        piVar1 = DAT_1004b4b4;
        param_2[3] = 0;
        *piVar1 = 0;
      }
      FUN_101d770e(DAT_1004b4a8);
    }
    else if (param_3 == 2) {
      uVar2 = FUN_100dfb88();
      uVar2 = FUN_10185540(DAT_1004b4a8,0,0,DAT_1004b4ac,0x75,DAT_1004b4a4,DAT_1004b4a0,uVar2);
      return uVar2;
    }
  }
  else {
    if (param_3 == 0x84) {
      if (param_2[3] == 0) {
        return 0;
      }
      iVar3 = (int)PTR_DAT_1004b4ec - (int)PTR_LAB_1004b4f0;
      uVar6 = DAT_1004b4f8;
      puVar7 = PTR_s_delete_guid_view_flag_false_1004b4f4;
    }
    else {
      if (param_3 != 0x85) {
        return 0;
      }
      if (param_2[3] == 0) {
        return 0;
      }
      iVar3 = (int)PTR_DAT_1004b490 - (int)PTR_LAB_1004b494;
      uVar6 = DAT_1004b49c;
      puVar7 = PTR_s_delete_guid_view_flag_true_1004b498;
    }
    FUN_1014c818(uVar6 | iVar3 * 0x20 & 0xff00U,puVar7);
  }
  return 0;
}


