// 0x101fe304 FUN_1018f268 :: surface update failed
// entry 0x1018f268  FUN_1018f268

uint FUN_1018f268(int param_1,undefined4 *param_2,undefined4 param_3)

{
  short sVar1;
  undefined1 uVar2;
  short sVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  bool bVar6;
  code *pcVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  undefined4 extraout_r1;
  undefined4 uVar11;
  undefined4 unaff_r4;
  undefined4 *puVar12;
  undefined4 unaff_r5;
  undefined4 unaff_r6;
  undefined4 unaff_r7;
  undefined4 unaff_lr;
  undefined4 uVar13;
  
  puVar12 = *(undefined4 **)(param_1 + 0x5c);
  if (((int)((uint)*(byte *)(param_1 + 0x1c) << 0x1d) < 0) &&
     ((*(byte *)(param_1 + 0x1c) & 0x30) != 0)) {
    iVar8 = FUN_101cb97e(param_2);
    FUN_101bee00(param_3,iVar8 << 1);
    FUN_101beeae();
  }
  if (*(code **)(param_1 + 0x38) != (code *)0x0) {
    (**(code **)(param_1 + 0x38))(param_1);
  }
  if (-1 < (int)((uint)*(byte *)(param_1 + 0x1c) << 0x1f)) {
    iVar8 = FUN_101842a4();
    if (iVar8 == 0) {
      FUN_101c7708(*(undefined4 *)(param_1 + 0x4c));
    }
    else if (*(char *)((int)puVar12 + 0x26) != '\0') {
      do {
        FUN_101be202(puVar12 + 0xf,0xffffffff);
      } while (*(char *)((int)puVar12 + 0x26) != '\0');
    }
  }
  *(bool *)((int)puVar12 + 0x25) = *(char *)(puVar12 + 9) == '\0';
  iVar8 = FUN_101cb86e(param_1);
  if (iVar8 == 0) {
    *(char *)(puVar12 + 9) = *(char *)(puVar12 + 9) + '\x01';
    if (-1 < (int)((uint)*(byte *)(param_1 + 0x1c) << 0x1f)) goto LAB_1018f2f2;
LAB_1018f2be:
    if (-1 < (int)((uint)*(byte *)((int)puVar12 + 0x25) << 0x1e)) goto LAB_1018f33e;
    uVar9 = thunk_FUN_101d7960(*puVar12,puVar12 + 7);
    uVar10 = FUN_101d7b9c(*puVar12);
  }
  else {
    *(undefined1 *)(puVar12 + 9) = 0;
    *(byte *)((int)puVar12 + 0x25) = *(byte *)((int)puVar12 + 0x25) | 2;
    if ((int)((uint)*(byte *)(param_1 + 0x1c) << 0x1f) < 0) goto LAB_1018f2be;
LAB_1018f2f2:
    uVar4 = *(undefined2 *)(param_2 + 2);
    uVar5 = *(undefined2 *)(param_2 + 3);
    uVar11 = *param_2;
    *(undefined2 *)((int)puVar12 + 0x1e) = *(undefined2 *)(param_2 + 1);
    *(short *)(puVar12 + 7) = (short)uVar11;
    *(undefined2 *)(puVar12 + 8) = uVar4;
    *(undefined2 *)((int)puVar12 + 0x22) = uVar5;
    iVar8 = FUN_101842a4();
    pcVar7 = DAT_1018f3dc;
    if (iVar8 != 0) {
      puVar12[10] = param_3;
      *(undefined1 *)((int)puVar12 + 0x26) = 1;
      iVar8 = DAT_1014296c;
      uVar13 = 1;
      FUN_101435d8(0);
      thunk_FUN_101915ac(0);
      FUN_101428f8(*(undefined2 *)(iVar8 + 0x70));
      uVar11 = 0;
      bVar6 = (bool)isCurrentModePrivileged();
      if (bVar6) {
        uVar11 = getBasePriority();
      }
      bVar6 = (bool)isCurrentModePrivileged();
      if ((bVar6) && (uVar10 = getBasePriority(), uVar10 == 0 || 0x20 < uVar10)) {
        setBasePriority(0x20);
      }
      InstructionSynchronizationBarrier(0xf);
      uVar9 = (uint)*(ushort *)(iVar8 + 0x4c);
      uVar10 = (uint)*(ushort *)(iVar8 + 0x4e);
      if (uVar10 != uVar9) {
        *(code **)(iVar8 + 0x68) = pcVar7;
        *(undefined4 **)(iVar8 + 0x6c) = puVar12 + 0xb;
        *(ushort *)(iVar8 + 0x70) = *(ushort *)(iVar8 + 0x4c);
      }
      bVar6 = (bool)isCurrentModePrivileged();
      if (bVar6) {
        setBasePriority(uVar11);
      }
      InstructionSynchronizationBarrier(0xf);
      if (uVar10 == uVar9) {
        (*pcVar7)(puVar12 + 0xb,extraout_r1,uVar11,uVar10,uVar13,unaff_r4,unaff_r5,unaff_r6,unaff_r7
                  ,unaff_lr);
      }
      return uVar9;
    }
    sVar1 = *(short *)(puVar12 + 8);
    sVar3 = *(short *)(puVar12 + 7);
    uVar2 = *(undefined1 *)((int)puVar12 + 0x25);
    uVar11 = *puVar12;
    iVar8 = FUN_101d7662();
    if (iVar8 == 0x10) {
      uVar13 = 0x20;
    }
    else {
      uVar13 = 8;
    }
    uVar9 = FUN_10188de8(uVar11,uVar2,puVar12 + 7,param_3,(sVar1 + 1) - sVar3,uVar13);
    uVar10 = uVar9;
    if ((int)((uint)*(byte *)((int)puVar12 + 0x25) << 0x1e) < 0) {
      uVar10 = FUN_101d7b9c(*puVar12);
    }
  }
  if (uVar9 == 0) {
    return uVar10;
  }
  FUN_1014c818(DAT_1018f3e8 | (DAT_1018f3e0 - DAT_1018f3e4) * 0x20 & 0xff00U,DAT_1018f3ec);
LAB_1018f33e:
  uVar10 = FUN_101cb864(param_1);
  return uVar10;
}


