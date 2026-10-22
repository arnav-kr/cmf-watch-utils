// entry 0x100d9a18  simple_video_8  (100d9a18)

void simple_video_8(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined1 auStack_5c [7];
  undefined1 uStack_55;
  undefined2 local_54;
  undefined2 local_52;
  undefined2 uStack_50;
  ushort local_4e;
  ushort local_4c;
  undefined1 auStack_44 [44];
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_100d9c64,0x3e,DAT_100d9c68,DAT_100d9c60,DAT_100d9c5c,DAT_100d9c58);
    FUN_101ba194(DAT_100d9c70,DAT_100d9c6c,DAT_100d9c64,0x3e);
    FUN_101ba194(DAT_100d9c74);
    FUN_100eeb30(DAT_100d9c64,0x3e);
  }
  iVar1 = FUN_101c54c2(param_1,DAT_100d9c78);
  if (iVar1 == 0) {
    FUN_1013a098(3,DAT_100d9c64,0x3e,DAT_100d9c68,DAT_100d9c60,DAT_100d9c80,DAT_100d9c7c);
    FUN_101ba194(DAT_100d9c70,DAT_100d9c6c,DAT_100d9c64,0x3e);
    FUN_101ba194(DAT_100d9c74);
    FUN_100eeb30(DAT_100d9c64,0x3e);
  }
  if (param_2 == 0) {
    return;
  }
  FUN_101bf3b2(auStack_44,0);
  iVar1 = FUN_10107030(auStack_44,param_2,1);
  if (iVar1 < 0) {
    FUN_1013a098(3,DAT_100d9c64,0x49,DAT_100d9c68,DAT_100d9c84,param_2);
    return;
  }
  *(undefined4 *)(param_1 + 0x30) = 0;
  FUN_101072f0(auStack_44,0);
  uVar2 = FUN_10107238(auStack_44,auStack_5c,0x16);
  if (uVar2 < 0x16) {
    uVar4 = 0x128;
    uVar5 = DAT_100d9c88;
LAB_100d9aea:
    FUN_1013a098(3,DAT_100d9c64,uVar4,DAT_100d9c8c,uVar5);
    FUN_101071e0(auStack_44);
    return;
  }
  if (CONCAT22(uStack_50,local_52) != DAT_100d9c90) {
    uVar4 = 0x12e;
    uVar5 = DAT_100d9c94;
    goto LAB_100d9aea;
  }
  *(uint *)(param_1 + 0x30) =
       (local_4e & 0x7ff) << 10 | 1 | (uint)local_4c << 0x15 | *(uint *)(param_1 + 0x30) & 0x3e0;
  iVar1 = param_1 + 0x9c;
  FUN_101071e0(auStack_44);
  FUN_101071e0(iVar1);
  if (*(int *)(param_1 + 0xd0) == 0) {
    iVar3 = ui_mem_1(2,0x40,DAT_100d9c98,DAT_100d9c68);
    *(int *)(param_1 + 0xd0) = iVar3;
    if (iVar3 == 0) {
      uVar4 = 0x57;
      goto LAB_100d9b68;
    }
  }
  if (*(int *)(param_1 + 0xd8) == 0) {
    iVar3 = ui_mem_1(2,0x40,((*(uint *)(param_1 + 0x30) & 0x1fffff) >> 10) *
                            (uint)(*(ushort *)(param_1 + 0x32) >> 5) * 2 + 0x3f,DAT_100d9c68);
    *(int *)(param_1 + 0xd8) = iVar3;
    if (iVar3 == 0) {
      uVar4 = 0x61;
LAB_100d9b68:
      FUN_1013a098(3,DAT_100d9c64,uVar4,DAT_100d9c68,DAT_100d9c9c);
      return;
    }
  }
  FUN_101bf3b2(iVar1,0,0x2c);
  FUN_10107030(iVar1,param_2,1);
  FUN_101072f0(iVar1,10,0);
  FUN_101bf352(param_1 + 0x5c,param_2,0x40);
  *(byte *)(param_1 + 0x55) = *(byte *)(param_1 + 0x55) | 2;
  *(undefined1 *)(param_1 + 0x9b) = 0;
  *(undefined1 *)(param_1 + 0x54) = uStack_55;
  lv_obj_pos_42(param_1);
  *(uint *)(param_1 + 0x4c) = (*(uint *)(param_1 + 0x30) & 0x1fffff) >> 0xb;
  *(undefined2 *)(param_1 + 200) = local_54;
  *(uint *)(param_1 + 0x50) = (uint)(*(ushort *)(param_1 + 0x32) >> 6);
  *(byte *)(param_1 + 0x30) = *(byte *)(param_1 + 0x30) & 0xe0 | 4;
  *(byte *)(param_1 + 0x55) = *(byte *)(param_1 + 0x55) | 4;
  FUN_101bf3b2(auStack_5c,0,0x18);
  uStack_50 = (undefined2)param_1;
  local_4e = (ushort)((uint)param_1 >> 0x10);
  simple_video_7(auStack_5c);
  FUN_101b653a(param_1);
  *(uint *)(param_1 + 0x44) = ((*(uint *)(param_1 + 0x30) & 0x1fffff) >> 10) - 1;
  *(undefined4 *)(param_1 + 0x3c) = 0;
  *(undefined4 *)(param_1 + 0x40) = 0;
  *(uint *)(param_1 + 0x48) = (*(ushort *)(param_1 + 0x32) >> 5) - 1;
  FUN_101b653a(param_1);
  FUN_10129ebc(param_1);
  return;
}


