// entry 0x100eb89c  thunk_FUN_100eb808  (100eb89c)

int thunk_FUN_100eb808(void)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined1 uVar8;
  uint unaff_r5;
  int unaff_r6;
  byte *unaff_r7;
  int unaff_r8;
  undefined4 in_stack_00000090;
  undefined1 uStack00000094;
  undefined1 uStack00000095;
  undefined2 in_stack_00000098;
  undefined2 in_stack_0000009c;
  
  if (*unaff_r7 - 1 < 2) {
    uVar7 = DAT_100eb958;
    if (*unaff_r7 != 1) {
      uVar7 = DAT_100eb95c;
    }
    FUN_101aa780(&stack0x00000010,0x40,uVar7,DAT_100eb954);
    iVar5 = FUN_10107030();
    if (iVar5 == 0) {
      in_stack_00000090 = DAT_100eb980;
      FUN_101bf3aa(&stack0x00000094,DAT_100eb984,0xa2);
      FUN_1014c818(DAT_100eb988 | unaff_r5,DAT_100eb98c,uStack00000094,uStack00000095);
      FUN_100eac84();
      FUN_101071e0();
      *(undefined4 *)(unaff_r8 + 0x1c) = 0;
    }
    else {
      FUN_1014c818(DAT_100eb960 | unaff_r5,DAT_100eb964,iVar5);
    }
  }
  else {
    iVar5 = 0;
  }
  iVar4 = DAT_100eb92c;
  bVar1 = *unaff_r7;
  *(char *)(DAT_100eb92c + 0xb) = (char)unaff_r6;
  if (bVar1 == 1) {
    uVar6 = 0xffffffff;
    uVar7 = uVar6;
  }
  else if (bVar1 == 2) {
    uVar6 = 0xfffffffe;
    uVar7 = uVar6;
  }
  else {
    if (bVar1 != 0xa5) goto LAB_100eb77e;
    uVar6 = *(undefined4 *)(DAT_100eb96c + 1);
    uVar7 = *(undefined4 *)(DAT_100eb96c + 5);
  }
  iVar5 = FUN_100de7a8(uVar6,uVar7);
LAB_100eb77e:
  if (iVar5 == 0) {
    *(char *)(iVar4 + 0xb) = (char)unaff_r6;
  }
  else {
    FUN_1014c818(DAT_100eb930 | unaff_r5,DAT_100eb934,iVar5);
    *(undefined1 *)(iVar4 + 0xb) = 10;
  }
  iVar5 = FUN_100eb074(1,0xa065);
  if (iVar5 != 0) {
    FUN_1014c818(DAT_100eb938 | unaff_r5,DAT_100eb93c,0xa065,iVar5);
  }
  pcVar3 = DAT_100eb944;
  if (unaff_r6 == 1) {
    uVar8 = 2;
  }
  else {
    uVar8 = 3;
  }
  *DAT_100eb940 = uVar8;
  cVar2 = *pcVar3;
  *(undefined1 *)(unaff_r8 + 0x12) = 0;
  if (cVar2 != '\0') {
    *pcVar3 = '\0';
    iVar4 = FUN_100f1058(0);
    if (0 < iVar4) {
      FUN_101bacba(0);
    }
    FUN_1014c818(DAT_100eb948 | unaff_r5,DAT_100eb94c,*pcVar3,DAT_100eb920);
  }
  return iVar5;
}


