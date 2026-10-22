// entry 0x10095700  FUN_10094a48  (10094a48)

undefined4 FUN_10094a48(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  int *piVar6;
  
  uVar4 = *(undefined4 *)(param_1 + 0x14);
  FUN_10116764(1);
  *DAT_10094e04 = uVar4;
  piVar5 = *(int **)(param_1 + 0x14);
  lv_disp_7(**(undefined4 **)(param_1 + 0x10));
  if (piVar5 != (int *)0x0) {
    iVar1 = FUN_10127cfc();
    *piVar5 = iVar1;
    if (iVar1 != 0) {
      lv_obj_pos_29(iVar1,0x1d2);
      FUN_101c5de4(*piVar5,9,0,0,param_4);
      lv_obj_13(*piVar5,0x8000);
      iVar1 = FUN_10127cfc(*piVar5);
      piVar5[0x15] = iVar1;
      if (iVar1 != 0) {
        lv_obj_pos_29(iVar1,0x1d2);
        FUN_101c5de4(piVar5[0x15],9,0);
        lv_obj_13(piVar5[0x15],0x8000);
        iVar1 = FUN_101380a0(*piVar5);
        piVar5[1] = iVar1;
        if (iVar1 != 0) {
          lv_obj_pos_29(iVar1,0x1d2);
          FUN_101c5de4(piVar5[1],9,0);
          iVar1 = FUN_101380b8(piVar5[1],1,0);
          if (iVar1 != 0) {
            uVar4 = FUN_1013d388();
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c5de4(uVar4,2,0);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            piVar6 = piVar5 + 0x1f;
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_10094e10);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x45);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_10094e14);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x88);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_10094e18);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0xcb);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_10094e1c);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x10e);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_10094e20);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x151);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_10094e24);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x194);
            lv_event_11(uVar4,PTR_FUN_10094800_1_10094e0c,7,DAT_10094e08);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,DAT_100951a0);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x1d7);
            lv_event_11(uVar4,PTR_FUN_10094800_1_100951a8,7,DAT_100951a4);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_100951ac);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x21a);
            lv_event_11(uVar4,PTR_FUN_10094800_1_100951a8,7,DAT_100951a4);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_100951b0);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x25d);
            lv_event_11(uVar4,PTR_FUN_10094800_1_100951a8,7,DAT_100951a4);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_100951b4);
            uVar4 = FUN_1013d388(iVar1);
            lv_obj_pos_29(uVar4,0x1d2,0x3c);
            FUN_101c7378(uVar4,0xff,0);
            FUN_101c7358(uVar4,0x4208,0);
            FUN_101c5de4(uVar4,2,0,0x2a0);
            lv_event_11(uVar4,PTR_FUN_10094800_1_100951a8,7,DAT_100951a4);
            uVar4 = FUN_1013f5f0(uVar4);
            FUN_101c74ce(uVar4,piVar6,0);
            FUN_101c74a4(uVar4,0xffffffff,0);
            FUN_101c5de4(uVar4,9,0);
            lv_label_14(uVar4,PTR_DAT_100951b8);
            iVar1 = FUN_101380b8(piVar5[1],2,0);
            if (iVar1 != 0) {
              FUN_1008ec58(piVar5,iVar1);
              iVar1 = FUN_101380b8(piVar5[1],3,0);
              piVar5[0xd] = iVar1;
              if (iVar1 != 0) {
                FUN_1008f4a4(piVar5,iVar1);
                iVar1 = FUN_101380b8(piVar5[1],4,0);
                piVar5[0xe] = iVar1;
                if (iVar1 != 0) {
                  FUN_1008f104(piVar5,iVar1);
                  iVar1 = FUN_101380b8(piVar5[1],6,0);
                  piVar5[3] = iVar1;
                  if (iVar1 != 0) {
                    FUN_1008f7e0(piVar5,iVar1);
                    iVar1 = FUN_101380b8(piVar5[1],7,0);
                    piVar5[4] = iVar1;
                    if (iVar1 != 0) {
                      FUN_1008f61c(piVar5,iVar1);
                      iVar1 = FUN_101380b8(piVar5[1],0x12,0);
                      piVar5[5] = iVar1;
                      if (iVar1 != 0) {
                        FUN_10092288(piVar5,iVar1);
                        iVar1 = FUN_101380b8(piVar5[1],0x14,0);
                        piVar5[2] = iVar1;
                        if (iVar1 != 0) {
                          FUN_1008e49c(piVar5,iVar1);
                          iVar1 = FUN_101380b8(piVar5[1],0x15,0);
                          piVar5[6] = iVar1;
                          if (iVar1 != 0) {
                            FUN_1008e8c4(piVar5,iVar1);
                            iVar1 = FUN_101380b8(piVar5[1],0xb,0);
                            piVar5[7] = iVar1;
                            if (iVar1 != 0) {
                              FUN_10090200(piVar5,iVar1);
                              iVar1 = FUN_101380b8(piVar5[1],0xc,0);
                              piVar5[8] = iVar1;
                              if (iVar1 != 0) {
                                FUN_1009044c(piVar5,iVar1);
                                iVar1 = FUN_101380b8(piVar5[1],0xe,0);
                                piVar5[9] = iVar1;
                                if (iVar1 != 0) {
                                  FUN_100906f0(piVar5,iVar1);
                                  iVar1 = FUN_101380b8(piVar5[1],0xf,0);
                                  if (iVar1 != 0) {
                                    FUN_100908d0(piVar5,iVar1);
                                    iVar1 = FUN_101380b8(piVar5[1],5,0);
                                    piVar5[0xf] = iVar1;
                                    if (iVar1 != 0) {
                                      FUN_1008eeb8(piVar5,iVar1);
                                      iVar1 = FUN_101380b8(piVar5[1],8,0);
                                      piVar5[10] = iVar1;
                                      if (iVar1 != 0) {
                                        FUN_1008fb74(piVar5,iVar1);
                                        iVar1 = FUN_101380b8(piVar5[1],9,0);
                                        piVar5[0xb] = iVar1;
                                        if (iVar1 != 0) {
                                          FUN_1008ff2c(piVar5,iVar1);
                                          iVar1 = FUN_101380b8(piVar5[1],0xd,0);
                                          piVar5[0xc] = iVar1;
                                          if (iVar1 != 0) {
                                            FUN_10090b88(piVar5,iVar1);
                                            iVar1 = FUN_101380b8(piVar5[1],0,0);
                                            if (iVar1 != 0) {
                                              uVar4 = FUN_1013f5f0();
                                              lv_label_18(uVar4,3);
                                              lv_obj_pos_27(uVar4,DAT_100955a0);
                                              FUN_1012bb14(uVar4,piVar5 + 0x28,0);
                                              lv_label_14(uVar4,PTR_DAT_100955a4);
                                              FUN_101c5de4(uVar4,2,0,0x14);
                                              uVar4 = FUN_1013d388(iVar1);
                                              lv_obj_pos_29(uVar4,0x50);
                                              FUN_101c7358(uVar4,0x7e0,0);
                                              FUN_101c7378(uVar4,0xff,0);
                                              FUN_101c5de4(uVar4,9,0x50,0x78);
                                              lv_event_11(uVar4,PTR_FUN_10093ecc_1_100955a8,7,0);
                                              uVar4 = FUN_1013f5f0(uVar4);
                                              FUN_101c74ce(uVar4,piVar6,0);
                                              FUN_101c74a4(uVar4,0xffffffff,0);
                                              FUN_101c5de4(uVar4,9,0);
                                              lv_label_14(uVar4,PTR_DAT_100955ac);
                                              uVar4 = FUN_1013d388(iVar1);
                                              lv_obj_pos_29(uVar4,0x50);
                                              FUN_101c7358(uVar4,0xf800,0);
                                              FUN_101c7378(uVar4,0xff,0);
                                              FUN_101c5de4(uVar4,9,0xffffffb0,0x78);
                                              lv_event_11(uVar4,PTR_FUN_10093ecc_1_100955a8,7,0);
                                              uVar4 = FUN_1013f5f0(uVar4);
                                              FUN_101c74ce(uVar4,piVar6,0);
                                              FUN_101c74a4(uVar4,0xffffffff,0);
                                              FUN_101c5de4(uVar4,9,0);
                                              lv_label_14(uVar4,PTR_DAT_100955b0);
                                              iVar1 = FUN_101380b8(piVar5[1],0x10,0);
                                              if (iVar1 != 0) {
                                                FUN_10090d5c(piVar5,iVar1);
                                                iVar1 = FUN_101380b8(piVar5[1],0x19,0);
                                                piVar5[0x10] = iVar1;
                                                if (iVar1 != 0) {
                                                  uVar4 = FUN_10127cfc();
                                                  lv_obj_pos_29(uVar4,0x1d2);
                                                  FUN_101c5de4(uVar4,9,0);
                                                  lv_obj_13(uVar4,0x8000);
                                                  lv_event_11(uVar4,PTR_FUN_10093ad8_1_100955b4,0xc,
                                                              0);
                                                  iVar1 = thunk_FUN_10146a3c(0x168,
                                                  PTR_s__sub_view_qrcode_create_100955b8);
                                                  if (iVar1 == 0) {
                                                    FUN_1014c818(DAT_100955c4 |
                                                                 ((int)PTR_DAT_100955c0 -
                                                                 (int)PTR_LAB_100955bc) * 0x20 &
                                                                 0xff00U,DAT_100955c8,
                                                                                                                                  
                                                  PTR_s__sub_view_qrcode_create_100955b8);
                                                  }
                                                  else {
                                                    FUN_100949a0(iVar1,0x168);
                                                    uVar4 = FUN_10134228(uVar4,0x13b,0,0xffffffff);
                                                    uVar3 = FUN_101bf36a(iVar1);
                                                    lv_qrcode_3(uVar4,iVar1,uVar3,0);
                                                    lv_obj_23(uVar4,0x20000000);
                                                    FUN_101c5de4(uVar4,9,0);
                                                    FUN_101c73b6(uVar4,0xffffffff,0);
                                                    FUN_101c73d6(uVar4,8,0);
                                                    FUN_101ce04a(iVar1);
                                                  }
                                                  iVar1 = FUN_101380b8(piVar5[1],0x1a,0);
                                                  piVar5[0x11] = iVar1;
                                                  if (iVar1 != 0) {
                                                    FUN_10091828(piVar5,iVar1);
                                                    iVar1 = FUN_101380b8(piVar5[1],0x1c,0);
                                                    if (iVar1 != 0) {
                                                      uVar4 = FUN_10127cfc();
                                                      lv_obj_pos_29(uVar4,0x1d2);
                                                      FUN_101c5de4(uVar4,9,0);
                                                      lv_obj_13(uVar4,0x8000);
                                                      iVar2 = thunk_FUN_10146a3c(0x40,DAT_100955cc);
                                                      if (iVar2 == 0) {
                                                        FUN_1014c818(DAT_100955d0 |
                                                                     ((int)PTR_DAT_100955c0 -
                                                                     (int)PTR_LAB_100955bc) * 0x20 &
                                                                     0xff00U,DAT_100955c8,
                                                                     DAT_100955cc);
                                                      }
                                                      else {
                                                        FUN_101b0f50();
                                                        uVar4 = FUN_10134228(uVar4,0xdc,0,0xffffffff
                                                                            );
                                                        uVar3 = FUN_101bf36a(iVar2);
                                                        lv_qrcode_3(uVar4,iVar2,uVar3,0);
                                                        lv_obj_23(uVar4,0x10000000);
                                                        FUN_101c5de4(uVar4,9,0);
                                                        FUN_101c73b6(uVar4,0xffffffff,0);
                                                        FUN_101c73d6(uVar4,8,0);
                                                        FUN_101ce04a(iVar2);
                                                        uVar4 = FUN_1013ed38(iVar1);
                                                        lv_img_2(uVar4,piVar5 + 0x16);
                                                        FUN_101c5de4(uVar4,9,0);
                                                        lv_obj_23(uVar4,1);
                                                        uVar4 = FUN_1013d388(iVar1);
                                                        lv_obj_pos_29(uVar4,0x3c);
                                                        FUN_101c7358(uVar4,0xffffffff,0);
                                                        FUN_101c7378(uVar4,0x33,0);
                                                        FUN_101c5de4(uVar4,0,0x109,0x1e);
                                                        FUN_101c74ec(uVar4,0x1e,0);
                                                        lv_event_11(uVar4,
                                                  PTR_LAB_10093964_1_1009578c,7,0);
                                                  }
                                                  iVar1 = FUN_101380b8(piVar5[1],10,0);
                                                  piVar5[0x12] = iVar1;
                                                  if (iVar1 != 0) {
                                                    FUN_10091da8(piVar5,iVar1);
                                                    iVar1 = FUN_101380b8(piVar5[1],0x1d,0);
                                                    if (iVar1 != 0) {
                                                      FUN_10091100(piVar5,iVar1);
                                                      iVar1 = FUN_101380b8(piVar5[1],0x11,0);
                                                      piVar5[0x13] = iVar1;
                                                      if (iVar1 != 0) {
                                                        FUN_10090f6c(piVar5,iVar1);
                                                        iVar1 = FUN_101380b8(piVar5[1],0x1e,0);
                                                        piVar5[0x14] = iVar1;
                                                        if (iVar1 != 0) {
                                                          uVar4 = FUN_1013f5f0();
                                                          lv_label_18(uVar4,3);
                                                          lv_obj_pos_27(uVar4,DAT_100955a0);
                                                          FUN_1012bb14(uVar4,piVar5 + 0x28,0);
                                                          lv_label_14(uVar4,PTR_DAT_100955d4);
                                                          FUN_101c5de4(uVar4,1,300);
                                                          uVar4 = FUN_1013d388(iVar1);
                                                          lv_obj_pos_29(uVar4,0x46);
                                                          FUN_101c7358(uVar4,0x7e0,0);
                                                          FUN_101c7378(uVar4,0xff,0);
                                                          FUN_101c5de4(uVar4,1,100,0x15e);
                                                          lv_event_11(uVar4,
                                                  PTR_LAB_10093d40_1_100955d8,7,0);
                                                  uVar4 = FUN_1013f5f0(uVar4);
                                                  FUN_101c74ce(uVar4,piVar6,0);
                                                  FUN_101c74a4(uVar4,0xffffffff,0);
                                                  FUN_101c5de4(uVar4,9,0);
                                                  lv_label_14(uVar4,PTR_DAT_100955dc);
                                                  uVar4 = FUN_1013d388(iVar1);
                                                  lv_obj_pos_29(uVar4,0x46);
                                                  FUN_101c7358(uVar4,0x7e0,0);
                                                  FUN_101c7378(uVar4,0xff,0);
                                                  FUN_101c5de4(uVar4,1,200,0x15e);
                                                  lv_event_11(uVar4,PTR_LAB_10093d40_1_100955d8,7,0)
                                                  ;
                                                  uVar4 = FUN_1013f5f0(uVar4);
                                                  FUN_101c74ce(uVar4,piVar6,0);
                                                  FUN_101c74a4(uVar4,0xffffffff,0);
                                                  FUN_101c5de4(uVar4,9,0);
                                                  lv_label_14(uVar4,PTR_DAT_100955e0);
                                                  uVar4 = FUN_1013f5f0(iVar1);
                                                  FUN_101c74ce(uVar4,piVar6,0);
                                                  FUN_101c74a4(uVar4,0xffffffff,0);
                                                  lv_obj_pos_27(uVar4,DAT_1009577c);
                                                  FUN_101c74e2(uVar4,1,0);
                                                  FUN_101c5de4(uVar4,1,0x118,0x15e);
                                                  piVar6 = DAT_10095780;
                                                  lv_label_18(uVar4,0);
                                                  lv_label_14(uVar4,PTR_DAT_10095784);
                                                  if (*piVar6 != 0) {
                                                    return 0;
                                                  }
                                                  iVar1 = lv_timer_2(PTR_LAB_100958fc_1_10095788,200
                                                                     ,*piVar5);
                                                  *piVar6 = iVar1;
                                                  return 0;
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  FUN_1014c818(DAT_10095798 | ((int)PTR_DAT_10095794 - (int)PTR_LAB_10095790) * 0x20 & 0xff00U,
               DAT_1009579c);
  return 0;
}


