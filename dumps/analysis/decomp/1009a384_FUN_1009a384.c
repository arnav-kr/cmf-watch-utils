// 0x1021b923 FUN_1009a384 :: album_wf_layout_style1_create
// entry 0x1009a384  FUN_1009a384

undefined4 FUN_1009a384(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  if (*(int *)(param_1 + 0xc) == 0) {
    iVar1 = thunk_FUN_10146a3c(0x40,PTR_s_album_wf_layout_style1_create_1009a3ec);
    *(int *)(param_1 + 0xc) = iVar1;
    if (iVar1 == 0) {
      FUN_1014c818(((int)PTR_DAT_1009a3f4 - (int)PTR_LAB_1009a3f0) * 0x20 & 0xff00U | 0x8f0011,
                   PTR_s_mllc_style1_err_____NULL_1009a3f8);
      return 0xffffffff;
    }
    FUN_101bf3b2(iVar1,0,0x40);
  }
  iVar1 = *(int *)(param_1 + 0xc);
  puVar3 = (undefined4 *)(iVar1 + -4);
  do {
    uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
    FUN_1012bb14(uVar2,param_1 + 0x2c,0);
    puVar3 = puVar3 + 1;
    *puVar3 = uVar2;
  } while (puVar3 != (undefined4 *)(iVar1 + 0xc));
  return 0;
}


