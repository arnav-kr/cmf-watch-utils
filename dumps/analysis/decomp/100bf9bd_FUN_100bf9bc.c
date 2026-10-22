// entry 0x100bf9bd  FUN_100bf9bc  (100bf9bc)

void FUN_100bf9bc(void)

{
  byte bVar1;
  undefined4 *puVar2;
  uint uVar3;
  undefined4 uVar4;
  
  puVar2 = DAT_100bf9f0;
  uVar3 = FUN_101cb8a2(*DAT_100bf9f0);
  if (uVar3 < 0x14d) {
    bVar1 = *DAT_100bf9f4;
    *DAT_100bf9f4 = bVar1 + 1;
    if (4 < (byte)(bVar1 + 1)) {
      FUN_101898bc(0x87,0);
    }
  }
  else {
    *DAT_100bf9f4 = 0;
  }
  uVar4 = FUN_101cb89a();
  *puVar2 = uVar4;
  return;
}


