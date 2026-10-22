// entry 0x101868d8  FUN_101868bc  (101868bc)

undefined4 * FUN_101868bc(int param_1,int *param_2,uint *param_3,int *param_4)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  int *piVar6;
  uint uVar7;
  
  uVar7 = *param_3;
  if (param_2 == (int *)0x0) {
    FUN_1014c818(DAT_101869c8 | (DAT_101869c0 - DAT_101869c4) * 0x20 & 0xff00U,DAT_101869cc);
    return (undefined4 *)0x0;
  }
  uVar3 = *(uint *)(*param_2 + 0x1c);
  if (uVar3 != 0) {
    if (uVar7 == 0) {
      piVar6 = (int *)(*(int *)(param_1 + 0x8c) + *(int *)(*param_2 + 0x20));
      *param_3 = 1;
      *param_4 = piVar6[2];
    }
    else {
      if (uVar3 <= uVar7) {
        return (undefined4 *)0x0;
      }
      piVar6 = (int *)(*(int *)(param_1 + 0x8c) + *param_4);
      iVar4 = piVar6[2];
      *param_3 = uVar7 + 1;
      *param_4 = iVar4;
    }
    iVar4 = *piVar6;
    if (iVar4 == 3) {
      puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_101869d0);
      if (puVar1 != (undefined4 *)0x0) {
        *puVar1 = piVar6;
        return puVar1;
      }
      uVar5 = 0xc37;
      uVar3 = (DAT_101869c0 - DAT_101869c4) * 0x20 & 0xff00;
      uVar7 = DAT_101869d8;
      uVar2 = DAT_101869d4;
    }
    else if (iVar4 == 1) {
      puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_101869d0);
      if (puVar1 != (undefined4 *)0x0) {
        iVar4 = piVar6[1];
        *puVar1 = piVar6;
        uVar2 = FUN_101866a0(piVar6[8],iVar4);
        puVar1[1] = uVar2;
        return puVar1;
      }
      uVar5 = 0xc46;
      uVar3 = (DAT_101869c0 - DAT_101869c4) * 0x20 & 0xff00;
      uVar7 = DAT_101869dc;
      uVar2 = DAT_101869d4;
    }
    else {
      if (iVar4 != 4) {
        return (undefined4 *)0x0;
      }
      puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_101869d0);
      if (puVar1 != (undefined4 *)0x0) {
        puVar1[1] = 0;
        puVar1[2] = 0;
        iVar4 = piVar6[1];
        *puVar1 = piVar6;
        uVar2 = FUN_101866a0(piVar6[9],iVar4);
        puVar1[2] = uVar2;
        return puVar1;
      }
      uVar5 = 0xc56;
      uVar3 = (DAT_101869c0 - DAT_101869c4) * 0x20 & 0xff00;
      uVar7 = DAT_101869e4;
      uVar2 = DAT_101869e0;
    }
    FUN_1014c818(uVar7 | uVar3,uVar2,DAT_101869d0,uVar5);
  }
  return (undefined4 *)0x0;
}


