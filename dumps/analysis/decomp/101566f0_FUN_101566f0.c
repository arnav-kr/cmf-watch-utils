// entry 0x101566f0  FUN_101566f0

undefined4 * FUN_101566f0(undefined4 param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 local_44;
  undefined4 local_40;
  undefined1 local_3c;
  undefined2 local_3b;
  undefined1 local_39;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  uVar5 = (DAT_101567b0 - DAT_101567ac) * 0x20 & 0xff00;
  FUN_1014c818(DAT_101567b4 | uVar5,DAT_101567b8);
  puVar2 = (undefined4 *)thunk_FUN_10146a3c(0x4c,DAT_101567bc);
  if (puVar2 == (undefined4 *)0x0) {
    FUN_1014c818(DAT_101567c0 | uVar5,DAT_101567c4);
  }
  else {
    FUN_101bf3b2(puVar2,0,0x4c);
    local_44 = *param_2;
    local_40 = param_2[1];
    local_3c = *(undefined1 *)(param_2 + 2);
    local_3b = 0;
    local_38 = param_2[3];
    local_39 = 0;
    local_34 = param_2[4];
    local_30 = param_2[5];
    local_2c = param_2[6];
    local_28 = DAT_101567c8;
    local_24 = param_2[7];
    local_20 = param_2[8];
    *(undefined1 *)(puVar2 + 0xc) = 0;
    local_1c = 0x1004;
    iVar3 = FUN_101609b0(&local_44);
    puVar2[0xd] = iVar3;
    if (iVar3 == 0) {
      FUN_1014c818(DAT_101567cc | uVar5,DAT_101567d0);
    }
    uVar4 = thunk_FUN_10146a3c(0x80,DAT_101567bc);
    puVar2[4] = 0x80;
    *(undefined2 *)((int)puVar2 + 0x2e) = 0x100;
    *(undefined1 *)(puVar2 + 3) = 0;
    puVar1 = DAT_101567d4;
    puVar2[5] = uVar4;
    *puVar1 = puVar2;
    puVar2[1] = 2;
    uVar4 = DAT_101567d8;
    puVar2[2] = param_1;
    *puVar2 = uVar4;
  }
  return puVar2;
}


