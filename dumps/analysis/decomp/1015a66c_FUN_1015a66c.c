// entry 0x1015a66c  FUN_1015a66c

undefined4 FUN_1015a66c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  uint local_c;
  undefined4 uStack_8;
  
  if (param_1 == 0) {
    local_c = 0;
  }
  else {
    param_4 = 0xa5a5a5a5;
    local_c = 0xa5a5a5a5;
  }
  uStack_8 = param_3;
  uVar1 = FUN_1010e1e4(DAT_1015a6b8,&local_c,4,param_4,param_1);
  uVar2 = uVar1;
  if (-1 < (int)uVar1) {
    uVar2 = DAT_1015a6c4;
  }
  uVar4 = (DAT_1015a6c0 - DAT_1015a6bc) * 0x20 & 0xff00;
  if (-1 < (int)uVar1) {
    uVar4 = uVar2 | uVar4;
    uVar3 = DAT_1015a6cc;
    uVar1 = local_c;
  }
  else {
    uVar4 = uVar4 | 0x2b40011;
    uVar3 = DAT_1015a6c8;
  }
  FUN_1014c818(uVar4,uVar3,uVar1);
  return 0;
}


