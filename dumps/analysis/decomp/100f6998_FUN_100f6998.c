// 0x1022d4e5 FUN_100f6998 :: \n%%ZEPHYR_TOOLS%%\gcc-arm-none-eabi-9-2020-q2-update-win32\bin\arm-none-eabi-addr2line.exe -e zephyr.elf -a -f 
// entry 0x100f6998  FUN_100f6998

void FUN_100f6998(int *param_1,int *param_2,int *param_3,int param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  
  if (param_1 <= param_3) {
    FUN_101ba194(DAT_100f6a44);
    uVar3 = DAT_100f6a60;
    uVar1 = DAT_100f6a58;
    if (param_4 != 0) {
      FUN_101ba194(DAT_100f6a48,*(undefined4 *)(param_4 + 0x18),*(undefined4 *)(param_4 + 0x14));
      FUN_100f6960(*(undefined4 *)(param_4 + 0x18));
      FUN_100f6960(*(undefined4 *)(param_4 + 0x14));
      uVar3 = DAT_100f6a60;
      uVar1 = DAT_100f6a58;
    }
    for (; param_2 < param_3; param_2 = param_2 + 1) {
      iVar4 = *param_2;
      if (iVar4 + 0xefffffffU <= uVar3) {
        FUN_101ba194(uVar1,iVar4);
        FUN_100f6960(iVar4);
      }
    }
    FUN_101ba194(DAT_100f6a4c);
    FUN_101ba194(DAT_100f6a54,DAT_100f6a50,param_1);
    uVar2 = DAT_100f6a5c;
    uVar1 = DAT_100f6a58;
    uVar3 = 0;
    do {
      FUN_101ba194(uVar1,param_1[uVar3]);
      if ((uVar3 & 7) == 7) {
        FUN_101ba194(uVar2);
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 != 0x80);
    FUN_101ba194(DAT_100f6a4c);
    return;
  }
  FUN_101ba194(DAT_100f6a40,param_1,param_2,param_3);
  return;
}


