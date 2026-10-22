// entry 0x101869e8  FUN_101869e8  (101869e8)

undefined4 * FUN_101869e8(int param_1,int param_2,uint *param_3,int *param_4)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  int *piVar6;
  uint uVar7;
  
  if (param_2 == 0) {
    FUN_1014c818(DAT_10186af0 | (DAT_10186ae8 - DAT_10186aec) * 0x20 & 0xff00U,DAT_10186af4);
    return (undefined4 *)0x0;
  }
  if (*(uint *)(param_2 + 0x10) != 0) {
    uVar3 = *param_3;
    if (uVar3 == 0) {
      piVar6 = (int *)(*(int *)(param_1 + 0x8c) + *(int *)(param_2 + 0x14));
      *param_3 = 1;
      *param_4 = piVar6[2];
    }
    else {
      if (*(uint *)(param_2 + 0x10) <= uVar3) {
        return (undefined4 *)0x0;
      }
      piVar6 = (int *)(*(int *)(param_1 + 0x8c) + *param_4);
      iVar4 = piVar6[2];
      *param_3 = uVar3 + 1;
      *param_4 = iVar4;
    }
    iVar4 = *piVar6;
    if (iVar4 == 3) {
      puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10186af8,3,param_4);
      if (puVar1 != (undefined4 *)0x0) {
        *puVar1 = piVar6;
        return puVar1;
      }
      uVar5 = 0xc96;
      uVar7 = (DAT_10186ae8 - DAT_10186aec) * 0x20 & 0xff00;
      uVar3 = DAT_10186b00;
      uVar2 = DAT_10186afc;
    }
    else if (iVar4 == 1) {
      puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10186af8,1,param_4);
      if (puVar1 != (undefined4 *)0x0) {
        iVar4 = piVar6[1];
        *puVar1 = piVar6;
        uVar2 = FUN_101866a0(piVar6[8],iVar4);
        puVar1[1] = uVar2;
        return puVar1;
      }
      uVar5 = 0xca5;
      uVar7 = (DAT_10186ae8 - DAT_10186aec) * 0x20 & 0xff00;
      uVar3 = DAT_10186b04;
      uVar2 = DAT_10186afc;
    }
    else {
      if (iVar4 != 4) {
        return (undefined4 *)0x0;
      }
      puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10186af8,4,param_4);
      if (puVar1 != (undefined4 *)0x0) {
        puVar1[1] = 0;
        puVar1[2] = 0;
        iVar4 = piVar6[1];
        *puVar1 = piVar6;
        uVar2 = FUN_101866a0(piVar6[9],iVar4);
        puVar1[2] = uVar2;
        return puVar1;
      }
      uVar5 = 0xcb5;
      uVar7 = (DAT_10186ae8 - DAT_10186aec) * 0x20 & 0xff00;
      uVar3 = DAT_10186b0c;
      uVar2 = DAT_10186b08;
    }
    FUN_1014c818(uVar3 | uVar7,uVar2,DAT_10186af8,uVar5);
  }
  return (undefined4 *)0x0;
}


