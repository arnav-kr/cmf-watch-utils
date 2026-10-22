// entry 0x1007f558  FUN_1007f558

uint FUN_1007f558(uint param_1)

{
  uint uVar1;
  byte *pbVar2;
  
  uVar1 = FUN_1007ebd4();
  pbVar2 = (byte *)(DAT_1007f594 + uVar1);
  while ((uVar1 != 0 && (*pbVar2 != param_1))) {
    uVar1 = uVar1 - 1 & 0xff;
    pbVar2 = pbVar2 + -1;
  }
  FUN_1014c818(DAT_1007f5a0 | (DAT_1007f59c - DAT_1007f598) * 0x20 & 0xff00U,DAT_1007f5a4,param_1,
               uVar1);
  return uVar1;
}


