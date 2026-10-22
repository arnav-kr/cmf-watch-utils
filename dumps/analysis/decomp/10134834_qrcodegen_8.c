// 0x1023e922 qrcodegen_8 :: (qrcodegen_VERSION_MIN * 4 + 17) <= result && result <= (qrcodegen_VERSION_MAX * 4 + 17)
// entry 0x10134834  qrcodegen_8

void qrcodegen_8(byte *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  if (param_1 == (byte *)0x0) {
    uVar1 = 0x2e4;
    uVar2 = DAT_1013485c;
  }
  else {
    if (*param_1 - 0x15 < 0x9d) {
      return;
    }
    uVar1 = 0x2e6;
    uVar2 = DAT_10134868;
  }
                    /* WARNING: Subroutine does not return */
  FUN_101a9a68(DAT_10134864,uVar1,DAT_10134860,uVar2,param_4);
}


