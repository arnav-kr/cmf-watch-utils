// entry 0x10184674  FUN_10184674

int FUN_10184674(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  uint *puVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  
  piVar2 = DAT_101847dc;
  puVar1 = DAT_101847d8;
  uVar7 = 0xffffffff;
  uVar9 = 0xffffffff;
  uVar5 = 0;
  puVar4 = DAT_101847d8;
  do {
    if (*puVar4 <= uVar5) {
      if ((uVar9 & uVar7) == 0xffffffff) {
        uVar7 = (DAT_101847e4 - DAT_101847e0) * 0x20 & 0xff00;
        FUN_1014c818(DAT_101847e8 | uVar7,DAT_101847ec);
        piVar2 = DAT_101847dc;
        uVar7 = DAT_101847f0 | uVar7;
        for (uVar9 = 0; uVar9 < *puVar1; uVar9 = uVar9 + 1) {
          FUN_1014c818(uVar7,DAT_101847f8,uVar9,
                       *(undefined4 *)(*(int *)(*piVar2 + uVar9 * 4) + 0x94));
        }
        iVar6 = 0;
      }
      else {
        iVar6 = FUN_10187088(param_1,param_2,param_3);
        if (iVar6 == 0) {
          FUN_1014c818(DAT_101847fc | (DAT_101847e4 - DAT_101847e0) * 0x20 & 0xff00U,DAT_10184800);
        }
        else {
          if (param_4 != 0) {
            *(undefined4 *)(iVar6 + 0x9c) = 1;
          }
          piVar2 = DAT_101847dc;
          if (uVar9 == 0xffffffff) {
            FUN_101845e8(*(undefined4 *)(*DAT_101847dc + uVar7 * 4),1);
            *(int *)(*piVar2 + uVar7 * 4) = iVar6;
          }
          else {
            *(int *)(*DAT_101847dc + uVar9 * 4) = iVar6;
          }
        }
      }
      return iVar6;
    }
    iVar8 = uVar5 * 4;
    iVar6 = *(int *)(*piVar2 + uVar5 * 4);
    if (iVar6 == 0) {
      if (uVar9 == 0xffffffff) {
        uVar9 = uVar5;
      }
    }
    else {
      iVar6 = FUN_101bf37a(param_1,*(undefined4 *)(iVar6 + 0x94));
      puVar4 = DAT_101847d8;
      iVar3 = *(int *)(*piVar2 + iVar8);
      if (iVar6 == 0) {
        if (param_4 != 0) {
          *(int *)(iVar3 + 0x9c) = *(int *)(iVar3 + 0x9c) + 1;
        }
        if (*(int *)(iVar3 + 0x98) != 0) {
          if ((param_5 == 0) && (iVar6 = FUN_101bf37a(param_3), iVar6 != 0)) {
            FUN_1014c818((DAT_101847e4 - DAT_101847e0) * 0x20 & 0xff00U | 0x29c0032,DAT_101847f4,
                         *(undefined4 *)(*(int *)(*piVar2 + iVar8) + 0x98),param_3);
            FUN_101860e8(*(undefined4 *)(*piVar2 + iVar8),param_3);
          }
          return *(int *)(*piVar2 + iVar8);
        }
      }
      else if (*(int *)(iVar3 + 0x9c) == 0) {
        uVar7 = uVar5;
      }
    }
    uVar5 = uVar5 + 1;
  } while( true );
}


