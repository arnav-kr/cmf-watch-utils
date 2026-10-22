// 0x1023ac57 FUN_101c22fa :: _UpdatePageMapTblCache
// entry 0x101c22fa  FUN_101c22fa

undefined4 FUN_101c22fa(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  char cVar5;
  undefined4 uVar6;
  int iVar7;
  code *pcVar8;
  undefined1 *puVar9;
  int iVar10;
  undefined4 unaff_r4;
  undefined4 unaff_r5;
  int iVar11;
  
  iVar4 = FUN_101c1e2c();
  if (iVar4 == 1) {
    return 1;
  }
  iVar10 = param_1[3];
  iVar11 = *(int *)(iVar10 + 0x6c);
  bVar2 = FUN_101c1e78();
  iVar4 = (uint)bVar2 * 0xc;
  iVar7 = iVar4 + iVar10;
  *(int *)(iVar10 + 0x84) = iVar4 + 0x88 + iVar10;
  if ((*(char *)(iVar7 + 0x88) == '\0') || (cVar1 = *(char *)(iVar7 + 0x89), cVar1 == -1)) {
LAB_1011d710:
    uVar3 = FUN_101c144e(param_1,param_2);
    iVar4 = FUN_1011d538(param_1,uVar3);
    if (iVar4 == 1) {
      *(undefined1 *)(*(int *)(iVar10 + 0x84) + 1) = *(undefined1 *)(*(int *)(iVar10 + 0x6c) + 1);
      puVar9 = *(undefined1 **)(iVar10 + 0x84);
      *(short *)(puVar9 + 2) = (short)uVar3;
      *puVar9 = 0;
      return 1;
    }
  }
  else {
    cVar5 = *(char *)(iVar11 + 1);
    if (cVar5 == cVar1) {
LAB_1011d6da:
      iVar4 = FUN_101c1d0e(param_1,*(undefined1 *)((uint)bVar2 * 0xc + iVar10 + 0x8a),cVar5,0xc,
                           param_4,unaff_r4,unaff_r5);
      if (iVar4 == 1) {
        *(int *)(iVar10 + 0x6c) = iVar11;
        goto LAB_1011d710;
      }
      if (*(char *)(*param_1 + 0x10) == '\0') {
        return 0;
      }
      pcVar8 = *(code **)(*param_1 + 0xc);
      if (pcVar8 == (code *)0x0) {
        return 0;
      }
      uVar6 = 0x1eb;
      uVar3 = DAT_1011d750;
    }
    else {
      cVar5 = *(char *)(iVar10 + 0x71);
      if (cVar5 == cVar1) {
        *(int *)(iVar10 + 0x6c) = iVar10 + 0x70;
        goto LAB_1011d6da;
      }
      if (*(char *)(*param_1 + 0x10) == '\0') {
        return 0;
      }
      pcVar8 = *(code **)(*param_1 + 0xc);
      if (pcVar8 == (code *)0x0) {
        return 0;
      }
      uVar6 = 0x1e4;
      uVar3 = DAT_1011d754;
    }
    (*pcVar8)(uVar3,DAT_1011d74c,uVar6);
  }
  return 0;
}


