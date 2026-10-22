// entry 0x10187d90  FUN_10187d90  (10187d90)

undefined4 * FUN_10187d90(int param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  uint uVar7;
  
  if (param_2 == 0) {
    FUN_1014c818(DAT_10187f0c | (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00U,DAT_10187f10);
  }
  else {
    iVar5 = *(int *)(param_2 + 0x14);
    for (iVar3 = 0; piVar6 = (int *)(iVar5 + *(int *)(param_1 + 0x8c)),
        *(int *)(param_2 + 0x10) != iVar3; iVar3 = iVar3 + 1) {
      if (piVar6[1] == param_3) {
        iVar3 = *piVar6;
        if (iVar3 == 3) {
          puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10187f18,3,param_1,param_2,param_3);
          if (puVar1 != (undefined4 *)0x0) {
            *puVar1 = piVar6;
            return puVar1;
          }
          uVar4 = 0x9ef;
          uVar7 = (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00;
          uVar2 = DAT_10187f38;
        }
        else if (iVar3 == 4) {
          puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10187f18,4,param_1,param_2,param_3);
          if (puVar1 != (undefined4 *)0x0) {
            puVar1[1] = 0;
            *puVar1 = piVar6;
            puVar1[2] = 0;
            iVar3 = FUN_101878e4(param_1,puVar1,1);
            if (-1 < iVar3) {
              return puVar1;
            }
            FUN_1014c818(DAT_10187f28 | (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00U,DAT_10187f2c,
                         DAT_10187f18,0xa08);
LAB_10187e52:
            FUN_101d7876(puVar1);
            return (undefined4 *)0x0;
          }
          uVar4 = 0x9fd;
          uVar7 = (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00;
          uVar2 = DAT_10187f24;
        }
        else if (iVar3 == 2) {
          puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10187f18,2,param_1,param_2,param_3);
          if (puVar1 != (undefined4 *)0x0) {
            puVar1[1] = 0;
            puVar1[2] = 0;
            *puVar1 = piVar6;
            iVar3 = FUN_10186dd4(param_1,puVar1);
            if (-1 < iVar3) {
              return puVar1;
            }
            goto LAB_10187e52;
          }
          uVar4 = 0xa15;
          uVar7 = (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00;
          uVar2 = DAT_10187f30;
        }
        else {
          if (iVar3 != 1) {
            return (undefined4 *)0x0;
          }
          puVar1 = (undefined4 *)FUN_101d780c(0,0xc,DAT_10187f18,1,param_1,param_2,param_3);
          if (puVar1 != (undefined4 *)0x0) {
            *puVar1 = piVar6;
            puVar1[1] = 0;
            return puVar1;
          }
          uVar4 = 0xa2a;
          uVar7 = (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00;
          uVar2 = DAT_10187f34;
        }
        FUN_1014c818(uVar2 | uVar7,DAT_10187f20,DAT_10187f18,uVar4);
        return (undefined4 *)0x0;
      }
      iVar5 = piVar6[2];
    }
    FUN_1014c818(DAT_10187f14 | (DAT_10187f04 - DAT_10187f08) * 0x20 & 0xff00U,DAT_10187f1c,
                 DAT_10187f18,0x9e4,param_3);
  }
  return (undefined4 *)0x0;
}


