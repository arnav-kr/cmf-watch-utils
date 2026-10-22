// 0x1021baab FUN_1009a890 :: album_wf_layout_style3_create
// entry 0x1009a890  FUN_1009a890

undefined4 FUN_1009a890(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  if (*(int *)(param_1 + 0xc) == 0) {
    iVar1 = thunk_FUN_10146a3c(0x50,PTR_s_album_wf_layout_style3_create_1009a914,param_3,param_4,
                               param_4);
    *(int *)(param_1 + 0xc) = iVar1;
    if (iVar1 == 0) {
      FUN_1014c818(((int)PTR_DAT_1009a91c - (int)PTR_LAB_1009a918) * 0x20 & 0xff00U | 0xa60011,
                   PTR_s_mllc_style3_err_____NULL_1009a920);
      return 0xffffffff;
    }
    FUN_101bf3b2(iVar1,0,0x50);
  }
  iVar1 = *(int *)(param_1 + 0xc);
  puVar3 = (undefined4 *)(iVar1 + -4);
  do {
    uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
    FUN_1012bb14(uVar2,param_1 + 0x2c,0);
    puVar3 = puVar3 + 1;
    *puVar3 = uVar2;
  } while ((undefined4 *)(iVar1 + 0xc) != puVar3);
  uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
  *(undefined4 *)(iVar1 + 0x40) = uVar2;
  FUN_1012bb14(uVar2,param_1 + 0x2c,0);
  lv_img(*(undefined4 *)(iVar1 + 0x40),iVar1 + 0x44);
  return 0;
}


