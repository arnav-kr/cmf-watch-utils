// 0x1023c7bc FUN_10122508 :: als_w1160 Algorithm Version: %08X\n
// entry 0x10122508  FUN_10122508

void FUN_10122508(undefined4 param_1,undefined4 param_2)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 extraout_r1;
  int iVar5;
  uint uVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  
  pcVar1 = DAT_101225e4;
  if ((*DAT_101225e4 == '\0') && (*DAT_101225e8 != '\0')) {
    iVar5 = 0;
    puVar7 = DAT_101225ec;
    puVar8 = DAT_101225f0;
    do {
      param_2 = CONCAT13(*puVar7,(int3)param_2);
      iVar3 = FUN_101223c8(*puVar8,&stack0xffffffe7,1,*puVar7,param_1,param_2);
      iVar2 = DAT_101225fc;
      if (iVar3 != 0) {
        *pcVar1 = '\0';
        uVar6 = (iVar2 - DAT_10122600) * 0x20 & 0xff00U | 0x26c0000;
        uVar4 = DAT_1012261c;
        goto LAB_101225d6;
      }
      if (iVar5 == 0) {
        sched_13(0xf,0);
      }
      iVar2 = DAT_101225f4;
      iVar5 = iVar5 + 1;
      puVar7 = puVar7 + 1;
      puVar8 = puVar8 + 1;
    } while (iVar5 != 0x10);
    *(undefined2 *)(DAT_101225f4 + 100) = 0xff;
    *(undefined4 *)(iVar2 + 8) = 0x3f800000;
    *(undefined4 *)(iVar2 + 0x1c) = 0x3f800000;
    *(undefined4 *)(iVar2 + 0x44) = 0;
    *(undefined4 *)(iVar2 + 0x58) = 0;
    iVar5 = DAT_101225fc;
    *(undefined4 *)(iVar2 + 0x8c) = DAT_101225f8;
    *(undefined1 *)(iVar2 + 0x90) = 2;
    iVar3 = DAT_10122600;
    *(undefined4 *)(iVar2 + 0x30) = 0;
    uVar4 = FUN_10122ac8();
    uVar6 = (iVar5 - iVar3) * 0x20 & 0xff00;
    FUN_1014c818(DAT_10122604 | uVar6,DAT_10122608,uVar4);
    uVar4 = FUN_10122ad0();
    FUN_1014c818(uVar6 | 0x22c0031,DAT_1012260c,uVar4);
    FUN_10122ad8(iVar2);
    *DAT_10122610 = 0;
    uVar4 = DAT_10122614;
    *pcVar1 = '\x01';
    FUN_101db006(uVar4,extraout_r1,500,0);
    uVar6 = uVar6 | 0x2680000;
    uVar4 = DAT_10122618;
LAB_101225d6:
    FUN_1014c818(uVar6 | 0x31,uVar4);
  }
  return;
}


