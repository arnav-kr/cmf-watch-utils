// entry 0x100cbf84  FUN_100cbf84

void FUN_100cbf84(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  char local_24;
  ushort local_23;
  ushort local_21;
  ushort local_1f;
  ushort local_1d;
  ushort local_1b;
  
  piVar1 = DAT_100cc044;
  uVar4 = (DAT_100cc040 - DAT_100cc03c) * 0x20 & 0xff00;
  iVar5 = *DAT_100cc044;
  FUN_1014c818(uVar4 | 0xe80031,DAT_100cc048);
  uVar2 = DAT_100cc04c;
  do {
    while (((int)((uint)*(byte *)(iVar5 + 4) << 0x1f) < 0 &&
           (iVar3 = FUN_10147748(*(undefined4 *)(*piVar1 + 0x18),&local_24,0xb), 0 < iVar3))) {
      local_23 = local_23 << 8 | local_23 >> 8;
      local_21 = local_21 << 8 | local_21 >> 8;
      local_1f = local_1f << 8 | local_1f >> 8;
      local_1d = local_1d << 8 | local_1d >> 8;
      local_1b = local_1b << 8 | local_1b >> 8;
      FUN_1014c818(uVar4 | 0xd10034,uVar2,local_24,local_21,local_1b,local_23);
      if (local_24 == -0xb) {
        FUN_100d343c(&local_24);
      }
    }
    FUN_101be1ec(10);
  } while( true );
}


