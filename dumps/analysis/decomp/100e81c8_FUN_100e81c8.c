// entry 0x100e81c8  FUN_100e81c8

undefined4 FUN_100e81c8(void)

{
  int *piVar1;
  int iVar2;
  int extraout_r2;
  int extraout_r3;
  int iVar3;
  undefined4 in_r3;
  undefined8 uVar4;
  
  iVar2 = 0;
  piVar1 = DAT_100e81d0;
  iVar3 = DAT_100e606c;
  while( true ) {
    uVar4 = FUN_101b9efe(iVar3,piVar1,iVar2,iVar3,in_r3);
    piVar1 = (int *)((ulonglong)uVar4 >> 0x20);
    if (((int)uVar4 != 0) &&
       ((*(int *)(extraout_r3 + 4) == 0 ||
        ((piVar1 != (int *)0x0 && (*(int *)(extraout_r3 + 4) == *piVar1)))))) break;
    iVar2 = extraout_r2 + 1;
    iVar3 = extraout_r3 + 0x28;
    if (iVar2 == 0x14) {
      return 0xffffffff;
    }
  }
  work_11(DAT_100e6074,DAT_100e6070);
  return 0;
}


