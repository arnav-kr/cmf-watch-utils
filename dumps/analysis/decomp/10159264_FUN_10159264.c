// entry 0x10159264  FUN_10159264

undefined4 FUN_10159264(int param_1,uint param_2,int param_3)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  if ((param_1 == 0) || (*(char *)(param_1 + 0x239) != '\0')) {
    uVar3 = 0xffffffff;
  }
  else {
    *(int *)(param_1 + 0x230) = param_3;
    *(undefined4 *)(param_1 + 0x234) = 0;
    *(undefined1 *)(param_1 + 0x239) = 1;
    iVar2 = DAT_101592c8;
    iVar1 = DAT_101592c4;
    *(uint *)(param_1 + 0x228) = param_2;
    *(undefined4 *)(param_1 + 0x22c) = 0;
    uVar4 = (iVar1 - iVar2) * 0x20 & 0xff00;
    *(char *)(param_1 + 0x238) = (char)((uint)(param_3 * 100) / param_2);
    FUN_1014c818(uVar4 | 0xb60031,DAT_101592cc,param_2);
    FUN_1014c818(uVar4 | 0xb70031,DAT_101592d0,*(undefined1 *)(param_1 + 0x238));
    uVar3 = 0;
  }
  return uVar3;
}


