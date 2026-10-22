// entry 0x100ec400  FUN_100ec400

void FUN_100ec400(void)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  char local_24;
  ushort local_23;
  ushort local_21;
  ushort local_1f;
  ushort local_1d;
  ushort local_1b;
  
  piVar1 = DAT_100ec560;
  uVar6 = (DAT_100ec558 - DAT_100ec554) * 0x20 & 0xff00;
  iVar7 = *DAT_100ec560;
  FUN_1014c818(DAT_100ec55c | uVar6,DAT_100ec564);
  uVar2 = DAT_100ec578;
  uVar8 = DAT_100ec568 | uVar6;
LAB_100ec428:
  do {
    while( true ) {
      while ((*(int *)(iVar7 + 0x14) == 0 ||
             (iVar3 = FUN_10147748(*(undefined4 *)(*piVar1 + 0x18),&local_24,0xb), iVar3 < 1))) {
        FUN_101be1ec(10);
      }
      if (local_24 == -0xb) break;
      uVar4 = uVar6 | 0x5c00011;
      uVar5 = DAT_100ec56c;
LAB_100ec456:
      FUN_1014c818(uVar4,uVar5);
    }
    local_23 = local_23 << 8 | local_23 >> 8;
    local_21 = local_21 << 8 | local_21 >> 8;
    local_1f = local_1f << 8 | local_1f >> 8;
    local_1d = local_1d << 8 | local_1d >> 8;
    local_1b = local_1b << 8 | local_1b >> 8;
    FUN_1014c818(uVar8,DAT_100ec570,local_1b,local_1f,local_1d,local_23);
    if (0x9065 < local_1b) {
      if (local_1b != 0x9075) {
switchD_100ec4e8_caseD_9061:
        uVar4 = uVar6 | uVar2;
        uVar5 = DAT_100ec574;
        goto LAB_100ec456;
      }
      FUN_100ebe24(&local_24);
      goto LAB_100ec428;
    }
    if (local_1b < 0x905e) {
      if (local_1b != 0x8052) goto switchD_100ec4e8_caseD_9061;
      FUN_100ec358(&local_24);
    }
    else {
      switch(local_1b) {
      case 0x905e:
        FUN_100eb344(&local_24);
        break;
      case 0x905f:
        FUN_100eb50c(&local_24);
        break;
      case 0x9060:
        FUN_100eb218(&local_24);
        break;
      default:
        goto switchD_100ec4e8_caseD_9061;
      case 0x9063:
        FUN_100ebac4(&local_24);
        break;
      case 0x9064:
        FUN_100ec19c(&local_24);
        break;
      case 0x9065:
        FUN_100eb990(&local_24);
      }
    }
  } while( true );
}


