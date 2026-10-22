// entry 0x100d2d5c  FUN_100d2d5c

void FUN_100d2d5c(void)

{
  undefined1 *puVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined1 *puVar7;
  ushort local_4e;
  undefined1 local_4c;
  byte local_4b;
  undefined1 local_4a;
  undefined1 auStack_48 [44];
  
  local_4e = 0;
  uVar5 = (DAT_100d2e58 - DAT_100d2e5c) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100d2e60 | uVar5,DAT_100d2e64);
  FUN_100de67c(&local_4c,&local_4b,auStack_48,10);
  puVar1 = DAT_100d2e68;
  if (10 < local_4b) {
    local_4b = 10;
  }
  DAT_100d2e68[0xb] = 1;
  puVar1[0xc] = local_4c;
  uVar2 = DAT_100d2e74;
  puVar1[0xd] = local_4b;
  local_4a = FUN_101b91c8();
  uVar4 = (uint)local_4b;
  puVar1[0xe] = local_4a;
  FUN_101ddb66(puVar1 + 0xf,auStack_48,uVar4 << 2,0x1fc);
  uVar3 = DAT_100d2e6c;
  uVar4 = (uVar4 + 1) * 4 & 0xff;
  puVar7 = puVar1 + 0xb;
  for (iVar6 = 0; iVar6 < (int)uVar4; iVar6 = iVar6 + 1) {
    FUN_1014c818(uVar5 | uVar2,uVar3,iVar6,*puVar7);
    puVar7 = puVar7 + 1;
  }
  if (uVar4 != 0) {
    uVar3 = FUN_101b605a(&local_4e,DAT_100d2e70,uVar4,0xa055);
    FUN_101ddb66(DAT_100d2e70,uVar3,local_4e,0x200);
  }
  *puVar1 = 0xf5;
  *(ushort *)(puVar1 + 1) = local_4e << 8 | local_4e >> 8;
  puVar1[3] = 0xff;
  puVar1[4] = 0xff;
  puVar1[6] = 1;
  puVar1[8] = 1;
  puVar1[9] = 0xa0;
  puVar7 = DAT_100d2e68;
  puVar1[5] = 0;
  puVar1[7] = 0;
  puVar1[10] = 0x55;
  FUN_100cc2d4(puVar7,local_4e + 0xb);
  return;
}


