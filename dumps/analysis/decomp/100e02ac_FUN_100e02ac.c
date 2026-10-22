// 0x10225cf8 FUN_100e02ac :: sys2ux update power info type:%d cmd:%d\n
// entry 0x100e02ac  FUN_100e02ac

undefined4 FUN_100e02ac(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 *puVar1;
  char cVar2;
  undefined1 uVar3;
  undefined1 *puVar4;
  char *pcVar5;
  int iVar6;
  undefined1 uVar7;
  bool bVar8;
  bool bVar9;
  uint uVar10;
  bool bVar11;
  
  uVar10 = (DAT_100e03e4 - DAT_100e03e0) * 0x20 & 0xff00;
  FUN_1014c818(uVar10 | 0x3b0032,DAT_100e03e8,*(undefined1 *)(param_2 + 2),
               *(undefined1 *)(param_2 + 3),param_4);
  pcVar5 = DAT_100e03f0;
  puVar4 = DAT_100e03ec;
  if (*(char *)(param_2 + 2) != '\x15') {
    if (*(char *)(param_2 + 2) == '\"') {
      if (*DAT_100e03f0 != '\x01') {
        return 0;
      }
      DAT_100e03ec[2] = 1;
      bVar9 = true;
    }
    else {
      bVar9 = false;
    }
    bVar11 = false;
    bVar8 = bVar11;
    goto LAB_100e0308;
  }
  cVar2 = *(char *)(param_2 + 3);
  if ((cVar2 == '\x01') || (cVar2 == '\x04')) {
    bVar11 = DAT_100e03ec[1] != '\x02';
    puVar1 = DAT_100e03ec + 1;
    if (bVar11) {
      DAT_100e03ec[2] = 0;
      *puVar1 = 2;
    }
    *DAT_100e03f0 = '\0';
LAB_100e02fa:
    if (cVar2 == '\b') {
      uVar7 = FUN_101ce6fc();
      *puVar4 = uVar7;
    }
    bVar9 = false;
    bVar8 = cVar2 == '\b';
  }
  else {
    bVar11 = false;
    if (cVar2 == '\x06') {
      DAT_100e03ec[2] = 0;
      puVar4[1] = 3;
    }
    else {
      if (cVar2 != '\x02') goto LAB_100e02fa;
      DAT_100e03ec[1] = 0;
      *pcVar5 = '\x01';
    }
    bVar9 = false;
    bVar11 = true;
    bVar8 = bVar9;
  }
LAB_100e0308:
  iVar6 = DAT_100e03f4;
  puVar4 = DAT_100e03ec;
  uVar3 = DAT_100e03ec[1];
  *(undefined1 *)(DAT_100e03f4 + 0x535d) = *DAT_100e03ec;
  *(undefined1 *)(iVar6 + 0x535e) = uVar3;
  uVar7 = puVar4[2];
  *(undefined1 *)(iVar6 + 0x535f) = uVar7;
  if (bVar11 == false) {
    if (bVar9 == false) {
      if (bVar8 == false) {
        return 0;
      }
      FUN_100d17cc();
      FUN_1014c818(uVar10 | 0x840031,DAT_100e0400,*puVar4);
      *(undefined1 *)(param_2 + 3) = 1;
    }
    else {
      FUN_1014c818(uVar10 | 0x7b0031,DAT_100e03fc,uVar7,bVar8,param_4);
    }
    FUN_101464b4(param_1,param_2);
  }
  else {
    FUN_1014c818(uVar10 | 0x680031,DAT_100e03f8,uVar3,bVar8,param_4);
    FUN_101bacb4(1);
    FUN_100d17cc();
    *(undefined1 *)(param_2 + 3) = puVar4[1];
    FUN_101464b4(param_1,param_2);
    FUN_101bacba(1);
  }
  return 0;
}


