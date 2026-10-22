// entry 0x1007b016  FUN_1007b016  (1007b016)

void FUN_1007b016(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int unaff_r4;
  
  FUN_101aa780(param_1,param_2,DAT_1007b14c);
  simple_video_8(*(undefined4 *)(unaff_r4 + 0x60));
  simple_video_10(*(undefined4 *)(unaff_r4 + 0x60),1);
  simple_video_9(*(undefined4 *)(unaff_r4 + 0x60),1);
  if (*(int *)(unaff_r4 + 0x74) == 0) {
    simple_img_6(*(undefined4 *)(unaff_r4 + 0x24),unaff_r4 + 0x1e8);
    iVar1 = unaff_r4 + 0x20c;
  }
  else {
    simple_img_6(*(undefined4 *)(unaff_r4 + 0x24),unaff_r4 + 0x1e8);
    iVar1 = unaff_r4 + 0x200;
  }
  simple_img_6(*(undefined4 *)(unaff_r4 + 0x28),iVar1);
  lv_obj_23(*(undefined4 *)(unaff_r4 + 0x30),1);
  *(undefined4 *)(unaff_r4 + 100) = 2;
  *(undefined4 *)(unaff_r4 + 0x68) = 0;
  simple_img_6(*(undefined4 *)(unaff_r4 + 0x1c),unaff_r4 + 0x1b8);
  return;
}


