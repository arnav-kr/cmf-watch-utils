// entry 0x100de7a8  FUN_100de7a8

undefined4 FUN_100de7a8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte *pbVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 extraout_r1;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  undefined4 unaff_r4;
  undefined4 unaff_r5;
  undefined4 unaff_r6;
  undefined4 unaff_lr;
  undefined4 uVar8;
  undefined4 uVar9;
  
  iVar6 = DAT_100de7f4;
  FUN_101be218(DAT_100de7f4 + 0x84,0xffffffff,param_3,param_4,param_4);
  FUN_1014c818(DAT_100de800 | (DAT_100de7f8 - DAT_100de7fc) * 0x20 & 0xff00U,DAT_100de804,param_1,
               param_2);
  *(undefined4 *)(iVar6 + 0xac) = param_1;
  *(undefined4 *)(iVar6 + 0xbc) = param_2;
  *(undefined1 *)(iVar6 + 0x98) = 3;
  FUN_100dc648();
  uVar3 = DAT_100de808;
  iVar6 = DAT_100de658 - DAT_100de654;
  uVar8 = DAT_100de808;
  uVar9 = extraout_r1;
  uVar2 = FUN_1018bf5c(0x10);
  pbVar1 = DAT_100de65c;
  uVar7 = iVar6 * 0x20 & 0xff00;
  FUN_1014c818(DAT_100de660 | uVar7,DAT_100de664,uVar2,(*DAT_100de65c & 3) >> 1,uVar8,uVar9,unaff_r4
               ,unaff_r5,unaff_r6,unaff_lr);
  uVar5 = (uint)*pbVar1;
  uVar4 = (uVar5 & 3) >> 1;
  iVar6 = uVar5 << 0x1e;
  if (iVar6 < 0) {
    FUN_1014c818(DAT_100de668 | uVar7,DAT_100de66c,uVar4,iVar6,uVar8,uVar9,unaff_r4,unaff_r5,
                 unaff_r6,unaff_lr);
LAB_100de616:
    uVar3 = 0xfffffff6;
  }
  else {
    iVar6 = FUN_1018215c();
    if (iVar6 == 0) {
      iVar6 = FUN_101d72e4(0,0,0,DAT_100de670);
      if (iVar6 != 0) {
        FUN_1014c818(DAT_100de674 | uVar7,DAT_100de678,uVar3);
        goto LAB_100de616;
      }
    }
    else {
      FUN_100de10c(uVar4,uVar4);
    }
    uVar3 = *(undefined4 *)(pbVar1 + 0x9c);
  }
  return uVar3;
}


