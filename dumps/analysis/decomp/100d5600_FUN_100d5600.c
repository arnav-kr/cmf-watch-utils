// entry 0x100d5600  FUN_100d5600

undefined4 FUN_100d5600(int param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  int *piVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  uint local_20;
  uint uStack_1c;
  
  piVar3 = (int *)thunk_FUN_10146a3c(0x1c,DAT_100d5694);
  if (piVar3 == (int *)0x0) {
    uVar4 = 0xffffffff;
  }
  else {
    uVar1 = *(ushort *)(param_1 + 2);
    iVar5 = *(int *)(param_1 + 8);
    piVar3[4] = param_2;
    *(ushort *)(piVar3 + 6) = uVar1;
    *piVar3 = iVar5;
    *(int **)(param_2 + 0xc) = piVar3;
    iVar2 = DAT_100d5698;
    if ((iVar5 == 0) || ((int)((uint)*(byte *)(param_1 + 1) << 0x1f) < 0)) {
      uVar4 = 0;
    }
    else {
      uVar6 = 1 << (uVar1 & 0x1f) & *(uint *)(DAT_100d5698 + (uint)(uVar1 >> 5) * 4);
      if (uVar6 == 0) {
        local_20 = uVar6;
        uStack_1c = uVar6;
        (**(code **)(param_1 + 0x18))((uint)uVar1,&local_20);
        uVar4 = FUN_100dfb88();
        FUN_1018566c(*(undefined4 *)(param_1 + 8),local_20,uStack_1c,0,DAT_100d56a0,DAT_100d569c,
                     uVar4);
        uVar6 = (uint)(*(ushort *)(param_1 + 2) >> 5);
        *(uint *)(iVar2 + uVar6 * 4) =
             1 << (*(ushort *)(param_1 + 2) & 0x1f) | *(uint *)(iVar2 + uVar6 * 4);
      }
      uVar4 = FUN_10185810(*(undefined4 *)(param_1 + 8),*(undefined2 *)(param_1 + 2),0);
    }
  }
  return uVar4;
}


