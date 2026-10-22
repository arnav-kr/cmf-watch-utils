// 0x1021b870 FUN_1009a0a4 :: album_wf_layout_create
// entry 0x1009a0a4  FUN_1009a0a4

undefined4 FUN_1009a0a4(int *param_1,undefined4 param_2,undefined4 *param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  byte bVar5;
  uint uVar6;
  
  puVar1 = (undefined4 *)thunk_FUN_10146a3c(0x5c,DAT_1009a190);
  if (puVar1 == (undefined4 *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    *puVar1 = param_2;
    *param_1 = (int)puVar1;
    *(undefined4 *)((int)puVar1 + 0x3a) = *param_3;
    *(undefined4 *)((int)puVar1 + 0x3e) = param_3[1];
    iVar3 = DAT_1009a194;
    *(undefined2 *)((int)puVar1 + 0x42) = *(undefined2 *)(param_3 + 2);
    *(undefined1 *)(puVar1 + 0xe) = 1;
    bVar5 = *(byte *)((int)puVar1 + 0x3a) & 3;
    uVar6 = (iVar3 - DAT_1009a198) * 0x20 & 0xff00;
    *(byte *)((int)puVar1 + 0x3a) = bVar5;
    FUN_1014c818(uVar6 | 0x780033,DAT_1009a19c,bVar5,(int)*(short *)((int)puVar1 + 0x3e),
                 (int)*(short *)(puVar1 + 0x10));
    uVar2 = FUN_100dfb88();
    iVar3 = FUN_10184804(DAT_1009a1a8,puVar1 + 5,DAT_1009a1a0,DAT_1009a1a4,uVar2);
    if (iVar3 < 0) {
      FUN_1014c818(uVar6 | 0x380011,DAT_1009a1ac);
    }
    lv_style();
    FUN_101cc72c(puVar1 + 0xb,*(undefined2 *)((int)puVar1 + 0x42));
    FUN_101cc73e(puVar1 + 0xb,0xff);
    FUN_101c6474(*puVar1,0);
    uVar2 = FUN_1013ed38(*puVar1);
    puVar1[1] = uVar2;
    uVar4 = FUN_10127cfc(*puVar1);
    uVar2 = DAT_1009a1b0;
    puVar1[2] = uVar4;
    lv_obj_pos_5(uVar4,uVar2);
    FUN_101c5de4(puVar1[2],1,(int)*(short *)((int)puVar1 + 0x3e),(int)*(short *)(puVar1 + 0x10));
    lv_obj_11(puVar1[2],0x4000);
    uVar2 = 0;
  }
  return uVar2;
}


