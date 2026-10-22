// entry 0x100dcfa8  FUN_100dcfa8  (100dcfa8)

char FUN_100dcfa8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte *pbVar1;
  uint uVar2;
  int iVar3;
  char cVar4;
  
  iVar3 = DAT_100dcfdc;
  FUN_101be218(DAT_100dcfdc + 0x84,0xffffffff,param_3,param_4,param_4);
  uVar2 = 0;
  cVar4 = '\0';
  pbVar1 = (byte *)(iVar3 + 8);
  while (uVar2 < *pbVar1) {
    uVar2 = uVar2 + 1;
    if (*(int *)(iVar3 + 0x10) != -1) {
      cVar4 = cVar4 + '\x01';
    }
    iVar3 = iVar3 + 0x10;
  }
  FUN_100dc648();
  return cVar4;
}


