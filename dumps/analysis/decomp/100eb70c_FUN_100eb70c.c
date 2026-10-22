// entry 0x100eb70c  FUN_100eb70c  (100eb70c)

int FUN_100eb70c(int param_1)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  char *pcVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined1 uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  undefined4 uStack_148;
  undefined1 auStack_144 [60];
  undefined4 uStack_108;
  undefined1 auStack_104 [60];
  undefined4 uStack_c8;
  undefined1 uStack_c4;
  undefined1 uStack_c3;
  undefined2 uStack_c2;
  undefined2 uStack_c0;
  undefined2 uStack_be;
  undefined2 uStack_bc;
  
  uStack_148 = 0;
  FUN_101bf3b2(auStack_144,0,0x3c);
  uStack_108 = 0;
  FUN_101bf3b2(auStack_104,0,0x3c);
  iVar10 = *DAT_100eb910;
  uVar9 = (DAT_100eb914 - DAT_100eb918) * 0x20 & 0xff00;
  iVar11 = iVar10 + 0x1c;
  FUN_1014c818(DAT_100eb91c | uVar9,DAT_100eb924,DAT_100eb920,param_1);
  FUN_101071e0(iVar11);
  pbVar3 = DAT_100eb928;
  *(undefined4 *)(iVar10 + 0x1c) = 0;
  iVar8 = DAT_100eb96c;
  if (param_1 == 1) {
    bVar1 = *pbVar3;
    if (bVar1 == 1) {
      bVar1 = pbVar3[9];
      uVar6 = DAT_100eb950;
LAB_100eb800:
      FUN_101aa780(&uStack_148,0x40,uVar6,DAT_100eb954,bVar1);
    }
    else {
      if (bVar1 == 2) {
        bVar1 = pbVar3[9];
        uVar6 = DAT_100eb968;
        goto LAB_100eb800;
      }
      if (bVar1 == 0xa5) {
        FUN_101aa780(&uStack_148,0x40,DAT_100eb970,DAT_100eb954,*(undefined4 *)(pbVar3 + 5));
        FUN_1014c818(DAT_100eb974 | uVar9,DAT_100eb978,DAT_100eb920,1,*(undefined4 *)(iVar8 + 1));
        if (*(int *)(iVar8 + 1) == 0) {
          FUN_101aa780(&uStack_108,0x40,DAT_100eb97c,DAT_100eb954,*(undefined4 *)(pbVar3 + 5));
          FUN_10107704(&uStack_148,&uStack_108);
        }
      }
    }
    if (1 < *pbVar3 - 1) goto LAB_100eb762;
    uVar6 = DAT_100eb958;
    if (*pbVar3 != 1) {
      uVar6 = DAT_100eb95c;
    }
    FUN_101aa780(&uStack_148,0x40,uVar6,DAT_100eb954);
    iVar8 = FUN_10107030(iVar11,&uStack_148,0x13);
    if (iVar8 == 0) {
      uStack_c8 = DAT_100eb980;
      FUN_101bf3aa(&uStack_c4,DAT_100eb984,0xa2);
      FUN_1014c818(DAT_100eb988 | uVar9,DAT_100eb98c,uStack_c4,uStack_c3,uStack_c2,uStack_c0,
                   uStack_be,uStack_bc);
      FUN_100eac84(iVar11,0,&uStack_c8,0xa6);
      FUN_101071e0(iVar11);
      *(undefined4 *)(iVar10 + 0x1c) = 0;
    }
    else {
      FUN_1014c818(DAT_100eb960 | uVar9,DAT_100eb964,iVar8);
    }
  }
  else {
LAB_100eb762:
    iVar8 = 0;
  }
  iVar11 = DAT_100eb92c;
  bVar1 = *pbVar3;
  *(char *)(DAT_100eb92c + 0xb) = (char)param_1;
  if (bVar1 == 1) {
    uVar5 = 0xffffffff;
    uVar6 = uVar5;
  }
  else if (bVar1 == 2) {
    uVar5 = 0xfffffffe;
    uVar6 = uVar5;
  }
  else {
    if (bVar1 != 0xa5) goto LAB_100eb77e;
    uVar5 = *(undefined4 *)(DAT_100eb96c + 1);
    uVar6 = *(undefined4 *)(DAT_100eb96c + 5);
  }
  iVar8 = FUN_100de7a8(uVar5,uVar6);
LAB_100eb77e:
  if (iVar8 == 0) {
    *(char *)(iVar11 + 0xb) = (char)param_1;
  }
  else {
    FUN_1014c818(DAT_100eb930 | uVar9,DAT_100eb934,iVar8);
    *(undefined1 *)(iVar11 + 0xb) = 10;
  }
  iVar8 = FUN_100eb074(1,0xa065);
  if (iVar8 != 0) {
    FUN_1014c818(DAT_100eb938 | uVar9,DAT_100eb93c,0xa065,iVar8);
  }
  pcVar4 = DAT_100eb944;
  if (param_1 == 1) {
    uVar7 = 2;
  }
  else {
    uVar7 = 3;
  }
  *DAT_100eb940 = uVar7;
  cVar2 = *pcVar4;
  *(undefined1 *)(iVar10 + 0x12) = 0;
  if (cVar2 != '\0') {
    *pcVar4 = '\0';
    iVar10 = FUN_100f1058(0);
    if (0 < iVar10) {
      FUN_101bacba(0);
    }
    FUN_1014c818(DAT_100eb948 | uVar9,DAT_100eb94c,*pcVar4,DAT_100eb920);
  }
  return iVar8;
}


