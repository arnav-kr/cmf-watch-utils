// entry 0x100f1058  FUN_100f1058  (100f1058)

undefined2 FUN_100f1058(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  bool bVar1;
  uint uVar2;
  undefined2 uVar3;
  int iVar4;
  undefined4 uVar5;
  
  uVar5 = 0;
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    uVar5 = getBasePriority();
  }
  bVar1 = (bool)isCurrentModePrivileged();
  if ((bVar1) && (uVar2 = getBasePriority(), uVar2 == 0 || 0x20 < uVar2)) {
    setBasePriority(0x20);
  }
  InstructionSynchronizationBarrier(0xf);
  iVar4 = FUN_100f0cb0(param_1,param_1,param_3,0x20,param_4);
  if (iVar4 == 0) {
    FUN_101ba194(DAT_100f108c);
    uVar3 = 0;
  }
  else {
    uVar3 = *(undefined2 *)(iVar4 + 2);
  }
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setBasePriority(uVar5);
  }
  InstructionSynchronizationBarrier(0xf);
  return uVar3;
}


