// entry 0x1007b884  FUN_1007b884  (1007b884)

void FUN_1007b884(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 *puVar6;
  uint uVar7;
  undefined4 *puVar8;
  int iVar9;
  int local_1c [2];
  
  iVar9 = *(int *)(param_1 + 0xc);
  puVar8 = *(undefined4 **)(iVar9 + 0x14);
  if (puVar8 != (undefined4 *)0x0) {
    if (puVar8[0x11c] == 0) {
      if (((puVar8[0xdf] == 1) && (iVar3 = FUN_101af3d0(), iVar3 != 0)) &&
         (iVar3 = puVar8[0x11f], iVar3 == 0)) {
        puVar8[0x11f] = 1;
        jx_sys_lock_3(0x6b);
        FUN_101b5982();
        local_1c[0] = iVar3;
        ux2sys_api_9(9,local_1c,4);
        if (local_1c[0] == 0) {
          FUN_101af1a4();
        }
        if (puVar8[0x31] != 0) {
          jx_pic_anim_7(puVar8[0x2f],0);
          jx_pic_anim_8(puVar8[0x2f],3);
          simple_img_6(puVar8[0x31],puVar8 + 0x62);
          FUN_1005b70c(puVar8[0x37],puVar8 + 0xce);
          FUN_101c74a4(puVar8[0x37],0xffffffff,0);
        }
      }
    }
    else {
      iVar3 = FUN_1007b158();
      if (iVar3 != 0) {
        lv_obj_23(puVar8[1],1);
        puVar8[0x11c] = 0;
        FUN_1007b2ac(iVar9);
      }
    }
    if (puVar8[0x11d] != 0) {
      FUN_101b5998(5,0x6b);
      if (puVar8[0x32] == 0) {
        uVar4 = FUN_10127cfc(*puVar8);
        lv_obj_pos_29(uVar4,0x1d2);
        FUN_101c7358(uVar4,0,0);
        FUN_101c7378(uVar4,0xff,0);
        FUN_101c5de4(uVar4,9,0,0);
        uVar4 = FUN_1013ed38(*puVar8);
        puVar8[0x32] = uVar4;
        FUN_101c5de4(uVar4,9,0,0);
        if ((puVar8[3] != 0) && (iVar3 = lv_obj_16(puVar8[3],1), iVar3 == 0)) {
          lv_obj_23(puVar8[3],1);
        }
        if ((puVar8[1] != 0) && (iVar3 = lv_obj_16(puVar8[1],1), iVar3 == 0)) {
          lv_obj_23(puVar8[1],1);
        }
        if ((puVar8[2] != 0) && (iVar3 = lv_obj_16(puVar8[2],1), iVar3 == 0)) {
          lv_obj_23(puVar8[2],1);
        }
      }
      puVar6 = puVar8 + 0x89;
      FUN_101d775a(puVar6,1);
      local_1c[0] = *(int *)(PTR_DAT_1007bc48 + puVar8[0x11e] * 4);
      iVar9 = FUN_101848d4(iVar9 + 4,local_1c,puVar6,0,1);
      if (iVar9 < 0) {
        FUN_1014c818(((int)PTR_DAT_1007bc4c - (int)PTR_LAB_1007bc50) * 0x20 & 0xff00U | 0x2580011,
                     PTR_s__img_ids_not_found_1007bc54);
      }
      iVar9 = puVar8[0x11e];
      puVar8[0x11e] = iVar9 + 1U;
      if (4 < iVar9 + 1U) {
        FUN_101898bc(100,0);
        FUN_10189638(1);
        return;
      }
      lv_img_2(puVar8[0x32],puVar6);
    }
    if (puVar8[0x19] == 2) {
      FUN_101b5998(5,0x6b);
      iVar3 = puVar8[0x1a];
      uVar5 = iVar3 + 1;
      uVar7 = puVar8[0x1d];
      puVar8[0x1a] = uVar5;
      puVar2 = PTR_s__d__d_1007bc5c;
      puVar1 = PTR_DAT_1007bc58;
      iVar9 = *(int *)(puVar8[0x1b] + uVar7 * 4);
      if (*(int *)(PTR_DAT_1007bc58 + iVar9 * 0x10 + 4) + 5U < uVar5) {
        if (uVar7 < puVar8[0x1c] - 1) {
          puVar8[0x1d] = uVar7 + 1;
          lv_label_15(puVar8[0x17],PTR_s__d__d_1007bc5c,uVar7 + 2);
          FUN_1005b70c(puVar8[0x14],
                       puVar8 + (*(int *)(puVar1 + *(int *)(puVar8[0x1b] + puVar8[0x1d] * 4) * 0x10)
                                + 0x46) * 2);
          iVar9 = *(int *)(puVar8[0x1b] + puVar8[0x1d] * 4);
          lv_label_15(puVar8[0x15],DAT_1007bc60,*(undefined4 *)(puVar1 + iVar9 * 0x10 + 4),
                      puVar8[(*(int *)(puVar1 + iVar9 * 0x10 + 8) + 0x46) * 2]);
          lv_obj_23(puVar8[0xc],1);
          FUN_1014c818(((int)PTR_DAT_1007bc4c - (int)PTR_LAB_1007bc50) * 0x20 & 0xff00U | 0x1e80031,
                       PTR_s__warm_up_preview_simple_video_pl_1007bc64);
          FUN_101aa780(puVar8 + 0x1f,0x40,PTR_s__NAND_E__d_ajs_1007bc68,
                       *(undefined4 *)
                        (puVar1 + *(int *)(puVar8[0x1b] + puVar8[0x1d] * 4) * 0x10 + 0xc));
          simple_video_9(puVar8[0x18],0);
          simple_video_8(puVar8[0x18],puVar8 + 0x1f);
          simple_video_10(puVar8[0x18],1);
          simple_video_9(puVar8[0x18],1);
          puVar8[0x1a] = 0;
          if (puVar8[0x1d] == puVar8[0x1c] + -1) {
            puVar6 = puVar8 + 0x7d;
          }
          else {
            puVar6 = puVar8 + 0x7a;
          }
          simple_img_6(puVar8[9],puVar6);
          simple_img_6(puVar8[10],puVar8 + 0x80);
        }
        else {
          puVar8[0x19] = 0;
          puVar8[0x1d] = 0;
          puVar8[0x1a] = 0;
          lv_label_15(puVar8[0x17],puVar2,1);
          FUN_101af1a4();
          simple_video_9(puVar8[0x18],0);
          lv_obj_23(puVar8[0xb],1);
          lv_obj_23(puVar8[5],1);
          lv_obj_13(puVar8[2],1);
          if (puVar8[0xdf] == 0) {
            jx_sys_lock_3(0x6b);
          }
          else {
            FUN_101b5998(300,0x6b);
          }
        }
      }
      else if (uVar5 < 7) {
        if (uVar5 == 4) {
          FUN_101af1a4();
          uVar4 = puVar8[0x34];
          if (puVar8[0xd] != 0) {
            lv_obj_tree_15();
            puVar8[0xd] = 0;
          }
          uVar4 = FUN_10127cfc(uVar4);
          puVar8[0xd] = uVar4;
          lv_obj_pos_29(uVar4,0x1d2);
          FUN_101c5de4(puVar8[0xd],9,0);
          FUN_101c7378(puVar8[0xd],0xff,0);
          FUN_101c7358(puVar8[0xd],0);
          uVar4 = FUN_1005a014(puVar8[0xd]);
          simple_img_6(uVar4,puVar8 + 0x86);
          lv_obj_23(uVar4,2);
          FUN_101c5de4(uVar4,9,0);
          FUN_101b98c4(puVar8[0xd]);
        }
        else if (uVar5 == 6) {
          if (puVar8[0xd] != 0) {
            lv_obj_tree_15();
            puVar8[0xd] = 0;
          }
          FUN_101af1a4();
          iVar9 = *(int *)(puVar8[0x1b] + puVar8[0x1d] * 4);
          uVar4 = *(undefined4 *)(puVar1 + iVar9 * 0x10 + 4);
          lv_label_15(puVar8[0x15],DAT_1007bccc,puVar8[0x1a] + -6,uVar4,
                      puVar8[(*(int *)(puVar1 + iVar9 * 0x10 + 8) + 0x46) * 2]);
          lv_bar_10(puVar8[0xc],0,uVar4);
          lv_bar_5(puVar8[0xc],0);
          lv_obj_13(puVar8[0xc],1);
        }
      }
      else {
        lv_label_15(puVar8[0x15],PTR_s__d__d__s_1007bc6c,iVar3 + -5,
                    *(int *)(PTR_DAT_1007bc58 + iVar9 * 0x10 + 4),
                    puVar8[(*(int *)(PTR_DAT_1007bc58 + iVar9 * 0x10 + 8) + 0x46) * 2]);
        lv_bar_5(puVar8[0xc],puVar8[0x1a] + -6,0);
      }
    }
    iVar9 = FUN_100d6770();
    if (iVar9 == 0) {
      FUN_101d7326(0x6b,0,0);
    }
  }
  return;
}


