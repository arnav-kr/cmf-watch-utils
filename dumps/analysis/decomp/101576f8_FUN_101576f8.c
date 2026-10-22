// 0x10244dbe FUN_101576f8 :: SAVE FIRMWARE_VERSION FAILD\n
// entry 0x101576f8  FUN_101576f8

undefined4 FUN_101576f8(int param_1,int param_2)

{
  int iVar1;
  int local_10;
  int local_c;
  
  local_10 = param_1;
  local_c = param_2;
  if (param_2 - 10U < 3) {
    iVar1 = FUN_100f731c();
    local_10 = *(int *)(iVar1 + 4);
    local_c = *(undefined4 *)(param_1 + 0x34);
  }
  else {
    if (param_2 != 4) {
      return 0;
    }
    iVar1 = FUN_100f731c();
    local_c = *(undefined4 *)(iVar1 + 8);
    local_10 = *(int *)(param_1 + 0x30);
  }
  iVar1 = FUN_1010e1b4(DAT_10157754,&local_10,8);
  if (iVar1 == 0) {
    return 0;
  }
  FUN_1014c818((DAT_1015775c - DAT_10157758) * 0x20 & 0xff00U | 0xa20011,DAT_10157760);
  return 0xffffffff;
}


