// 0x1022113a FUN_100d0e50 :: updateLen:%d \n
// entry 0x100d0e50  FUN_100d0e50

void FUN_100d0e50(uint param_1,uint param_2,undefined4 param_3)

{
  undefined1 *puVar1;
  int iVar2;
  undefined4 uVar3;
  ushort uVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  undefined1 *puVar8;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  
  iVar2 = DAT_100d0f64;
  puVar1 = DAT_100d0f60;
  puVar8 = DAT_100d0f60 + 0xb;
  uStack_2c = param_2 & 0xffff;
  uStack_28 = param_3;
  FUN_101ddb66(puVar8,param_2,param_3,0x200,param_1);
  uVar3 = DAT_100d0fa0;
  uVar7 = (iVar2 - DAT_100d0f68) * 0x20 & 0xff00;
  FUN_1014c818(uVar7 | 0x3200031,DAT_100d0fa0,puVar1[0xb]);
  uVar5 = FUN_101b605a((int)&uStack_2c + 2,puVar8,param_3,param_1);
  FUN_1014c818(DAT_100d0f6c | uVar7,DAT_100d0f70,uStack_2c._2_2_);
  uVar4 = uStack_2c._2_2_;
  FUN_101ddb66(puVar8,uVar5,uStack_2c._2_2_,0x200);
  *puVar1 = 0xf5;
  *(ushort *)(puVar1 + 1) = uVar4 << 8 | uVar4 >> 8;
  puVar1[3] = 0xff;
  puVar1[4] = 0xff;
  uVar6 = DAT_100d0f74 | uVar7;
  puVar1[6] = 1;
  puVar1[8] = 1;
  puVar1[5] = 0;
  puVar1[7] = 0;
  *(ushort *)(puVar1 + 9) = (ushort)((param_1 & 0xff) << 8) | (ushort)(param_1 >> 8) & 0xff;
  FUN_1014c818(uVar6,uVar3,puVar1[0xb]);
  FUN_1014c818(DAT_100d0f78 | uVar7,DAT_100d0f7c,*puVar1);
  FUN_1014c818(uVar7 | 0x3300031,DAT_100d0f80,*(undefined2 *)(puVar1 + 1));
  FUN_1014c818(DAT_100d0f84 | uVar7,DAT_100d0f88,*(undefined2 *)(puVar1 + 3));
  FUN_1014c818(DAT_100d0f8c | uVar7,DAT_100d0f90,*(undefined2 *)(puVar1 + 5));
  FUN_1014c818(DAT_100d0f94 | uVar7,DAT_100d0f98,*(undefined2 *)(puVar1 + 7));
  FUN_1014c818(uVar7 | 0x3340031,DAT_100d0f9c,*(undefined2 *)(puVar1 + 9));
  FUN_100cc2d4(puVar1,(uStack_2c >> 0x10) + 0xb);
  return;
}


