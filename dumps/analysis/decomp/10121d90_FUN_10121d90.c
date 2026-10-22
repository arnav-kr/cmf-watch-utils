// 0x1023c5ab FUN_10121d90 :: dvfs_update_freq
// entry 0x10121d90  FUN_10121d90

/* WARNING: Removing unreachable block (ram,0x10121c52) */
/* WARNING: Removing unreachable block (ram,0x10121c58) */
/* WARNING: Removing unreachable block (ram,0x10121c54) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10121d90(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  int iVar2;
  code *pcVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  byte bVar12;
  uint uVar13;
  undefined8 uVar14;
  
  if (*DAT_10121db0 != '\0') {
    FUN_101ba194(DAT_10121db4);
    return 0;
  }
  uVar7 = (DAT_10121d50 - DAT_10121d4c) * 0x20 & 0xff00;
  FUN_1014c818(DAT_10121d54 | uVar7,DAT_10121d58,param_1,1,param_2);
  uVar14 = FUN_10121b70(param_1);
  iVar2 = DAT_10121d68;
  if ((int)uVar14 < 0) {
    FUN_1014c818(DAT_10121d5c | uVar7,DAT_10121d64,DAT_10121d60,param_1,param_2);
    uVar4 = 0xffffffea;
  }
  else {
    iVar8 = (int)uVar14 * 0xe + *(int *)(DAT_10121d68 + 0x20);
    sem(DAT_10121d68,(int)((ulonglong)uVar14 >> 0x20),0xffffffff,0xffffffff);
    cVar1 = *(char *)(iVar8 + 1);
    if (cVar1 == -1) {
      FUN_1014c818(DAT_10121d74 | uVar7,DAT_10121d70);
    }
    else {
      *(char *)(iVar8 + 1) = cVar1 + '\x01';
    }
    uVar11 = (uint)*(byte *)(iVar2 + 0x1a);
    uVar10 = (uint)*(byte *)(iVar2 + 0x18);
    iVar8 = *(int *)(iVar2 + 0x20) + 1;
    do {
      uVar11 = uVar11 - 1;
      if (uVar11 == 0xffffffff) {
        FUN_1014c818(uVar7 | 0xcd0031,DAT_10121d6c,*(undefined1 *)(iVar2 + 0x19),iVar8,param_2);
        uVar11 = (uint)*(byte *)(iVar2 + 0x19);
        goto LAB_10121c32;
      }
    } while (*(char *)(iVar8 + uVar11 * 0xe) == '\0');
    if (uVar10 == uVar11) {
      FUN_1014c818(uVar7 | 0xc70031,DAT_10121d80,uVar10,iVar8,param_2);
    }
    else {
LAB_10121c32:
      uVar4 = DAT_10121d8c;
      while (cVar1 = (char)uVar10, uVar10 != uVar11) {
        if ((int)uVar11 < (int)uVar10) {
          bVar12 = cVar1 - 1;
        }
        else {
          bVar12 = cVar1 + 1;
        }
        uVar13 = (uint)bVar12;
        iVar8 = *(int *)(iVar2 + 0x20);
        iVar9 = iVar8 + uVar13 * 0xe;
        uVar5 = FUN_100efe28();
        FUN_100ef5b4();
        FUN_1014c818(uVar7 | 0xe60032,uVar4,*(undefined1 *)(iVar8 + uVar10 * 0xe),
                     *(undefined1 *)(iVar8 + uVar13 * 0xe));
        (*DAT_10121d84)(1,uVar10,uVar13);
        pcVar3 = DAT_10121d84;
        uVar6 = (uint)*(ushort *)(iVar9 + 0xc);
        if (uVar5 < uVar6) {
          if ((int)(_DAT_40004000 << 0xe) < 0) {
            iVar8 = uVar6 + 0x32;
          }
          else {
            iVar8 = uVar6 + 100;
          }
          FUN_100efe70(iVar8);
          FUN_100efe40(*(undefined2 *)(iVar9 + 0xc));
        }
        FUN_101ba194(DAT_10121d88,*(undefined2 *)(iVar9 + 4),*(undefined2 *)(iVar9 + 2),
                     *(undefined2 *)(iVar9 + 6),*(undefined2 *)(iVar9 + 8),
                     *(undefined2 *)(iVar9 + 10),*(undefined2 *)(iVar9 + 0xc));
        FUN_100ef694(*(undefined2 *)(iVar9 + 4),*(undefined2 *)(iVar9 + 2));
        FUN_100ef5e4(*(undefined2 *)(iVar9 + 6),*(undefined2 *)(iVar9 + 8),
                     *(undefined2 *)(iVar9 + 10));
        if (*(ushort *)(iVar9 + 0xc) < uVar5) {
          FUN_100efe40();
          if ((_DAT_40004000 & 0x20000) == 0) {
            iVar8 = *(ushort *)(iVar9 + 0xc) + 100;
          }
          else {
            iVar8 = *(ushort *)(iVar9 + 0xc) + 0x32;
          }
          FUN_100efe70(iVar8);
        }
        (*pcVar3)(2,uVar10,uVar13);
        uVar10 = uVar13;
      }
      *(char *)(iVar2 + 0x18) = cVar1;
    }
    FUN_10199f88(DAT_10121d68);
    uVar4 = 0;
  }
  return uVar4;
}


