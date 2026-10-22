// entry 0x100eb0c4  FUN_100eb074  (100eb074)

int FUN_100eb074(int param_1,uint param_2,undefined4 param_3)

{
  undefined1 *puVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  
  uStack_1c = param_2 & 0xffff;
  uStack_18 = param_3;
  if (param_1 != 0) {
    uVar3 = FUN_101b605a((int)&uStack_1c + 2,DAT_100eb10c,param_1,param_2,param_1);
    FUN_101ddb66(DAT_100eb10c,uVar3,uStack_1c._2_2_,0x40);
  }
  puVar1 = DAT_100eb110;
  *DAT_100eb110 = 0xf5;
  *(ushort *)(puVar1 + 1) = uStack_1c._2_2_ << 8 | uStack_1c._2_2_ >> 8;
  puVar1[3] = 0xff;
  puVar1[4] = 0xff;
  puVar1[6] = 1;
  puVar1[8] = 1;
  *(ushort *)(puVar1 + 9) = (ushort)((param_2 & 0xff) << 8) | (ushort)(param_2 >> 8) & 0xff;
  uVar2 = DAT_100eb11c;
  uVar5 = (DAT_100eb114 - DAT_100eb118) * 0x20 & 0xff00;
  puVar1[5] = 0;
  puVar1[7] = 0;
  FUN_1014c818(uVar2 | uVar5,DAT_100eb120,param_2,param_1);
  iVar4 = FUN_100eb024(puVar1,(uStack_1c >> 0x10) + 0xb);
  if (iVar4 != 0) {
    FUN_1014c818(DAT_100eb124 | uVar5,DAT_100eb128,param_2,iVar4);
  }
  return iVar4;
}


