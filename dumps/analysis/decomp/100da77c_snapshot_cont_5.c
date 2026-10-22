// 0x10223a8c snapshot_cont_5 :: snapshot_cont_update_snapshot
// entry 0x100da77c  snapshot_cont_5

void snapshot_cont_5(int param_1)

{
  int iVar1;
  uint uVar2;
  
  if (param_1 == 0) {
    FUN_1013a098(3,DAT_100da874,0x10c,DAT_100da878,DAT_100da870,DAT_100da86c,DAT_100da868);
    FUN_101ba194(DAT_100da880,DAT_100da87c,DAT_100da874,0x10c);
    FUN_101ba194(DAT_100da884);
    FUN_100eeb30(DAT_100da874,0x10c);
  }
  iVar1 = FUN_101c54c2(param_1,DAT_100da888);
  if (iVar1 == 0) {
    FUN_1013a098(3,DAT_100da874,0x10c,DAT_100da878,DAT_100da870,DAT_100da890,DAT_100da88c);
    FUN_101ba194(DAT_100da880,DAT_100da87c,DAT_100da874,0x10c);
    FUN_101ba194(DAT_100da884);
    FUN_100eeb30(DAT_100da874,0x10c);
  }
  if ((int)((uint)*(byte *)(param_1 + 0x53) << 0x1f) < 0) {
    *(byte *)(param_1 + 0x53) = *(byte *)(param_1 + 0x53) & 0xfe;
    if (*(int *)(param_1 + 8) != 0) {
      for (uVar2 = 0; uVar2 < (uint)(*(int **)(param_1 + 8))[1]; uVar2 = uVar2 + 1) {
        lv_obj_1(*(undefined4 *)(**(int **)(param_1 + 8) + uVar2 * 4),1);
      }
    }
    snapshot_cont_2(param_1,param_1 + 0x2c);
    *(byte *)(param_1 + 0x53) = *(byte *)(param_1 + 0x53) | 1;
    if (*(int *)(param_1 + 8) != 0) {
      for (uVar2 = 0; uVar2 < (uint)(*(int **)(param_1 + 8))[1]; uVar2 = uVar2 + 1) {
        lv_obj_11(*(undefined4 *)(**(int **)(param_1 + 8) + uVar2 * 4),1);
      }
    }
  }
  return;
}


