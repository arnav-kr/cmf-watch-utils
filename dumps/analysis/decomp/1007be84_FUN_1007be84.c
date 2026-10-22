// entry 0x1007be84  FUN_1007be84  (1007be84)

void FUN_1007be84(undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  
  iVar1 = FUN_101c4c9a();
  puVar2 = (undefined4 *)FUN_101c4ca6(param_1);
  iVar3 = FUN_101c4c96(param_1);
  if (iVar1 != 7) {
    return;
  }
  iVar1 = *(int *)(iVar3 + 0x10);
  uVar6 = (DAT_1007c058 - DAT_1007c054) * 0x20 & 0xff00;
  FUN_1014c818(DAT_1007c05c | uVar6,DAT_1007c060,iVar1);
  if (iVar1 == 0) {
    if (puVar2 != (undefined4 *)0x0) {
      if (puVar2[6] == 0) {
        uVar4 = FUN_10127cfc(*puVar2);
        puVar2[6] = uVar4;
        lv_obj_pos_26(uVar4,0,0);
        lv_obj_pos_29(puVar2[6],0x1d2);
        FUN_101c7326(puVar2[6],100,0);
        lv_event_11(puVar2[6],DAT_1007c064,0xc,puVar2);
        lv_obj_23(puVar2[6],2);
        lv_obj_13(puVar2[6],0x8000);
        uVar4 = FUN_100d93cc(*puVar2);
        lv_obj_pos_29(uVar4,0x1ce);
        jx_scroll_bar_2(uVar4,puVar2[6]);
        jx_scroll_bar_3(uVar4,1);
        FUN_101af1c2(uVar4);
        uVar4 = FUN_1013f5f0(puVar2[6]);
        FUN_101c74a4(uVar4,0xad55,0);
        FUN_101c74e2(uVar4,3,0);
        FUN_101c74ce(uVar4,puVar2 + 0xea,0);
        lv_obj_13(uVar4,2);
        lv_obj_pos_29(uVar4,DAT_1007c068,puVar2[0xec]);
        lv_label_15(uVar4,DAT_1007c06c,puVar2[0x1e]);
        uVar5 = FUN_1005b578(puVar2[6]);
        lv_obj_pos_27(uVar5,100);
        lv_obj_13(uVar5,2);
        FUN_101c74a4(uVar5,0xad55,0);
        FUN_101c74e2(uVar5,1,0);
        FUN_101c74ce(uVar5,puVar2 + 0xea,0);
        FUN_101af1cc(uVar5,*(undefined2 *)(puVar2 + 0xec));
        FUN_101acba4(uVar5,3);
        FUN_1005b70c(uVar5,puVar2 + 0x9c);
        FUN_101c5de4(uVar5,1,200,0x1a);
        lv_obj_pos_46(uVar4,uVar5,0x11,0xfffffff6,0);
        uVar4 = FUN_1007bcd0(0,*(undefined4 *)puVar2[0x1b],puVar2);
        for (uVar7 = 1; uVar7 < (uint)puVar2[0x1c]; uVar7 = uVar7 + 1) {
          uVar4 = FUN_1007bcd0(uVar4,*(undefined4 *)(puVar2[0x1b] + uVar7 * 4),puVar2);
        }
        FUN_1014c818(uVar6 | DAT_1007c070,DAT_1007c074);
      }
      else {
        lv_obj_13(puVar2[6],1);
      }
    }
    lv_obj_23(puVar2[5],1);
    uVar4 = puVar2[6];
  }
  else {
    if (iVar1 != 1) {
      return;
    }
    if (puVar2 != (undefined4 *)0x0) {
      puVar10 = puVar2 + 0x1f;
      if (puVar2[0xb] == 0) {
        uVar4 = FUN_101380a0(*puVar2);
        puVar2[0xb] = uVar4;
        lv_obj_pos_29(uVar4,0x1d2);
        FUN_101c5de4(puVar2[0xb],9,0,0);
        uVar4 = FUN_101380b8(puVar2[0xb],0,0,2);
        puVar2[0x33] = uVar4;
        uVar4 = FUN_1005a014();
        simple_img_6(uVar4,puVar2 + 0x74);
        FUN_101c5de4(uVar4,2,0,0x1b8);
        uVar4 = FUN_1005a014(puVar2[0x33]);
        puVar2[7] = uVar4;
        simple_img_6(uVar4,puVar2 + 0x6e);
        FUN_101c5de4(puVar2[7],2,0,0x18);
        lv_obj_23(puVar2[7],2);
        *(undefined4 *)(puVar2[7] + 0x10) = 0;
        lv_event_11(puVar2[7],DAT_1007c48c,7,puVar2);
        uVar4 = FUN_1013f5f0(puVar2[0x33]);
        lv_obj_pos_29(uVar4,0x114,0x32);
        FUN_101c74a4(uVar4,0xffffffff,0);
        puVar8 = puVar2 + 0xfa;
        FUN_101c74e2(uVar4,2,0);
        FUN_101c74ce(uVar4,puVar8,0);
        lv_label_18(uVar4,1);
        lv_label_17(uVar4,puVar2 + 0xa4);
        lv_obj_pos_46(uVar4,puVar2[7],0xe,0,8);
        iVar1 = FUN_1005a014(puVar2[0x33]);
        simple_img_6(iVar1,puVar2 + 0x71);
        FUN_101c5de4(iVar1,2,0,0xe4);
        lv_obj_23(iVar1,2);
        uVar4 = DAT_1007c48c;
        *(undefined4 *)(iVar1 + 0x10) = 1;
        lv_event_11(iVar1,uVar4,7,puVar2);
        uVar4 = FUN_1013f5f0(puVar2[0x33]);
        lv_obj_pos_29(uVar4,0x114,0x32);
        FUN_101c74a4(uVar4,0xffffffff,0);
        FUN_101c74e2(uVar4,2,0);
        FUN_101c74ce(uVar4,puVar8,0);
        lv_label_18(uVar4,1);
        lv_label_17(uVar4,puVar2 + 0xa6);
        lv_obj_pos_46(uVar4,iVar1,0xe,0,8);
        uVar4 = FUN_101380b8(puVar2[0xb],1,0,1);
        puVar2[0x34] = uVar4;
        uVar4 = FUN_1005a014();
        simple_img_6(uVar4,puVar2 + 0x77);
        FUN_101c5de4(uVar4,2,0,0x1b8);
        uVar4 = FUN_1005a014(puVar2[0x34]);
        puVar2[9] = uVar4;
        simple_img_6(uVar4,puVar2 + 0x7a);
        FUN_101c5de4(puVar2[9],8,0xffffffda,0xffffffa7);
        lv_obj_23(puVar2[9],2);
        *(undefined4 *)(puVar2[9] + 0x10) = 3;
        lv_event_11(puVar2[9],DAT_1007c48c,7,puVar2);
        uVar4 = FUN_1005a014(puVar2[0x34]);
        puVar2[10] = uVar4;
        simple_img_6(uVar4,puVar2 + 0x83);
        FUN_101c5de4(puVar2[10],7,0x26,0xffffffa7);
        lv_obj_23(puVar2[10],2);
        *(undefined4 *)(puVar2[10] + 0x10) = 2;
        lv_event_11(puVar2[10],DAT_1007c48c,7,puVar2);
        FUN_100dfee0(puVar2[0x34],puVar2 + 0x5a,puVar2 + 0x10a);
        puVar2[0x1d] = 0;
        uVar4 = FUN_1013f5f0(puVar2[0x34]);
        puVar2[0x17] = uVar4;
        lv_obj_pos_29(uVar4,100,0x24);
        FUN_101c74a4(puVar2[0x17],0xad55,0);
        FUN_101c74e2(puVar2[0x17],2,0);
        FUN_101c74ce(puVar2[0x17],puVar2 + 0xea,0);
        lv_label_18(puVar2[0x17],1);
        lv_label_15(puVar2[0x17],DAT_1007c490,puVar2[0x1d] + 1,puVar2[0x1c]);
        FUN_101c5de4(puVar2[0x17],2,0,0x30);
        uVar4 = FUN_1005b578(puVar2[0x34]);
        puVar2[0x14] = uVar4;
        lv_obj_pos_27(uVar4,0x118);
        lv_obj_13(puVar2[0x14],2);
        FUN_101c74a4(puVar2[0x14],0xffffffff,0);
        FUN_101c74e2(puVar2[0x14],2,0);
        FUN_101c74ce(puVar2[0x14],puVar8,0);
        FUN_101af1cc(puVar2[0x14],*(undefined2 *)(puVar2 + 0xfc));
        FUN_101acba4(puVar2[0x14],3);
        iVar1 = DAT_1007c4a0;
        puVar8 = puVar2 + 0xe;
        FUN_1005b70c(puVar2[0x14],
                     puVar2 + (*(int *)(DAT_1007c4a0 + *(int *)puVar2[0x1b] * 0x10) + 0x46) * 2);
        FUN_101c5de4(puVar2[0x14],2,0,0x144);
        uVar4 = FUN_1013f5f0(puVar2[0x34]);
        puVar2[0x15] = uVar4;
        FUN_101c74a4(uVar4,0xad55,0);
        FUN_101c74e2(puVar2[0x15],2,0);
        FUN_101c74ce(puVar2[0x15],puVar2 + 0xe2,0);
        lv_obj_13(puVar2[0x15],2);
        lv_obj_pos_29(puVar2[0x15],0xca,puVar2[0xe4]);
        iVar3 = iVar1 + *(int *)puVar2[0x1b] * 0x10;
        lv_label_15(puVar2[0x15],DAT_1007c494,*(undefined4 *)(iVar3 + 4),
                    puVar2[(*(int *)(iVar3 + 8) + 0x46) * 2]);
        lv_obj_pos_46(puVar2[0x15],puVar2[0x14],0xe,0,6);
        uVar5 = *(undefined4 *)(iVar1 + *(int *)puVar2[0x1b] * 0x10 + 4);
        puVar9 = puVar2 + 0x11;
        FUN_1014c818(uVar6 | DAT_1007c498,DAT_1007c49c);
        lv_style_6(puVar8);
        FUN_101cc712(puVar8,0x4229);
        FUN_101cc724(puVar8,0xff);
        FUN_101cc79a(puVar8,0x7fff);
        lv_style_6(puVar9);
        FUN_101cc712(puVar9,0xffffffff);
        FUN_101cc724(puVar9,0xff);
        uVar4 = FUN_10141eb8(puVar2[0x34]);
        puVar2[0xc] = uVar4;
        lv_obj_pos_29(uVar4,0xca,6);
        FUN_1012bb14(puVar2[0xc],puVar8,0);
        FUN_1012bb14(puVar2[0xc],puVar9,0x20000);
        lv_bar_10(puVar2[0xc],0,uVar5);
        lv_bar_5(puVar2[0xc],uVar5,0);
        lv_obj_23(puVar2[0xc],1);
        lv_obj_13(puVar2[0xc],2);
        lv_obj_pos_46(puVar2[0xc],puVar2[0x14],0xe,0,0x38);
        uVar4 = FUN_100d9a00(puVar2[0x34]);
        puVar2[0x18] = uVar4;
        FUN_101c5de4(uVar4,9,0,0xffffffdd);
        FUN_101aa780(puVar10,0x40,DAT_1007c630,
                     *(undefined4 *)(iVar1 + *(int *)puVar2[0x1b] * 0x10 + 0xc));
        simple_video_8(puVar2[0x18],puVar10);
        simple_video_10(puVar2[0x18],1);
        simple_video_9(puVar2[0x18],1);
        puVar2[0x19] = 2;
        puVar2[0x1a] = 0;
        lv_tileview_1(puVar2[0xb],1,0,0);
      }
      else {
        lv_obj_13(puVar2[0xb],1);
        if (puVar2[0xd] != 0) {
          lv_obj_23(puVar2[0xd],2);
        }
        lv_tileview_1(puVar2[0xb],1,0);
        FUN_1014c818(DAT_1007c634 | uVar6,DAT_1007c638);
        puVar2[0x1d] = 0;
        simple_img_6(puVar2[9],puVar2 + 0x7a);
        simple_img_6(puVar2[10],puVar2 + 0x83);
        if (puVar2[0x18] != 0) {
          simple_video_9(puVar2[0x18],1);
          puVar2[0x1a] = 0;
          puVar2[0x19] = 2;
        }
        lv_label_15(puVar2[0x17],DAT_1007c63c,puVar2[0x1d] + 1,puVar2[0x1c]);
        iVar1 = DAT_1007c640;
        FUN_1005b70c(puVar2[0x14],
                     puVar2 + (*(int *)(DAT_1007c640 +
                                       *(int *)(puVar2[0x1b] + puVar2[0x1d] * 4) * 0x10) + 0x46) * 2
                    );
        iVar3 = iVar1 + *(int *)(puVar2[0x1b] + puVar2[0x1d] * 4) * 0x10;
        lv_label_15(puVar2[0x15],DAT_1007c644,*(undefined4 *)(iVar3 + 4),
                    puVar2[(*(int *)(iVar3 + 8) + 0x46) * 2]);
        lv_obj_23(puVar2[0xc],1);
        FUN_1014c818(uVar6 | DAT_1007c648,DAT_1007c64c);
        FUN_101aa780(puVar10,0x40,DAT_1007c630,
                     *(undefined4 *)(iVar1 + *(int *)(puVar2[0x1b] + puVar2[0x1d] * 4) * 0x10 + 0xc)
                    );
        simple_video_9(puVar2[0x18],0);
        simple_video_8(puVar2[0x18],puVar10);
        simple_video_10(puVar2[0x18],1);
        simple_video_9(puVar2[0x18],1);
        puVar2[0x1a] = 0;
        if (puVar2[0xd] != 0) {
          lv_obj_tree_15();
          puVar2[0xd] = 0;
        }
      }
    }
    lv_obj_23(puVar2[5],1);
    uVar4 = puVar2[0xb];
  }
  lv_obj_13(uVar4,1);
  return;
}


