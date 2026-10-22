// entry 0x100ccb34  FUN_100ccb34

undefined4 FUN_100ccb34(undefined2 *param_1,undefined4 param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined1 *puVar4;
  uint uVar5;
  undefined1 auStack_d8 [180];
  
  uVar5 = 0x10 - (param_3 + 4U & 0xf);
  if (0x1ff < (int)(param_3 + 4U + uVar5)) {
    return 0;
  }
  uVar1 = FUN_100ccafc(param_2,param_3,0);
  iVar2 = DAT_100ccbf8;
  FUN_101ddb66(DAT_100ccbf8,param_2,param_3,0x200);
  *(char *)(iVar2 + param_3) = (char)uVar1;
  *(char *)(iVar2 + param_3 + 1) = (char)((uint)uVar1 >> 8);
  *(char *)(iVar2 + param_3 + 2) = (char)((uint)uVar1 >> 0x10);
  *(char *)(iVar2 + param_3 + 3) = (char)((uint)uVar1 >> 0x18);
  puVar4 = (undefined1 *)(iVar2 + param_3 + 3);
  iVar2 = 0;
  do {
    iVar2 = iVar2 + 1;
    puVar4 = puVar4 + 1;
    *puVar4 = (char)uVar5;
  } while (iVar2 < (int)(uVar5 & 0xff));
  uVar5 = param_3 + uVar5 + 4;
  uVar3 = uVar5 & 0xffff;
  if (uVar3 + 0x10 < 0x200) {
    if (param_4 != 0) {
      FUN_100fdb68(auStack_d8,param_4);
    }
    iVar2 = FUN_101bda5a(DAT_100ccc00,uVar3 + 0x10,DAT_100ccbf8,uVar3,DAT_100ccbfc,auStack_d8);
    if (iVar2 == 1) {
      FUN_101ddb66(DAT_100ccc08,DAT_100ccc04,uVar3,0x200);
      uVar1 = DAT_100ccc08;
      goto LAB_100ccbb2;
    }
  }
  uVar1 = 0;
LAB_100ccbb2:
  *param_1 = (short)uVar5;
  return uVar1;
}


