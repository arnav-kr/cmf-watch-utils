// 0x1023e878 qrcodegen_4 :: qrcodegen_VERSION_MIN <= ver && ver <= qrcodegen_VERSION_MAX
// entry 0x101346f4  qrcodegen_4

int qrcodegen_4(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_1 - 1U < 0x28) {
    iVar1 = (param_1 * (param_1 + 8) + 4) * 0x10;
    if (param_1 != 1) {
      iVar2 = param_1 / 7 + 2;
      iVar1 = iVar2 * (iVar2 * -0x19 + 10) + iVar1;
      if (param_1 < 7) {
        iVar1 = iVar1 + 0x37;
      }
      else {
        iVar1 = iVar1 + 0x13;
      }
    }
    return iVar1;
  }
                    /* WARNING: Subroutine does not return */
  FUN_101a9a68(DAT_10134748,0x151,DAT_10134744,DAT_10134740,param_4);
}


