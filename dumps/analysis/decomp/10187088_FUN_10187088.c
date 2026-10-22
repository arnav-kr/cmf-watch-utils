// entry 0x10187088  FUN_10187088  (10187088)

int FUN_10187088(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int *piVar9;
  int *piVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  undefined1 *local_48;
  uint local_44;
  short local_38;
  char local_36;
  ushort local_34;
  ushort local_2e;
  
  iVar1 = thunk_FUN_10146a3c(0xa8,DAT_101873cc);
  uVar11 = (DAT_101873d0 - DAT_101873d4) * 0x20 & 0xff00;
  if (iVar1 == 0) {
    FUN_1014c818(DAT_101873d8 | uVar11,DAT_101873dc);
    return 0;
  }
  FUN_101bf3b2(iVar1,0,0xa8);
  if (param_1 != 0) {
    FUN_1014c818(DAT_101873e0 | uVar11,DAT_101873e4,param_1);
    iVar2 = FUN_101d783c();
    if (iVar2 == 0) {
      iVar3 = FUN_10107030(iVar1,param_1,1);
      iVar2 = 0;
      if (iVar3 < 0) {
        FUN_1014c818(DAT_10187430 | uVar11,DAT_101873fc,param_1);
        goto LAB_10187100;
      }
LAB_101871f2:
      iVar3 = FUN_101bf36a(param_1);
      uVar5 = thunk_FUN_10146a3c(iVar3 + 1,DAT_101873cc);
      *(undefined4 *)(iVar1 + 0x94) = uVar5;
      iVar3 = FUN_101bf36a(param_1);
      FUN_101bf3b2(uVar5,0,iVar3 + 1);
      FUN_101bf34a(*(undefined4 *)(iVar1 + 0x94),param_1);
      goto LAB_10187220;
    }
    iVar2 = FUN_101bf36a(param_1);
    iVar2 = thunk_FUN_10146a3c(iVar2 + 1,DAT_101873cc);
    if (iVar2 == 0) {
      FUN_1014c818(DAT_101873e8 | uVar11,DAT_101873ec);
      goto LAB_10187100;
    }
    iVar3 = FUN_101bf36a(param_1);
    FUN_101bf3b2(iVar2,0,iVar3 + 1);
    FUN_101bf34a(iVar2,param_1);
    iVar3 = FUN_101bf362(iVar2,0x3a);
    if (iVar3 != -1) {
      uVar12 = uVar11 | DAT_10187454;
      FUN_1014c818(DAT_101873f0 | uVar11,DAT_101873f4,1);
      FUN_1014c818(uVar12,DAT_101873f8,1,*(undefined1 *)(iVar3 + 1));
      *(undefined1 *)(iVar3 + 1) = 0x42;
      iVar4 = FUN_10107030(iVar1,iVar2,1);
      if (-1 < iVar4) {
        uVar5 = 1;
LAB_101871e6:
        FUN_1014c818(DAT_10187404 | uVar11,DAT_10187408,uVar5,iVar2);
        goto LAB_101871f2;
      }
      FUN_1014c818(uVar12,DAT_101873f8,0,*(undefined1 *)(iVar3 + 1));
      *(undefined1 *)(iVar3 + 1) = 0x41;
      iVar3 = FUN_10107030(iVar1,iVar2,1);
      if (-1 < iVar3) {
        uVar5 = 0;
        goto LAB_101871e6;
      }
      iVar3 = FUN_10107030(iVar1,param_1,1);
      uVar12 = DAT_10187400;
      uVar5 = DAT_101873fc;
      if (-1 < iVar3) goto LAB_101871f2;
LAB_101871d6:
      FUN_1014c818(uVar12 | uVar11,uVar5,param_1);
    }
    goto LAB_101871dc;
  }
  FUN_1014c818(DAT_10187434 | uVar11,DAT_10187438);
  iVar2 = 0;
LAB_10187220:
  if (param_2 == 0) {
    uVar12 = uVar11 | 0x6a80031;
    uVar5 = DAT_101877c8;
LAB_10187344:
    FUN_1014c818(uVar12,uVar5);
  }
  else {
    FUN_1014c818(DAT_1018740c | uVar11,DAT_10187410,param_2);
    iVar3 = FUN_101d7820(0,0x78,DAT_10187414);
    *(int *)(iVar1 + 0xa0) = iVar3;
    if (iVar3 == 0) {
      FUN_1014c818(DAT_10187418 | uVar11,DAT_1018741c);
LAB_10187250:
      FUN_10186b34();
    }
    else {
      FUN_101bf3b2(iVar3,0,0x78);
      iVar3 = FUN_101bf36a(param_2);
      iVar3 = thunk_FUN_10146a3c(iVar3 + 1,DAT_10187414);
      if (iVar3 == 0) {
        uVar12 = DAT_1018743c | uVar11;
        uVar5 = DAT_10187440;
        goto LAB_10187344;
      }
      iVar4 = FUN_101bf36a(param_2);
      FUN_101bf3b2(iVar3,0,iVar4 + 1);
      FUN_101bf34a(iVar3,param_2);
      iVar4 = FUN_101d783c();
      if ((iVar4 == 0) || (iVar4 = FUN_101bf362(iVar3,0x3a), 0x42 < *(byte *)(iVar4 + 1))) {
        local_48 = (undefined1 *)0x0;
        local_44 = 0;
      }
      else {
        local_48 = (undefined1 *)(iVar4 + 1);
        local_44 = 1;
      }
      iVar4 = 0;
      uVar12 = DAT_10187444 | uVar11;
      do {
        if (local_48 != (undefined1 *)0x0) {
          *local_48 = *(undefined1 *)(iVar4 + DAT_10187448);
        }
        if (iVar4 == 0) {
          iVar13 = iVar1 + 0x2c;
          iVar6 = FUN_10107030(iVar13,iVar3,1);
          if (iVar6 < 0) {
            FUN_1014c818(DAT_1018744c | uVar11,DAT_10187450,iVar3);
          }
          else {
            FUN_101072f0(iVar13,0,0);
            uVar7 = FUN_10107238(iVar13,&local_38);
            if (uVar7 < 0x10) {
              uVar12 = uVar11 | 0x4b80031;
              uVar5 = DAT_10187788;
            }
            else {
              if ((local_38 == 0x4552) && (local_36 == 'S')) {
                piVar10 = (int *)FUN_101d7820(0,(uint)local_34 << 4,DAT_10187794);
                if (piVar10 == (int *)0x0) {
                  FUN_1014c818(DAT_10187798 | uVar11,DAT_1018779c);
                }
                else {
                  uVar7 = FUN_10107238(iVar13,piVar10,(uint)local_34 << 4);
                  uVar8 = (uint)local_34;
                  if (uVar7 < uVar8 << 4) {
                    FUN_1014c818(DAT_101877a0 | uVar11,DAT_101877a4);
                    FUN_101d7826(0,piVar10);
                    goto LAB_10187482;
                  }
                  iVar6 = *(int *)(iVar1 + 0xa0);
                  *(undefined4 *)(iVar6 + 0x2c) = 1;
                  *(uint *)(iVar6 + 0x30) = uVar8;
                  uVar5 = FUN_101d7820(0,uVar8 << 2,DAT_10187794);
                  *(undefined4 *)(iVar6 + 0x34) = uVar5;
                  iVar6 = *(int *)(iVar1 + 0xa0);
                  if (*(int *)(iVar6 + 0x34) == 0) {
                    FUN_1014c818(DAT_101877a8 | uVar11,DAT_101877ac);
                    FUN_101d7826(0,piVar10);
                  }
                  else {
                    uVar5 = FUN_101d7820(0,(uint)local_34 << 2,DAT_10187794);
                    iVar13 = *(int *)(iVar1 + 0xa0);
                    *(undefined4 *)(iVar6 + 0x38) = uVar5;
                    if (*(int *)(iVar13 + 0x38) != 0) {
                      FUN_1014c818(uVar12,DAT_101877b4,*(undefined4 *)(iVar13 + 0x2c),
                                   *(undefined4 *)(iVar13 + 0x30));
                      piVar9 = piVar10;
                      for (iVar6 = 0; iVar6 < (int)(uint)local_34; iVar6 = iVar6 + 1) {
                        iVar13 = *(int *)(*(int *)(iVar1 + 0xa0) + 0x38);
                        *(int *)(*(int *)(*(int *)(iVar1 + 0xa0) + 0x34) + iVar6 * 4) = *piVar9 + 4;
                        *(uint *)(iVar13 + iVar6 * 4) =
                             CONCAT12(*(undefined1 *)((int)piVar9 + 0xf),(short)piVar9[1]) - 4;
                        piVar9 = piVar9 + 4;
                      }
                      goto LAB_10187590;
                    }
                    FUN_1014c818(DAT_101877b0 | uVar11,DAT_101877ac);
                    FUN_101d7826(0,piVar10);
                    FUN_101d7826(0,*(undefined4 *)(*(int *)(iVar1 + 0xa0) + 0x34));
                    iVar13 = *(int *)(iVar1 + 0xa0);
                    *(undefined4 *)(iVar13 + 0x34) = 0;
                  }
                }
                goto LAB_101874c2;
              }
              uVar12 = DAT_1018778c | uVar11;
              uVar5 = DAT_10187790;
            }
            FUN_1014c818(uVar12,uVar5);
LAB_10187482:
            FUN_101071e0(iVar13);
          }
          break;
        }
        iVar6 = FUN_10107030(*(int *)(iVar1 + 0xa0) + 0x3c,iVar3,1);
        if (-1 < iVar6) {
          FUN_101072f0(*(int *)(iVar1 + 0xa0) + 0x3c,0);
          uVar7 = FUN_10107238(*(int *)(iVar1 + 0xa0) + 0x3c,&local_38);
          if (0xf < uVar7) {
            piVar10 = (int *)FUN_101d7820(0,(uint)local_34 << 4,DAT_10187794);
            if (piVar10 != (int *)0x0) {
              uVar7 = FUN_10107238(*(int *)(iVar1 + 0xa0) + 0x3c,piVar10,(uint)local_34 << 4);
              uVar8 = (uint)local_34;
              if (uVar8 << 4 <= uVar7) {
                iVar6 = *(int *)(iVar1 + 0xa0);
                *(uint *)(iVar6 + 0x68) = (uint)local_2e;
                *(uint *)(iVar6 + 0x6c) = local_2e + uVar8 + -1;
                uVar5 = FUN_101d7820(0,uVar8 << 2,DAT_10187794);
                *(undefined4 *)(iVar6 + 0x70) = uVar5;
                iVar6 = *(int *)(iVar1 + 0xa0);
                if (*(int *)(iVar6 + 0x70) == 0) {
                  FUN_1014c818(DAT_101877bc | uVar11,DAT_101877ac);
                  FUN_101d7826(0,piVar10);
                  goto LAB_1018763e;
                }
                uVar5 = FUN_101d7820(0,(uint)local_34 << 2,DAT_10187794);
                iVar13 = *(int *)(iVar1 + 0xa0);
                *(undefined4 *)(iVar6 + 0x74) = uVar5;
                if (*(int *)(iVar13 + 0x74) != 0) {
                  FUN_1014c818(uVar11 | 0x5380032,DAT_101877b4,*(undefined4 *)(iVar13 + 0x68),
                               *(undefined4 *)(iVar13 + 0x6c));
                  piVar9 = piVar10;
                  for (iVar6 = 0; iVar6 < (int)(uint)local_34; iVar6 = iVar6 + 1) {
                    iVar13 = *(int *)(*(int *)(iVar1 + 0xa0) + 0x74);
                    *(int *)(*(int *)(*(int *)(iVar1 + 0xa0) + 0x70) + iVar6 * 4) = *piVar9 + 4;
                    *(uint *)(iVar13 + iVar6 * 4) =
                         CONCAT12(*(undefined1 *)((int)piVar9 + 0xf),(short)piVar9[1]) - 4;
                    piVar9 = piVar9 + 4;
                  }
LAB_10187590:
                  FUN_101d7826(0,piVar10);
                  *(int *)(iVar1 + 0xa4) = *(int *)(iVar1 + 0xa4) + 1;
                  goto LAB_101875a2;
                }
                FUN_1014c818(DAT_101877c0 | uVar11,DAT_101877c4);
                FUN_101d7826(0,piVar10);
                FUN_101d7826(0,*(undefined4 *)(*(int *)(iVar1 + 0xa0) + 0x70));
                iVar13 = *(int *)(iVar1 + 0xa0);
                *(undefined4 *)(iVar13 + 0x70) = 0;
                goto LAB_10187642;
              }
              FUN_1014c818(uVar11 | 0x5180031,DAT_101877a4);
              FUN_101d7826(0,piVar10);
              iVar13 = *(int *)(iVar1 + 0xa0) + 0x3c;
              goto LAB_10187482;
            }
            FUN_1014c818(DAT_101877b8 | uVar11,DAT_1018779c);
LAB_1018763e:
            iVar13 = *(int *)(iVar1 + 0xa0);
LAB_10187642:
            iVar13 = iVar13 + 0x3c;
LAB_101874c2:
            FUN_101071e0(iVar13);
            FUN_101ce04a(iVar3);
            goto LAB_10187250;
          }
          FUN_1014c818(uVar11 | 0x5080031,DAT_10187788);
          break;
        }
LAB_101875a2:
        uVar7 = iVar4 + 1;
        iVar4 = 1;
      } while (uVar7 <= local_44);
      FUN_101ce04a(iVar3);
    }
  }
  if (param_3 != 0) {
    FUN_1014c818(DAT_10187420 | uVar11,DAT_10187424,param_3);
    iVar3 = FUN_101d783c();
    iVar4 = iVar1 + 0x58;
    if (iVar3 == 0) {
      iVar3 = FUN_10107030(iVar4,param_3,1);
      if (-1 < iVar3) goto LAB_101877d4;
      FUN_1014c818(DAT_101877cc | uVar11,DAT_101877d0,param_3);
      if (iVar2 == 0) goto LAB_10187100;
    }
    else {
      if (iVar2 != 0) {
        FUN_101ce04a(iVar2);
      }
      iVar2 = FUN_101bf36a(param_3);
      iVar2 = thunk_FUN_10146a3c(iVar2 + 1,DAT_101873cc);
      iVar3 = FUN_101bf36a(param_3);
      FUN_101bf3b2(iVar2,0,iVar3 + 1);
      FUN_101bf34a(iVar2,param_3);
      iVar3 = FUN_101bf362(iVar2,0x3a);
      if (iVar3 != -1) {
        *(undefined1 *)(iVar3 + 1) = 0x42;
        iVar6 = FUN_10107030(iVar4,iVar2,1);
        if (iVar6 < 0) {
          *(undefined1 *)(iVar3 + 1) = 0x41;
          iVar3 = FUN_10107030(iVar4,iVar2,1);
          if ((iVar3 < 0) &&
             (iVar3 = FUN_10107030(iVar4,param_3,1), uVar12 = DAT_1018742c, uVar5 = DAT_10187428,
             param_1 = param_3, iVar3 < 0)) goto LAB_101871d6;
        }
LAB_101877d4:
        iVar3 = FUN_101bf36a(param_3);
        uVar5 = thunk_FUN_10146a3c(iVar3 + 1,DAT_101878bc);
        *(undefined4 *)(iVar1 + 0x98) = uVar5;
        iVar3 = FUN_101bf36a(param_3);
        FUN_101bf3b2(uVar5,0,iVar3 + 1);
        FUN_101bf34a(*(undefined4 *)(iVar1 + 0x98),param_3);
        goto LAB_10187802;
      }
    }
LAB_101871dc:
    FUN_101ce04a(iVar2);
LAB_10187100:
    FUN_101071e0(iVar1);
    FUN_101071e0(iVar1 + 0x2c);
    FUN_101071e0(iVar1 + 0x58);
    if (*(int *)(iVar1 + 0x94) != 0) {
      FUN_101ce04a();
      *(undefined4 *)(iVar1 + 0x94) = 0;
    }
    if (*(int *)(iVar1 + 0x98) != 0) {
      FUN_101ce04a();
      *(undefined4 *)(iVar1 + 0x98) = 0;
    }
    FUN_101ce04a(iVar1);
    return 0;
  }
  FUN_1014c818(DAT_101878cc | uVar11,DAT_101878d0);
LAB_10187802:
  FUN_101072f0(iVar1,0,2);
  uVar12 = FUN_10107350(iVar1);
  FUN_101072f0(iVar1,0);
  iVar3 = FUN_101d7820(2,uVar12,DAT_101878c0);
  *(int *)(iVar1 + 0x90) = iVar3;
  if (iVar3 == 0) {
    FUN_1014c818(DAT_101878c4 | uVar11,DAT_101878c8);
    FUN_10186b34();
  }
  else {
    uVar7 = FUN_10107238(iVar1,iVar3,uVar12);
    if (uVar7 < uVar12) {
      FUN_1014c818(DAT_101878d4 | uVar11,DAT_101878d8,uVar7);
      FUN_101d7826(2,*(undefined4 *)(iVar1 + 0x90));
    }
    else {
      piVar10 = *(int **)(iVar1 + 0x90);
      iVar3 = *piVar10;
      if (iVar3 != -1) {
        *(int **)(iVar1 + 0x88) = piVar10 + 2;
        *(int *)(iVar1 + 0x84) = iVar3;
        *(int **)(iVar1 + 0x8c) = piVar10 + iVar3 * 9 + 2;
        goto LAB_1018783e;
      }
      FUN_1014c818(DAT_101878dc | uVar11,DAT_101878e0);
      FUN_101a9c34(*(undefined4 *)(iVar1 + 0x90));
    }
    *(undefined4 *)(iVar1 + 0x90) = 0;
  }
LAB_1018783e:
  if (iVar2 == 0) {
    return iVar1;
  }
  FUN_101ce04a(iVar2);
  return iVar1;
}


