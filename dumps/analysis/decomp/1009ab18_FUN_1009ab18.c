// 0x1021bb65 FUN_1009ab18 :: album_wf_layout_style4_create
// entry 0x1009ab18  FUN_1009ab18

undefined4 FUN_1009ab18(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  
  if (*(int *)(param_1 + 0xc) == 0) {
    iVar1 = thunk_FUN_10146a3c(0x80,PTR_s_album_wf_layout_style4_create_1009abd0,param_3,param_4,
                               param_4);
    *(int *)(param_1 + 0xc) = iVar1;
    if (iVar1 == 0) {
      FUN_1014c818(((int)PTR_DAT_1009abd8 - (int)PTR_LAB_1009abd4) * 0x20 & 0xff00U | 0xd20011,
                   PTR_s_mllc_style4_err_____NULL_1009abdc);
      return 0xffffffff;
    }
    FUN_101bf3b2(iVar1,0,0x80);
  }
  iVar1 = *(int *)(param_1 + 0xc);
  iVar4 = param_1 + 0x2c;
  puVar5 = (undefined4 *)(iVar1 + -4);
  do {
    uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
    FUN_1012bb14(uVar2,iVar4,0);
    puVar5 = puVar5 + 1;
    *puVar5 = uVar2;
  } while ((undefined4 *)(iVar1 + 0xc) != puVar5);
  uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
  *(undefined4 *)(iVar1 + 0x40) = uVar2;
  FUN_1012bb14(uVar2,iVar4,0);
  lv_img(*(undefined4 *)(iVar1 + 0x40),iVar1 + 0x44);
  uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
  *(undefined4 *)(iVar1 + 0x50) = uVar2;
  FUN_1012bb14(uVar2,iVar4,0);
  uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
  FUN_1012bb14(uVar2,iVar4,0);
  uVar3 = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(iVar1 + 0x60) = uVar2;
  uVar2 = FUN_1013ed38(uVar3);
  FUN_1012bb14(uVar2,iVar4,0);
  *(undefined4 *)(iVar1 + 100) = uVar2;
  return 0;
}


