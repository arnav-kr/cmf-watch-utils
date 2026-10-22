// 0x1021b9dd FUN_1009a4f0 :: album_wf_layout_style2_create
// entry 0x1009a4f0  FUN_1009a4f0

undefined4 FUN_1009a4f0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  
  if (*(int *)(param_1 + 0xc) == 0) {
    iVar1 = thunk_FUN_10146a3c(0x70,PTR_s_album_wf_layout_style2_create_1009a58c,param_3,param_4,
                               param_4);
    *(int *)(param_1 + 0xc) = iVar1;
    if (iVar1 == 0) {
      FUN_1014c818(((int)PTR_DAT_1009a594 - (int)PTR_LAB_1009a590) * 0x20 & 0xff00U | 0xbc0011,
                   PTR_s_mllc_style2_err_____NULL_1009a598);
      return 0xffffffff;
    }
    FUN_101bf3b2(iVar1,0,0x70);
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
  uVar2 = FUN_1013ed38(*(undefined4 *)(param_1 + 8));
  FUN_1012bb14(uVar2,iVar4,0);
  uVar3 = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(iVar1 + 0x50) = uVar2;
  uVar2 = FUN_1013ed38(uVar3);
  FUN_1012bb14(uVar2,iVar4,0);
  *(undefined4 *)(iVar1 + 0x54) = uVar2;
  return 0;
}


