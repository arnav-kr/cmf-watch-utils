// entry 0x100dcfec  FUN_100dcfec

undefined4 FUN_100dcfec(void)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  undefined4 local_170;
  undefined4 uStack_16c;
  undefined1 auStack_168 [63];
  undefined1 local_129;
  char local_128;
  char local_127 [259];
  undefined4 local_24;
  
  uVar6 = (DAT_100dd110 - DAT_100dd10c) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100dd114 | uVar6,DAT_100dd11c,DAT_100dd118);
  local_170 = 0;
  uStack_16c = 0;
  iVar3 = FUN_101073f0(&local_170,DAT_100dd120);
  uVar2 = DAT_100dd154;
  if (iVar3 != 0) {
    return 0xffffffff;
  }
  uVar7 = uVar6 | DAT_100dd150;
LAB_100dd02a:
  do {
    iVar3 = FUN_101074ec(&local_170,&local_128);
    if ((iVar3 != 0) || (local_127[0] == '\0')) goto LAB_100dd0ec;
  } while (local_128 != '\0');
  FUN_1014c818(uVar7,DAT_100dd124,local_127,local_24);
  iVar3 = FUN_101ddf8a(local_127,DAT_100dd128);
  if (iVar3 == 0) {
    iVar3 = FUN_101ddf8a(local_127,DAT_100dd12c);
    bVar1 = false;
    if (iVar3 != 0) {
      bVar1 = true;
    }
  }
  else {
    bVar1 = true;
  }
  if (((!bVar1) && (iVar3 = FUN_101ddf8a(local_127,DAT_100dd138), iVar3 == 0)) &&
     (iVar3 = FUN_101ddf8a(local_127,DAT_100dd13c), iVar3 == 0)) {
    iVar3 = FUN_101ddf8a(local_127,DAT_100dd140);
    if (iVar3 == 0) goto LAB_100dd02a;
    iVar3 = FUN_100deaa4(local_127);
    iVar4 = FUN_100dc534();
    if (iVar4 == 0) {
LAB_100dd0ec:
      FUN_101075f8(&local_170);
      FUN_1014c818(DAT_100dd148 | uVar6,DAT_100dd14c,DAT_100dd118);
      return 0;
    }
    piVar5 = DAT_100dd144;
    for (iVar4 = 0; iVar4 < (int)(uint)*(byte *)(DAT_100dd144 + 2); iVar4 = iVar4 + 1) {
      piVar5 = piVar5 + 4;
      if (*piVar5 == iVar3) goto LAB_100dd02a;
    }
  }
  FUN_101ba1ae(auStack_168,0x40,DAT_100dd130,DAT_100dd120,local_127);
  local_129 = 0;
  FUN_1014c818(uVar6 | uVar2,DAT_100dd134,local_127,auStack_168);
  FUN_1010764c(auStack_168);
  goto LAB_100dd02a;
}


