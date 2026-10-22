// entry 0x101866f8  FUN_101866f8  (101866f8)

undefined4 * FUN_101866f8(int param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int *piVar8;
  
  if (param_2 == 0) {
    uVar4 = 0xb53;
    uVar6 = (DAT_10186810 - DAT_1018680c) * 0x20 & 0xff00;
    uVar1 = DAT_1018681c;
    uVar2 = DAT_10186818;
LAB_10186714:
    FUN_1014c818(uVar1 | uVar6,uVar2,DAT_10186814,uVar4);
  }
  else {
    iVar7 = *(int *)(param_2 + 0x14);
    for (iVar5 = 0; piVar8 = (int *)(iVar7 + *(int *)(param_1 + 0x8c)),
        *(int *)(param_2 + 0x10) != iVar5; iVar5 = iVar5 + 1) {
      if (piVar8[1] == param_3) {
        iVar5 = *piVar8;
        if (iVar5 == 3) {
          puVar3 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10186814,3,param_1,param_2);
          if (puVar3 != (undefined4 *)0x0) {
            *puVar3 = piVar8;
            return puVar3;
          }
          uVar4 = 0xb74;
          uVar6 = (DAT_10186810 - DAT_1018680c) * 0x20 & 0xff00;
          uVar1 = DAT_10186834;
          uVar2 = DAT_10186828;
        }
        else if (iVar5 == 1) {
          puVar3 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10186814,1,param_1,param_2);
          if (puVar3 != (undefined4 *)0x0) {
            iVar5 = piVar8[1];
            *puVar3 = piVar8;
            uVar2 = FUN_101866a0(piVar8[8],iVar5);
            puVar3[1] = uVar2;
            return puVar3;
          }
          uVar4 = 0xb82;
          uVar6 = (DAT_10186810 - DAT_1018680c) * 0x20 & 0xff00;
          uVar1 = DAT_1018682c;
          uVar2 = DAT_10186828;
        }
        else {
          if (iVar5 != 4) {
            return (undefined4 *)0x0;
          }
          puVar3 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10186814,4,param_1,param_2);
          if (puVar3 != (undefined4 *)0x0) {
            puVar3[1] = 0;
            puVar3[2] = 0;
            iVar5 = piVar8[1];
            *puVar3 = piVar8;
            uVar2 = FUN_101866a0(piVar8[9],iVar5);
            puVar3[2] = uVar2;
            return puVar3;
          }
          uVar4 = 0xb92;
          uVar6 = (DAT_10186810 - DAT_1018680c) * 0x20 & 0xff00;
          uVar1 = DAT_10186830;
          uVar2 = DAT_10186828;
        }
        goto LAB_10186714;
      }
      iVar7 = piVar8[2];
    }
    FUN_1014c818(DAT_10186820 | (DAT_10186810 - DAT_1018680c) * 0x20 & 0xff00U,DAT_10186824,
                 DAT_10186814,0xb69,param_3,*(undefined4 *)(param_2 + 0x18));
  }
  return (undefined4 *)0x0;
}


