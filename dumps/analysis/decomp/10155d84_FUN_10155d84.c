// entry 0x10155d84  FUN_10155d84

int FUN_10155d84(int param_1,uint param_2,undefined4 param_3)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  
  puVar1 = DAT_10155e3c;
  uStack_1c = param_2 & 0xffff;
  iVar6 = param_1;
  uStack_18 = param_3;
  if (0 < *(int *)(param_1 + 4)) {
    iVar4 = FUN_10155c38();
    uVar7 = *(uint *)(param_1 + 4);
    if (iVar4 == 0) {
      uVar5 = FUN_101b605a((int)&uStack_1c + 2,*(undefined4 *)(param_1 + 8),uVar7 & 0xffff,param_2);
      uVar3 = uStack_1c._2_2_;
      FUN_101ddb66(puVar1 + 0xb,uVar5,uStack_1c._2_2_,0x200);
      *(ushort *)(puVar1 + 1) = uVar3 << 8 | uVar3 >> 8;
    }
    else {
      uStack_1c = CONCAT22((short)uVar7,(undefined2)uStack_1c);
      FUN_101ddb66(puVar1 + 0xb,*(undefined4 *)(param_1 + 8),uVar7 & 0xffff,0x200);
      *(ushort *)(puVar1 + 1) = (ushort)((uVar7 & 0xff) << 8) | (ushort)((uVar7 & 0xffff) >> 8);
    }
  }
  *puVar1 = 0xf5;
  puVar1[3] = 0xff;
  puVar1[4] = 0xff;
  puVar1[5] = 0;
  puVar1[7] = 0;
  puVar1[6] = 1;
  puVar1[8] = 1;
  puVar2 = DAT_10155e3c;
  uVar7 = (param_2 >> 0x10) << 0x18 | (param_2 >> 0x18) << 0x10 | (param_2 & 0xff) << 8 |
          param_2 >> 8 & 0xff;
  *(short *)(puVar1 + 9) = (short)uVar7;
  iVar6 = FUN_10155bf8(param_1,puVar2,(uStack_1c >> 0x10) + 0xb,uVar7,iVar6);
  if (iVar6 != 0) {
    FUN_1014c818(DAT_10155e48 | (DAT_10155e44 - DAT_10155e40) * 0x20 & 0xff00U,DAT_10155e4c,param_2,
                 iVar6);
  }
  return iVar6;
}


