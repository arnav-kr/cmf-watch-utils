// 0x10244c00 FUN_101574a4 :: upgrade pending, skip erase parts
// entry 0x101574a4  FUN_101574a4

undefined4 FUN_101574a4(uint param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 uVar1;
  char cVar2;
  byte bVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  undefined4 uVar11;
  uint uVar12;
  int iVar13;
  uint uVar14;
  
  uVar12 = (DAT_101576a4 - DAT_101576a0) * 0x20 & 0xff00;
  uVar10 = param_1;
  FUN_1014c818(uVar12 | 0xfd0031,DAT_101576a8,*(undefined1 *)(param_1 + 0x2e7),DAT_101576a0,param_1,
               param_2,param_3);
  uVar4 = DAT_101576f4;
  cVar2 = *(char *)(param_1 + 0x2e7);
  uVar5 = DAT_101576b0;
  uVar11 = DAT_101576ac;
  if (cVar2 == '\x01') {
LAB_101574d4:
    uVar5 = uVar5 | uVar12;
LAB_101574ec:
    FUN_1014c818(uVar5,uVar11);
  }
  else {
    if (*(int *)(param_1 + 8) << 0x1f < 0) {
      if (cVar2 == '\x03') {
        uVar5 = uVar12 | 0x1080031;
        uVar11 = DAT_101576b4;
        goto LAB_101574ec;
      }
      uVar5 = DAT_101576bc;
      uVar11 = DAT_101576b8;
      if ((cVar2 == '\x04') || (cVar2 == '\x0f')) goto LAB_101574d4;
    }
    uVar5 = 0;
    iVar13 = param_1 + 0x2e4;
    uVar14 = uVar12 | DAT_101576f0;
    do {
      iVar6 = partition_1(uVar5 & 0xff);
      if (iVar6 == 0) {
        return 0xffffffea;
      }
      cVar2 = *(char *)(iVar6 + 9);
      if (cVar2 != '\0') {
        if (*(int *)(param_1 + 8) << 0x1f < 0) {
          if ((*(char *)(iVar6 + 8) == '\x05') || (cVar2 == -2)) {
            bVar3 = *(byte *)(iVar6 + 10);
            uVar9 = FUN_101d0972(*(undefined4 *)(param_1 + 0x18));
            if (bVar3 >> 4 == uVar9) goto LAB_101575cc;
            uVar1 = *(undefined1 *)(iVar6 + 9);
            uVar10 = (uint)(*(byte *)(iVar6 + 10) >> 4);
            uVar9 = DAT_101576d8 | uVar12;
            uVar11 = DAT_101576dc;
LAB_101575c6:
            FUN_1014c818(uVar9,uVar11,uVar5,uVar1,uVar10);
          }
          else {
            FUN_1014c818(uVar12 | 0x1260032,DAT_101576d4,uVar5,cVar2,uVar10,param_2,param_3);
          }
        }
        else {
          iVar7 = partition_5();
          if (iVar7 == 0) {
            FUN_1014c818(DAT_101576c0 | uVar12,DAT_101576c4,uVar5);
          }
          else {
LAB_101575cc:
            if ((*(char *)(param_1 + 0x2e7) == '\x04') || (*(char *)(param_1 + 0x2e7) == '\x02')) {
              uVar8 = FUN_1015a9e8(iVar13,*(undefined1 *)(iVar6 + 9));
              FUN_1014c818(uVar12 | 0x1380032,DAT_101576e0,*(undefined1 *)(param_1 + 0x2e7),uVar8);
              if ((uVar8 == 1) || (uVar8 - 5 < 3)) {
                uVar1 = *(undefined1 *)(iVar6 + 9);
                uVar9 = uVar12 | 0x13e0033;
                uVar11 = DAT_101576e4;
                uVar10 = uVar8;
                goto LAB_101575c6;
              }
              if ((uVar8 != 3) ||
                 ((-1 < *(int *)(param_1 + 8) << 0x1f &&
                  (bVar3 = *(byte *)(param_1 + 0x2e5), uVar9 = FUN_101bbdcc(), bVar3 != uVar9))))
              goto LAB_10157682;
              uVar9 = (*(int *)(param_1 + 0x302) + *(int *)(param_1 + 0x31b) & 0xfffff000U) -
                      *(int *)(iVar6 + 0xc);
              FUN_1014c818(uVar14,DAT_101576e8,uVar5,*(undefined1 *)(iVar6 + 9));
              uVar10 = uVar9;
              FUN_1014c818(uVar12 | uVar4,DAT_101576ec,*(undefined4 *)(param_1 + 0x302),
                           *(undefined4 *)(param_1 + 0x31b),uVar9);
              *(uint *)(param_1 + 0x31b) =
                   (*(int *)(iVar6 + 0xc) - *(int *)(param_1 + 0x302)) + uVar9;
              FUN_101572b8(param_1,iVar6,uVar9);
              uVar11 = 5;
            }
            else {
LAB_10157682:
              if ((*(uint *)(param_1 + 8) & 0x10) != 0) goto LAB_10157548;
              FUN_101572b8(param_1,iVar6);
              uVar11 = 1;
            }
            FUN_1015a97c(iVar13,*(undefined1 *)(iVar6 + 9),uVar11);
          }
        }
      }
LAB_10157548:
      uVar5 = uVar5 + 1;
    } while (uVar5 != 0x1e);
    cVar2 = *(char *)(param_1 + 0x2e7);
    if ((cVar2 != '\x04') && (cVar2 != '\x02')) {
      if (cVar2 != '\0') {
        FUN_1014c818(uVar12 | 0x1640031,DAT_101576c8);
        FUN_101d0a72(iVar13);
      }
      uVar11 = DAT_101576d0;
      uVar4 = DAT_101576cc;
      *(undefined1 *)(param_1 + 0x2e7) = 1;
      FUN_1014c818(uVar4 | uVar12,uVar11);
    }
    FUN_1015a8d4(iVar13);
  }
  return 0;
}


