// entry 0x10060a54  FUN_10060a54

undefined4 FUN_10060a54(undefined4 param_1,int param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_50;
  undefined4 local_3c;
  undefined4 uStack_38;
  int local_34;
  undefined4 local_30;
  
  if (param_2 == 3) {
    uVar1 = lv_disp_7(*param_3);
    iVar3 = thunk_FUN_10146a3c(0x20,PTR_s__goodbye_view_layout_10060b90);
    if (iVar3 == 0) {
      uVar1 = 0xfffffff4;
    }
    else {
      iVar4 = iVar3 + 0x14;
      iVar2 = FUN_10184804(DAT_10060b9c,iVar4,PTR_s__NAND_K_logo_sty_10060b98,
                           PTR_s__NAND_K_logo_res_10060b94,0);
      if (iVar2 < 0) {
        FUN_1014c818(((int)PTR_DAT_10060ba0 - (int)PTR_LAB_10060ba4) * 0x20 & 0xff00U | 0x1d0011,
                     PTR_s_SCENE_LOGO_0x_x_not_found_10060ba8,DAT_10060b9c);
      }
      else {
        local_5c = DAT_10060bb0;
        iVar2 = FUN_101848d4(iVar4,&local_5c,iVar3,iVar3 + 0xc,1);
        FUN_101d7714(iVar4);
        if (iVar2 == 0) {
          FUN_101c7378(uVar1,0xff,0);
          FUN_101c7358(uVar1,0,0);
          uVar1 = FUN_1013ed38(uVar1);
          lv_obj_pos_26(uVar1,*(undefined4 *)(iVar3 + 0xc),*(undefined4 *)(iVar3 + 0x10));
          lv_img_2(uVar1,iVar3);
          FUN_101391e4(&local_5c);
          local_3c = 0xfa;
          uStack_38 = 0xfa;
          local_30 = 2000;
          local_58 = DAT_10060bb4;
          local_50 = DAT_10060bb8;
          local_5c = uVar1;
          local_34 = iVar2;
          lv_anim_1(&local_5c);
          FUN_101c76a6(*param_3);
          param_3[3] = iVar3;
          return 0;
        }
      }
      FUN_1014c818(((int)PTR_DAT_10060ba0 - (int)PTR_LAB_10060ba4) * 0x20 & 0xff00U | 0x450011,
                   PTR_s_res_load_failed_10060bac);
      FUN_101ce04a(iVar3);
      uVar1 = 0xfffffffe;
    }
  }
  else {
    if (param_2 == 4) {
      iVar3 = param_3[3];
      if (iVar3 == 0) {
        return 0;
      }
      FUN_10139210(0,DAT_10060bb4);
      FUN_101d775a(iVar3,1);
      FUN_101ce04a(iVar3);
    }
    else if (param_2 == 2) {
      uVar1 = FUN_101d72fe();
      return uVar1;
    }
    uVar1 = 0;
  }
  return uVar1;
}


