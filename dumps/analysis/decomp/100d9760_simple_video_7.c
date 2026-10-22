// entry 0x100d9760  simple_video_7  (100d9760)

void simple_video_7(int param_1)

{
  ushort uVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  undefined2 uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  bool bVar9;
  
  iVar6 = *(int *)(param_1 + 0xc);
  if (((*(int *)(iVar6 + 0xd0) == 0) || (*(int *)(iVar6 + 0xd8) == 0)) ||
     ((*(byte *)(iVar6 + 0x55) & 5) == 0)) {
    return;
  }
  uVar1 = *(ushort *)(iVar6 + 200);
  uVar7 = (uint)uVar1;
  uVar8 = uVar7 + 2;
  *(byte *)(iVar6 + 0x55) = *(byte *)(iVar6 + 0x55) & 0xfb;
  uVar2 = FUN_10107238(iVar6 + 0x9c,*(int *)(iVar6 + 0xd0),uVar8);
  if (uVar2 < uVar7) {
    FUN_1013a098(3,DAT_100d9860,0xa2,DAT_100d985c,DAT_100d9858,uVar7);
  }
  else {
    bVar9 = uVar8 != uVar2;
    if (bVar9) {
      uVar2 = 0;
    }
    piVar3 = *(int **)(iVar6 + 0xd0);
    *(ushort *)(iVar6 + 0xcc) = uVar1;
    uVar5 = (undefined2)uVar2;
    if (!bVar9) {
      uVar5 = CONCAT11(*(undefined1 *)((int)piVar3 + uVar7 + 1),*(undefined1 *)((int)piVar3 + uVar7)
                      );
    }
    *(undefined2 *)(iVar6 + 200) = uVar5;
    if (*piVar3 == DAT_100d9864) {
      uVar2 = (*(uint *)(iVar6 + 0x30) & 0x1fffff) >> 10;
      FUN_100ffcc4(piVar3,uVar7,*(undefined4 *)(iVar6 + 0xd8),1,uVar2,0,0,uVar2,
                   *(ushort *)(iVar6 + 0x32) >> 5);
      if (*(short *)(iVar6 + 200) != 0) {
LAB_100d9842:
        lv_obj_pos_42(iVar6);
        return;
      }
      if ((int)((uint)*(byte *)(iVar6 + 0x55) << 0x1c) < 0) {
        iVar4 = simple_video_6();
        *(short *)(iVar6 + 200) = (short)iVar4;
        if (iVar4 != 0) goto LAB_100d9842;
      }
      else {
        lv_obj_pos_42(iVar6);
      }
    }
    else {
      FUN_1013a098(3,DAT_100d9860,0xab,DAT_100d985c,DAT_100d9868);
    }
  }
  *(byte *)(iVar6 + 0x55) = *(byte *)(iVar6 + 0x55) & 0xfe;
  FUN_101cc7a6(param_1);
  return;
}


