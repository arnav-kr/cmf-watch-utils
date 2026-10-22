// entry 0x100e7350  FUN_100e7350

int FUN_100e7350(void)

{
  int *piVar1;
  int extraout_r2;
  int iVar2;
  int iVar3;
  int iVar4;
  int extraout_r3;
  undefined4 in_r3;
  undefined8 uVar5;
  
  FUN_1014c818((DAT_100e7378 - DAT_100e737c) * 0x20 & 0xff00U | 0x1180031,DAT_100e7384,DAT_100e7380,
               DAT_100e737c,in_r3);
  iVar2 = DAT_100e5edc;
  iVar4 = 0;
  piVar1 = DAT_100e7388;
  iVar3 = DAT_100e5edc;
  while( true ) {
    uVar5 = FUN_101b9efe(iVar3,piVar1,iVar3,iVar4);
    piVar1 = (int *)((ulonglong)uVar5 >> 0x20);
    if (((int)uVar5 != 0) &&
       ((*(int *)(extraout_r2 + 4) == 0 ||
        ((piVar1 != (int *)0x0 && (*(int *)(extraout_r2 + 4) == *piVar1)))))) break;
    iVar4 = extraout_r3 + 1;
    iVar3 = extraout_r2 + 0x28;
    if (iVar4 == 0x14) {
      return -1;
    }
  }
  iVar2 = extraout_r3 * 0x28 + iVar2;
  *(byte *)(iVar2 + 0x20) = *(byte *)(iVar2 + 0x20) & 0xfe;
  return extraout_r3;
}


