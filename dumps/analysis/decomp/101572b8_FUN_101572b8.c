// 0x10244acf FUN_101572b8 :: storage not init, update failed\n
// entry 0x101572b8  FUN_101572b8

/* WARNING: Removing unreachable block (ram,0x10156c82) */
/* WARNING: Removing unreachable block (ram,0x10156c8e) */

undefined4 FUN_101572b8(int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  code *pcVar4;
  int iVar5;
  undefined4 unaff_r4;
  uint uVar6;
  int iVar7;
  undefined4 unaff_r5;
  uint uVar8;
  undefined4 unaff_r6;
  uint uVar9;
  undefined4 unaff_r7;
  undefined4 unaff_r8;
  undefined4 unaff_lr;
  undefined8 uVar10;
  
  iVar3 = param_3;
  piVar2 = (int *)FUN_10156e44(*(byte *)(param_2 + 10) >> 4);
  uVar6 = (DAT_10157374 - DAT_10157378) * 0x20 & 0xff00;
  if (piVar2 == (int *)0x0) {
    FUN_1014c818(uVar6 | 0xdc0031,DAT_1015737c);
    return 0xffffffea;
  }
  FUN_1014c818(uVar6 | 0xe10034,DAT_10157380,param_2,*(undefined4 *)(param_2 + 0xc),
               *(undefined4 *)(param_2 + 0x10),param_3,iVar3);
  uVar8 = (*(int *)(param_2 + 0x10) - param_3) + 0xfffU & DAT_10157384;
  uVar9 = *(int *)(param_2 + 0xc) + param_3 & DAT_10157384;
  FUN_1014c818(uVar6 | 0xe70032,DAT_10157388,uVar9,uVar8);
  if (((*(int *)(param_1 + 0x24) != 0) && (*(int *)(param_1 + 0x20) != 0)) &&
     (iVar3 = FUN_10156b88(piVar2,uVar9,uVar8,*(int *)(param_1 + 0x24),*(int *)(param_1 + 0x20)),
     iVar3 == 1)) {
    FUN_1014c818(uVar6 | 0xec0031,DAT_1015738c);
    return 0;
  }
  FUN_1014c818(DAT_10156e3c | (DAT_10156e38 - DAT_10156e34) * 0x20 & 0xff00U,DAT_10156e40,uVar9,
               uVar8,unaff_r4,unaff_r5,unaff_r6,unaff_r7,unaff_r8,unaff_lr);
  if (piVar2 == (int *)0x0) {
    return 0xffffffea;
  }
  iVar3 = piVar2[5];
  if ((1 < iVar3 - 1U) && (iVar3 != 4)) {
    if (iVar3 != 0) {
                    /* WARNING: Could not recover jumptable at 0x10156e28. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar1 = (**(code **)(*(int *)(*piVar2 + 8) + 8))(*piVar2,uVar9,uVar8);
      return uVar1;
    }
    uVar1 = 0;
    if (piVar2 == (int *)0x0) {
      uVar1 = 0xffffffea;
    }
    else {
      for (; 0 < (int)uVar8; uVar8 = uVar8 - uVar6) {
        iVar3 = piVar2[3];
        uVar6 = uVar8;
        if (iVar3 <= (int)uVar8) {
          uVar6 = iVar3 - (iVar3 - 1U & uVar9);
        }
        pcVar4 = *(code **)(*(int *)(*piVar2 + 8) + 8);
        uVar10 = (*pcVar4)(*piVar2,uVar9,uVar6,pcVar4,uVar1,unaff_r4,unaff_r5,unaff_r6,unaff_r7,
                           unaff_lr);
        if ((int)uVar10 != 0) {
          FUN_1014c818(DAT_10156dc0 | (DAT_10156dbc - DAT_10156db8) * 0x20 & 0xff00U,DAT_10156dc4,
                       (int)uVar10,uVar9);
          return 0xfffffffb;
        }
        uVar9 = uVar9 + uVar6;
        FUN_10105210(0,(int)((ulonglong)uVar10 >> 0x20),0);
      }
      uVar1 = 0;
    }
    return uVar1;
  }
  if (piVar2 != (int *)0x0) {
    if ((uVar9 & 0x1ff) == 0) {
      uVar6 = uVar9;
      if ((int)uVar9 < 0) {
        uVar6 = uVar9 + 0x1ff;
      }
      iVar5 = (int)uVar6 >> 9;
      iVar3 = thunk_FUN_10146a3c(0x200,DAT_10156d4c,0,0,piVar2,uVar9,unaff_r4,unaff_r5,unaff_r6,
                                 unaff_r7,unaff_r8,unaff_lr);
      if (iVar3 == 0) {
        FUN_1014c818((DAT_10156d40 - DAT_10156d3c) * 0x20 & 0xff00U | 0x17e0011,DAT_10156d50,0x200);
        return 0xfffffff4;
      }
      FUN_101bf3b2(iVar3,0xff,0x200);
      iVar7 = 1;
      while( true ) {
        if (iVar7 == 0) {
          FUN_101ce04a(iVar3);
          return 0;
        }
        iVar5 = iVar5 + 1;
        uVar10 = (**(code **)(*(int *)(*piVar2 + 8) + 4))
                           (*piVar2,iVar5 * 0x200 + -0x200,iVar3,0x200);
        if ((int)uVar10 != 0) break;
        FUN_10105210(0,(int)((ulonglong)uVar10 >> 0x20),0);
        iVar7 = iVar7 + -1;
      }
      FUN_1014c818((DAT_10156d40 - DAT_10156d3c) * 0x20 & 0xff00U | 0x1860013,DAT_10156d54,
                   (int)uVar10,iVar5,iVar7 + -1);
      FUN_101ce04a(iVar3);
      return 0xfffffffb;
    }
    FUN_1014c818(DAT_10156d44 | (DAT_10156d40 - DAT_10156d3c) * 0x20 & 0xff00U,DAT_10156d48,uVar9);
  }
  return 0xffffffea;
}


