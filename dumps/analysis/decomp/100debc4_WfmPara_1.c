// entry 0x100debc4  WfmPara_1

undefined4 WfmPara_1(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  uint uVar2;
  
  uVar2 = (DAT_100dec48 - DAT_100dec44) * 0x20 & 0xff00;
  FUN_1014c818(uVar2 | 0x510032,DAT_100dec50,DAT_100dec4c,*(undefined1 *)(param_1 + 1),param_4);
  if ((*(char *)(param_1 + 1) == '\0') &&
     (FUN_1014c818(uVar2 | 0x560011,DAT_100dec54), *(char *)(param_1 + 1) == '\0')) {
    FUN_101ba194(DAT_100dec60,DAT_100dec58,DAT_100dec5c,0x57);
    FUN_101ba194(DAT_100dec64);
    FUN_100eeb30(DAT_100dec5c,0x57);
  }
  *param_1 = DAT_100dec68;
  uVar1 = FUN_101b9036(param_1,0x7c);
  param_1[0x1f] = uVar1;
  FUN_101bf3aa(DAT_100dec6c,param_1,0x80);
  FUN_1014c818(uVar2 | 0x600031,DAT_100dec70,DAT_100dec4c);
  return 0;
}


