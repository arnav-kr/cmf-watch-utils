// entry 0x100e7f78  FUN_100e7f78

int FUN_100e7f78(void)

{
  char cVar1;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  uint uVar14;
  int iVar15;
  uint uVar16;
  int iVar17;
  uint uVar18;
  uint uVar19;
  undefined1 uStack_48;
  byte local_47;
  byte local_46;
  char local_45;
  char local_44;
  char local_43;
  undefined1 uStack_3c;
  byte local_3b;
  byte local_3a;
  char local_39;
  char local_38;
  char local_37;
  char cVar2;
  
  FUN_100c14b4(&uStack_48);
  uVar3 = FUN_101b5950(&uStack_48,*DAT_100e8190 * 0xe10 + DAT_100e8190[1] * 0x3c);
  iVar13 = DAT_100e8198;
  iVar5 = uVar3 - DAT_100e8194 * (uVar3 / DAT_100e8194);
  uVar14 = 0;
  uVar12 = 0xffffffff;
  uVar11 = ((int)PTR_DAT_100e81a0 - (int)PTR_LAB_100e819c) * 0x20 & 0xff00;
  FUN_1014c818(uVar11 | 0xe30032,PTR_s_Todo_expire_time_cb__now__u____u_100e81a4,uVar3,iVar5);
  uVar6 = DAT_100e81a8 | uVar11;
  iVar15 = iVar13;
  do {
    iVar17 = DAT_100e81c4;
    if (*(byte *)(DAT_100e8198 + 8) <= uVar14) {
      for (uVar6 = 0; uVar6 < *(byte *)(DAT_100e8198 + 8); uVar6 = uVar6 + 1) {
        uVar14 = *(uint *)(uVar6 * 0xc + iVar17);
        if (uVar3 < uVar14) {
          uVar4 = uVar14 - uVar3;
          if (uVar4 <= uVar12) {
            uVar12 = uVar4;
          }
          FUN_1014c818(uVar11 | 0x1360033,DAT_100e81c0,uVar6,uVar14,uVar4);
        }
      }
      if (uVar12 == 0xffffffff) {
        uVar12 = 0x2a30;
      }
      FUN_1014c818(uVar11 | 0x13e0031,PTR_s_Todo_next_expire_time__u_100e81ac,uVar12);
      return uVar12 * 1000;
    }
    if ((*(char *)(iVar13 + 0xd) != '\0') && (*(char *)(iVar13 + 0x14) == '\0')) {
      uVar16 = *(uint *)(iVar13 + 0x10);
      FUN_100e7a74(uVar16,&uStack_3c);
      uVar4 = (uint)*(byte *)(iVar13 + 0x16);
      switch(uVar4) {
      case 0:
        uVar10 = (uint)(*(char *)(iVar15 + 0x19ad) == '\0');
        break;
      case 1:
        uVar10 = 1;
        break;
      case 2:
        cVar1 = local_43;
        cVar2 = local_37;
        goto LAB_100e8126;
      case 4:
        if (local_38 != local_44) goto switchD_100e8050_default;
      case 3:
        cVar1 = local_45;
        cVar2 = local_39;
LAB_100e8126:
        uVar10 = (uint)(cVar2 == cVar1);
        break;
      case 5:
        if (local_43 == '\0') {
          uVar10 = 0x40;
        }
        else {
          uVar10 = 1 << (uint)(byte)(local_43 - 1) & 0xff;
        }
        uVar10 = (uint)((uVar10 & *(byte *)(iVar13 + 0x15)) != 0);
        break;
      default:
switchD_100e8050_default:
        uVar10 = 0;
      }
      uVar7 = (uint)local_3b;
      uVar8 = (uint)local_3a;
      uVar9 = (uint)*(byte *)(iVar13 + 0x15);
      uVar18 = uVar16;
      uVar19 = uVar10;
      FUN_1014c818(uVar6,PTR_s_Todo__u____u____u__today__u__mod_100e81b0,uVar14,uVar3,uVar16,uVar10,
                   uVar4,uVar9,uVar8,uVar7);
      if (uVar10 != 0) {
        iVar17 = uVar16 - DAT_100e8194 * (uVar16 / DAT_100e8194);
        if ((local_3a < local_46) || ((local_3a == local_46 && (local_3b <= local_47)))) {
          FUN_1014c818(uVar11 | 0x1160032,PTR_s_Todo_is_expire___02u__02u__100e81b4,local_3a,
                       local_3b,uVar18,uVar19,uVar4,uVar9,uVar8,uVar7);
          iVar17 = iVar17 + 0x15180;
        }
        uVar16 = DAT_100e81b8;
        if ((uint)local_46 * 0x3c + local_47 + 0x10000 < *(uint *)(iVar15 + 0x19b0)) {
          *(undefined4 *)(iVar15 + 0x19b0) = 0;
        }
        uVar10 = iVar17 - iVar5;
        if (uVar10 <= uVar12) {
          uVar12 = uVar10;
        }
        FUN_1014c818(uVar16 | uVar11,DAT_100e81bc,iVar5,iVar17,uVar10,uVar12,uVar4,uVar9,uVar8,uVar7
                    );
      }
    }
    uVar14 = uVar14 + 1;
    iVar13 = iVar13 + 0xa4;
    iVar15 = iVar15 + 0xc;
  } while( true );
}


