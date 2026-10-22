// entry 0x100cd324  FUN_100cd324

void FUN_100cd324(uint param_1,uint param_2,uint param_3,uint param_4,short param_5,
                 undefined4 param_6)

{
  undefined1 *puVar1;
  undefined4 uVar2;
  uint uVar3;
  ushort local_22 [3];
  
  local_22[0] = 0;
  uVar3 = (DAT_100cd3e0 - DAT_100cd3e4) * 0x20 & 0xff00;
  FUN_1014c818(uVar3 | 0x2f80014,DAT_100cd3e8,param_3,param_4,param_5,param_1);
  if (param_5 != 0) {
    FUN_101ddb66(DAT_100cd3ec,param_6,param_5,0x200);
    uVar2 = FUN_101b605a(local_22,DAT_100cd3ec,param_5,param_1);
    FUN_1014c818(DAT_100cd3f0 | uVar3,DAT_100cd3f4,local_22[0]);
    FUN_101ddb66(DAT_100cd3ec,uVar2,local_22[0],0x200);
  }
  puVar1 = DAT_100cd3f8;
  *DAT_100cd3f8 = 0xf5;
  *(ushort *)(puVar1 + 1) = local_22[0] << 8 | local_22[0] >> 8;
  *(ushort *)(puVar1 + 3) = (ushort)((param_1 & 0xff) << 8) | (ushort)(param_1 >> 8) & 0xff;
  *(ushort *)(puVar1 + 5) = (ushort)((param_3 & 0xff) << 8) | (ushort)(param_3 >> 8) & 0xff;
  *(ushort *)(puVar1 + 7) = (ushort)((param_4 & 0xff) << 8) | (ushort)(param_4 >> 8) & 0xff;
  *(ushort *)(puVar1 + 9) = (ushort)((param_2 & 0xff) << 8) | (ushort)(param_2 >> 8) & 0xff;
  FUN_100cc2d4(puVar1,local_22[0] + 0xb);
  FUN_1014c818(DAT_100cd3fc | uVar3,DAT_100cd400);
  return;
}


