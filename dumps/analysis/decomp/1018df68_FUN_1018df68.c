// 0x101fdd74 FUN_1018df68 :: view %d update cancel\n
// entry 0x1018df68  FUN_1018df68

int FUN_1018df68(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  char *pcVar2;
  uint uVar3;
  int *piVar4;
  
  piVar4 = (int *)*DAT_1018dff4;
  while( true ) {
    if (piVar4 == (int *)0x0) {
      return -3;
    }
    if (*(ushort *)(piVar4[1] + 0x10) == param_1) break;
    piVar4 = (int *)*piVar4;
  }
  uVar3 = (uint)*(byte *)(piVar4 + 4);
  if ((uVar3 & 0x28) == 0x28) {
    FUN_1014c818((DAT_1018dff8 - DAT_1018dffc) * 0x20 & 0xff00U | 0x1d20031,DAT_1018e000,param_1);
    iVar1 = 0;
  }
  else {
    if (-1 < (int)(uVar3 << 0x1c)) {
      iVar1 = FUN_1018dafc();
      return iVar1;
    }
    if ((int)(uVar3 << 0x1e) < 0) {
      iVar1 = FUN_1018bc4c(param_1,0xb,0,uVar3 << 0x1e,param_4);
      if (iVar1 == 0) {
        pcVar2 = (char *)FUN_1018b1ac();
        if ((pcVar2 != (char *)0x0) && (*pcVar2 != '\0')) {
          FUN_1018e71c(piVar4);
        }
        *(byte *)(piVar4 + 4) = *(byte *)(piVar4 + 4) | 0x20;
        return 0;
      }
    }
    else {
      iVar1 = -0x8c;
    }
  }
  return iVar1;
}


