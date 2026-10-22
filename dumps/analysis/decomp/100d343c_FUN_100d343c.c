// entry 0x100d343c  FUN_100d343c

int FUN_100d343c(undefined1 *param_1)

{
  char cVar1;
  char cVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  ushort uVar7;
  undefined2 uVar8;
  short sVar9;
  short sVar10;
  undefined1 *puVar11;
  int *piVar12;
  byte *pbVar13;
  byte *pbVar14;
  uint *puVar15;
  byte *pbVar16;
  byte *pbVar17;
  char *pcVar18;
  undefined4 *puVar19;
  undefined4 *puVar20;
  int iVar22;
  undefined4 uVar23;
  char *pcVar24;
  int iVar25;
  undefined4 uVar26;
  undefined4 *puVar27;
  undefined1 uVar28;
  uint uVar29;
  undefined4 uVar30;
  uint uVar31;
  uint uVar32;
  undefined1 *puVar33;
  int iVar34;
  uint uVar35;
  byte bVar36;
  uint uVar37;
  uint uVar38;
  ushort local_64;
  ushort local_62;
  undefined4 local_60;
  uint uStack_5c;
  uint uStack_58;
  uint local_54;
  uint local_50;
  undefined4 local_4c;
  undefined4 local_48;
  uint local_44;
  uint local_40;
  uint uStack_3c;
  undefined4 auStack_2c [2];
  undefined4 *puVar21;
  
  puVar11 = DAT_100d376c;
  uVar3 = *(ushort *)(param_1 + 1);
  uVar4 = *(ushort *)(param_1 + 3);
  uVar5 = *(ushort *)(param_1 + 5);
  uVar6 = *(ushort *)(param_1 + 7);
  uVar7 = *(ushort *)(param_1 + 9);
  *DAT_100d376c = *param_1;
  *(ushort *)(puVar11 + 9) = uVar7 << 8 | uVar7 >> 8;
  *(ushort *)(puVar11 + 3) = uVar4 << 8 | uVar4 >> 8;
  *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
  *(ushort *)(puVar11 + 5) = uVar5 << 8 | uVar5 >> 8;
  *(ushort *)(puVar11 + 7) = uVar6 << 8 | uVar6 >> 8;
  pcVar24 = DAT_100d3ac8;
  if (*(short *)(param_1 + 3) == -1) {
    uVar3 = *(ushort *)(param_1 + 9);
    if (0x908b < uVar3) {
switchD_100d3616_caseD_8048:
      FUN_100cc284(DAT_100d3770,*(undefined2 *)(param_1 + 1));
      FUN_1014c818(DAT_100d377c | (DAT_100d3778 - DAT_100d3774) * 0x20 & 0xff00U,DAT_100d3780);
      return 0;
    }
    if (uVar3 < 0x9052) {
      if (uVar3 < 0x8052) {
        if (uVar3 < 0x8047) {
          if (uVar3 == 0x8005) {
            local_4c = (uint)local_4c._2_2_ << 0x10;
            iVar22 = FUN_100cc284(puVar11 + 0xb,*(undefined2 *)(param_1 + 1));
            uVar29 = (DAT_100d3ad0 - DAT_100d3ad4) * 0x20 & 0xff00;
            if (iVar22 == 0) {
              FUN_1014c818(DAT_100d3b00 | uVar29,DAT_100d3b04);
              puVar11[0xb] = 1;
              uVar23 = FUN_101b605a(&local_4c,puVar11 + 0xb,1,5);
              uVar29 = local_4c & 0xffff;
              *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
              puVar11[10] = 5;
              puVar11[9] = 0;
              FUN_101ddb66(puVar11 + 0xb,uVar23,uVar29,0x200);
              pcVar24 = DAT_100d3b08;
              FUN_100cc2d4(puVar11,uVar29 + 0xb);
              if (*pcVar24 == '\0') {
                FUN_101bacb4();
                *pcVar24 = '\x01';
              }
              FUN_101be22e(DAT_100d3b0c,0x32);
            }
            else {
              FUN_1014c818(DAT_100d3b10 | uVar29,DAT_100d3ae8);
            }
            return 0;
          }
          if (uVar3 == 0x8006) {
            local_4c = (uint)local_4c._2_2_ << 0x10;
            iVar22 = FUN_100cc284(puVar11 + 0xb,*(undefined2 *)(param_1 + 1));
            uVar29 = (DAT_100d3ad0 - DAT_100d3ad4) * 0x20 & 0xff00;
            if (iVar22 == 0) {
              FUN_1014c818(uVar29 | 0x2700031,DAT_100d3aec);
              puVar11[0xb] = *DAT_100d3af0;
              puVar11[0xc] = *DAT_100d3af4;
              puVar11[0xd] = *DAT_100d3af8;
              puVar11[0xe] = *DAT_100d3afc;
              uVar23 = FUN_101b605a(&local_4c,puVar11 + 0xb,4,6);
              uVar29 = local_4c & 0xffff;
              *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
              puVar11[10] = 6;
              puVar11[9] = 0;
              FUN_101ddb66(puVar11 + 0xb,uVar23,uVar29,0x200);
              FUN_100cc2d4(puVar11,uVar29 + 0xb);
            }
            else {
              FUN_1014c818(uVar29 | 0x27c0031,DAT_100d3ae8);
            }
            return 0;
          }
          if (uVar3 == 0x8004) {
            FUN_100d2e78();
            return 0;
          }
        }
        else {
          switch(uVar3) {
          case 0x8047:
            uVar8 = *(undefined2 *)(param_1 + 1);
            *DAT_100d3784 = 1;
            puVar27 = DAT_100d3770;
            *DAT_100d3788 = 1;
            iVar22 = FUN_100cc284(puVar27,uVar8);
            uVar29 = (DAT_100d3778 - DAT_100d3774) * 0x20 & 0xff00;
            if (iVar22 != 0) {
              FUN_1014c818(uVar29 | 0x1220031,DAT_100d379c);
              return 0;
            }
            iVar22 = FUN_100cce30(DAT_100d378c + -0x10);
            if (iVar22 != 1) {
              FUN_1014c818(DAT_100d3794 | uVar29,DAT_100d3798);
              return 0;
            }
            FUN_1014c818(uVar29 | 0x1180031,DAT_100d3790);
            puVar19 = (undefined4 *)FUN_100cd138();
            puVar11[10] = 0x48;
            puVar27 = DAT_100d3770;
            puVar11[9] = 0;
            puVar20 = puVar19;
            do {
              puVar21 = puVar20 + 1;
              *puVar27 = *puVar20;
              puVar20 = puVar21;
              puVar27 = puVar27 + 1;
            } while (puVar21 != puVar19 + 0xc);
            FUN_100cc2d4(DAT_100d376c,0x3b);
            return 0;
          case 0x8049:
            FUN_100cc278();
            FUN_100cdcac(param_1);
            return 0;
          case 0x804a:
            FUN_1014c818(DAT_100d37a0 | (DAT_100d3778 - DAT_100d3774) * 0x20 & 0xff00U,DAT_100d37a4)
            ;
            return 0;
          case 0x804b:
            local_4c = (uint)local_4c._2_2_ << 0x10;
            iVar22 = FUN_100cc284(DAT_100d3770,*(undefined2 *)(param_1 + 1));
            uVar29 = (DAT_100d3778 - DAT_100d3774) * 0x20 & 0xff00;
            if (iVar22 == 0) {
              uVar23 = FUN_100cd1ac(&local_4c);
              FUN_1014c818(DAT_100d37a8 | uVar29,DAT_100d37ac,local_4c & 0xffff);
              uVar29 = local_4c & 0xffff;
              *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
              puVar11[10] = 0x4c;
              puVar27 = DAT_100d3770;
              puVar11[9] = 0;
              FUN_101ddb66(puVar27,uVar23,uVar29,0x200);
              FUN_100cc2d4(DAT_100d376c,uVar29 + 0xb);
            }
            else {
              FUN_1014c818(DAT_100d37b0 | uVar29,DAT_100d379c);
            }
            return 0;
          case 0x804d:
            FUN_100e7890();
            local_4c = local_4c & 0xffff0000;
            if (*pcVar24 == '\x02') {
              FUN_100e0514(2,0);
              *pcVar24 = '\0';
              FUN_100c6ccc();
            }
            iVar22 = FUN_100cc284(DAT_100d3acc,*(undefined2 *)(param_1 + 1));
            uVar29 = (DAT_100d3ad0 - DAT_100d3ad4) * 0x20 & 0xff00;
            if (iVar22 == 0) {
              FUN_1014c818(DAT_100d3ad8 | uVar29,DAT_100d3adc);
              uVar23 = DAT_100d3acc;
              puVar11[0xb] = 0x5a;
              uVar30 = FUN_101b605a(&local_4c,uVar23,1,4);
              uVar29 = local_4c & 0xffff;
              *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
              puVar11[10] = 4;
              uVar23 = DAT_100d3acc;
              puVar11[9] = 0;
              FUN_101ddb66(uVar23,uVar30,uVar29,0x200);
              FUN_100cc2d4(DAT_100d3ae0,uVar29 + 0xb);
            }
            else {
              FUN_1014c818(DAT_100d3ae4 | uVar29,DAT_100d3ae8);
            }
            return 0;
          case 0x8051:
            FUN_100d332c();
            return 0;
          }
        }
      }
      goto switchD_100d3616_caseD_8048;
    }
    switch(uVar3) {
    case 0x9052:
      FUN_100cc284(DAT_100d3acc,*(undefined2 *)(param_1 + 1));
      iVar22 = FUN_101ce6fc();
      piVar12 = DAT_100d3b18;
      if (iVar22 < 0x14) {
        uVar28 = 0xc;
      }
      else if (*DAT_100d3b18 == 0) {
        uVar28 = 2;
      }
      else {
        iVar22 = thunk_FUN_100d5e34(0x114,0);
        if (iVar22 == 0) {
          iVar22 = *piVar12;
          puVar11[0xb] = 1;
          FUN_100581e8(iVar22,1);
          goto LAB_100d398a;
        }
        uVar28 = 10;
      }
      puVar11[0xb] = uVar28;
LAB_100d398a:
      uVar23 = FUN_101b605a(&local_4c,DAT_100d3acc,1,0xa052);
      uVar3 = (ushort)local_4c;
      uVar29 = local_4c & 0xffff;
      FUN_101ddb66(DAT_100d3acc,uVar23,uVar29,0x200);
      *puVar11 = 0xf5;
      *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
      *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
      puVar11[6] = 1;
      puVar11[8] = 1;
      puVar11[9] = 0xa0;
      iVar22 = DAT_100d3ad0;
      puVar11[10] = 0x52;
      uVar23 = DAT_100d3b14;
      iVar22 = iVar22 - DAT_100d3ad4;
      puVar11[5] = 0;
      puVar11[7] = 0;
      FUN_1014c818(iVar22 * 0x20 & 0xff00U | 0x11c00021,uVar23,uVar29);
      FUN_100cc2d4(DAT_100d3ae0,(local_4c & 0xffff) + 0xb);
      return 0;
    case 0x9053:
      uVar29 = (DAT_100d3ad0 - DAT_100d3ad4) * 0x20 & 0xff00;
      local_60 = local_60 & 0xffff0000;
      FUN_1014c818(uVar29 | 0x19a0031,DAT_100d3b1c);
      FUN_101d0a9e(0);
      local_4c = 0;
      ux2sys_api_6(0x51,&local_4c);
      iVar22 = FUN_100cc284(DAT_100d3acc,*(undefined2 *)(param_1 + 1));
      uVar23 = DAT_100d3acc;
      if (iVar22 == 0) {
        puVar11[0xb] = 0x5a;
        uVar23 = FUN_101b605a(&local_60,uVar23,1,0xa053);
        uVar29 = local_60 & 0xffff;
        *(ushort *)(puVar11 + 1) = (ushort)local_60 << 8 | (ushort)local_60 >> 8;
        puVar11[9] = 0xa0;
        puVar11[10] = 0x53;
        FUN_101ddb66(DAT_100d3acc,uVar23,uVar29,0x200);
        FUN_100cc2d4(DAT_100d3ae0,uVar29 + 0xb);
      }
      else {
        FUN_1014c818(uVar29 | 0x1a80031,DAT_100d3ae8);
      }
      ux2sys_api_5(2,0);
      return 0;
    default:
      goto switchD_100d3616_caseD_8048;
    case 0x9055:
      FUN_100d3138();
      return 0;
    case 0x9056:
      FUN_100d25f8();
      return 0;
    case 0x9057:
      local_4c = (uint)local_4c._2_2_ << 0x10;
      iVar22 = FUN_100cc284(DAT_100d3e34,*(undefined2 *)(param_1 + 1));
      uVar29 = (DAT_100d3e38 - DAT_100d3e3c) * 0x20 & 0xff00;
      if (iVar22 == 0) {
        FUN_1014c818(DAT_100d3e40 | uVar29,DAT_100d3e44);
        iVar25 = DAT_100d3e4c;
        iVar22 = DAT_100d3e48;
        uVar23 = *(undefined4 *)(DAT_100d3e48 + 0x68);
        *(undefined2 *)(puVar11 + 0x10) = 0;
        *(undefined4 *)(puVar11 + 0xb) = uVar23;
        *(undefined1 *)(iVar25 + 2) = 0;
        uVar23 = DAT_100d3e54;
        uVar29 = DAT_100d3e50 | uVar29;
        puVar11[0xf] = *(undefined1 *)(iVar22 + 0x43bb);
        puVar33 = (undefined1 *)(iVar25 + -5);
        iVar22 = 0;
        do {
          iVar25 = iVar22 + 1;
          FUN_1014c818(uVar29,uVar23,iVar22,*puVar33);
          puVar33 = puVar33 + 1;
          iVar22 = iVar25;
        } while (iVar25 != 8);
        uVar23 = FUN_101b605a(&local_4c,DAT_100d3e34,8,0xa057);
        uVar29 = local_4c & 0xffff;
        *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
        puVar11[9] = 0xa0;
        puVar11[10] = 0x57;
        FUN_101ddb66(DAT_100d3e34,uVar23,uVar29,0x200);
        FUN_100cc2d4(DAT_100d3e58,uVar29 + 0xb);
      }
      else {
        FUN_1014c818(DAT_100d3e5c | uVar29,DAT_100d3e60);
      }
      return 0;
    case 0x9058:
      local_4c = (uint)local_4c._2_2_ << 0x10;
      iVar22 = FUN_100cc284(DAT_100d3e34,*(undefined2 *)(param_1 + 1));
      uVar29 = (DAT_100d3e38 - DAT_100d3e3c) * 0x20 & 0xff00;
      if (iVar22 == 0) {
        iVar22 = FUN_101b608c(&local_4c,DAT_100d3e34,*(undefined2 *)(param_1 + 1),0x8004);
        if (iVar22 == 0) {
          FUN_1014c818(DAT_100d3e64 | uVar29,DAT_100d3e68);
          return 0;
        }
        FUN_1014c818(DAT_100d3e6c | uVar29,DAT_100d3e70,local_4c & 0xffff);
      }
      else {
        FUN_1014c818(DAT_100d3e74 | uVar29,DAT_100d3e60);
      }
      return 0;
    case 0x9059:
      FUN_100cdfbc();
      return 0;
    case 0x905c:
      FUN_100ce158();
      return 0;
    case 0x905d:
      FUN_100d239c();
      return 0;
    case 0x9066:
      FUN_100d23f4();
      return 0;
    case 0x9067:
      FUN_100cd754();
      return 0;
    case 0x9068:
      FUN_100cd8c8();
      return 0;
    case 0x9069:
      FUN_100d24e0();
      return 0;
    case 0x906a:
      FUN_100d2c48();
      return 0;
    case 0x906b:
      local_4c = (uint)local_4c._2_2_ << 0x10;
      iVar22 = FUN_100cc284(DAT_100d3e34,*(undefined2 *)(param_1 + 1));
      uVar29 = (DAT_100d3e38 - DAT_100d3e3c) * 0x20 & 0xff00;
      if (iVar22 == 0) {
        iVar22 = FUN_101b608c(&local_4c,DAT_100d3e34,*(undefined2 *)(param_1 + 1),0x906b);
        if (iVar22 == 0) {
          FUN_1014c818(DAT_100d3e78 | uVar29,DAT_100d3e7c);
          return 0;
        }
        FUN_1014c818(uVar29 | 0xa800031,DAT_100d3e80,local_4c & 0xffff);
        uVar29 = local_4c & 0xffff;
        if (uVar29 - 0x70 < 0x58) {
          FUN_101ddb66(DAT_100d3e34,iVar22,uVar29,0x200);
          FUN_100ce2a4(uVar29);
        }
      }
      else {
        FUN_1014c818(DAT_100d3e84 | uVar29,DAT_100d3e88);
      }
      return 0;
    case 0x906d:
      FUN_100d0594();
      return 0;
    case 0x906e:
      FUN_100cdec4();
      return 0;
    case 0x906f:
      FUN_100d03a8();
      return 0;
    case 0x9070:
      FUN_100cff1c();
      return 0;
    case 0x9071:
      FUN_100cfb84();
      return 0;
    case 0x9072:
      FUN_100cf218();
      return 0;
    case 0x9073:
      FUN_100cf520();
      return 0;
    case 0x9074:
      FUN_100cfa50();
      return 0;
    case 0x9076:
      FUN_100cf100();
      return 0;
    case 0x9077:
      FUN_1004b848();
      return 0;
    case 0x9078:
      FUN_100cf8b0();
      return 0;
    case 0x9079:
      FUN_100cda3c();
      return 0;
    case 0x907a:
      FUN_100ce3c4();
      return 0;
    case 0x907f:
      FUN_100cdb74();
      return 0;
    case 0x9080:
      local_60 = (uint)local_60._2_2_ << 0x10;
      local_4c = (uint)local_4c._2_2_ << 0x10;
      iVar22 = FUN_100cc284(DAT_100d3e34,*(undefined2 *)(param_1 + 1));
      uVar29 = (DAT_100d3e38 - DAT_100d3e3c) * 0x20 & 0xff00;
      if (iVar22 == 0) {
        pcVar24 = (char *)FUN_101b608c(&local_60,DAT_100d3e34,*(undefined2 *)(param_1 + 1),0x9080);
        if (pcVar24 == (char *)0x0) {
          FUN_1014c818(DAT_100d3e8c | uVar29,DAT_100d3e90);
          return 0;
        }
        FUN_1014c818(DAT_100d3e94 | uVar29,DAT_100d3e98,local_60 & 0xffff);
        uVar23 = DAT_100d3e34;
        if (*pcVar24 == -0x5b) {
          puVar11[0xb] = 0x5a;
          uVar23 = FUN_101b605a(&local_4c,uVar23,1,0xa080);
          FUN_101ddb66(DAT_100d3e34,uVar23,local_4c & 0xffff,0x200);
        }
        *puVar11 = 0xf5;
        *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
        puVar11[3] = 0xff;
        puVar11[4] = 0xff;
        puVar11[9] = 0xa0;
        uVar23 = DAT_100d3e58;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[10] = 0x80;
        FUN_100cc2d4(uVar23,(local_4c & 0xffff) + 0xb);
        ux2sys_api_5(1,0);
      }
      else {
        FUN_1014c818(DAT_100d4128 | uVar29,DAT_100d412c);
      }
      return 0;
    case 0x9083:
      FUN_100d003c();
      return 0;
    case 0x908a:
      goto switchD_100d351c_caseD_908a;
    case 0x908b:
      local_4c = (uint)local_4c._2_2_ << 0x10;
      iVar22 = FUN_100cc284(DAT_100d4130,*(undefined2 *)(param_1 + 1));
      if (iVar22 == 0) {
        pcVar24 = (char *)FUN_101b608c(&local_4c,DAT_100d4130,*(undefined2 *)(param_1 + 1),0x907f);
        if (pcVar24 == (char *)0x0) {
          FUN_1014c818(DAT_100d4154 | (DAT_100d4138 - DAT_100d4134) * 0x20 & 0xff00U,DAT_100d4140);
          return 0;
        }
        if (*pcVar24 == -0x5b) {
          FUN_100d328c();
        }
      }
      else {
        FUN_1014c818(DAT_100d4158 | (DAT_100d4138 - DAT_100d4134) * 0x20 & 0xff00U,DAT_100d4150);
      }
      return 0;
    }
  }
  local_64 = 0;
  if (*(short *)(param_1 + 1) == 0) {
    iVar22 = 0;
  }
  else {
    iVar22 = FUN_100cc284(puVar11 + 0xb);
    if (iVar22 == 1) {
      FUN_1014c818(DAT_100d415c | (DAT_100d4138 - DAT_100d4134) * 0x20 & 0xff00U,DAT_100d4160);
      return 1;
    }
    iVar25 = FUN_101b608c(&local_64,puVar11 + 0xb,*(undefined2 *)(param_1 + 1),
                          *(undefined2 *)(param_1 + 3));
    if (iVar25 == 0) {
      FUN_1014c818(DAT_100d4164 | (DAT_100d4138 - DAT_100d4134) * 0x20 & 0xff00U,DAT_100d4168);
      return -1;
    }
    FUN_101ddb66(puVar11 + 0xb,iVar25,local_64,0x200);
  }
  pcVar18 = DAT_100d55e8;
  pbVar17 = DAT_100d5388;
  pbVar16 = DAT_100d4ff8;
  pcVar24 = DAT_100d4c58;
  pbVar14 = DAT_100d48ec;
  puVar33 = DAT_100d4528;
  pbVar13 = DAT_100d44dc;
  uVar3 = *(ushort *)(param_1 + 3);
  if (uVar3 < 0x66) {
    if (uVar3 < 0x53) {
      return iVar22;
    }
    switch(uVar3) {
    case 0x53:
      FUN_100cd404(param_1);
      return iVar22;
    case 0x54:
      if (*(short *)(param_1 + 9) == 1) {
        local_48._0_2_ = *(undefined2 *)(puVar11 + 0xf);
        local_48._2_2_ = *(undefined2 *)(puVar11 + 0x11);
        local_4c = *(uint *)(puVar11 + 0xb);
        FUN_101b5932(local_4c,*DAT_100d4500 * 0xe10 + DAT_100d4500[1] * 0x3c,DAT_100d4528);
        uVar29 = (DAT_100d44e4 - DAT_100d44e8) * 0x20 & 0xff00;
        FUN_1014c818(uVar29 | DAT_100d4508,DAT_100d4504,*(undefined2 *)(puVar33 + 6),puVar33[4],
                     puVar33[3],puVar33[2],puVar33[1],*puVar33);
        FUN_1014c818(uVar29 | DAT_100d450c,DAT_100d4510,(undefined2)local_48,local_48._2_2_);
        FUN_101b9a32(2,&local_4c);
        *puVar11 = 0xf5;
        puVar11[1] = 0;
        puVar11[2] = 0;
        uVar3 = *(ushort *)(param_1 + 3);
        param_1[9] = 3;
        param_1[10] = 0;
        puVar11[10] = 3;
        uVar23 = DAT_100d44f0;
        *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        FUN_100cc2d4(uVar23,0xb);
        return iVar22;
      }
      return iVar22;
    default:
      return iVar22;
    case 0x56:
      FUN_100cd5f4(param_1);
      return iVar22;
    case 0x57:
      FUN_1014c818(DAT_100d4514 | (DAT_100d44e4 - DAT_100d44e8) * 0x20 & 0xff00U,DAT_100d44f8,0x57,
                   *(undefined2 *)(param_1 + 9),*DAT_100d44dc);
      if (*pbVar13 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 2) {
        *DAT_100d4518 = 1;
        *puVar11 = 0xf5;
        puVar11[1] = 0;
        puVar11[2] = 0;
        param_1[9] = 1;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        uVar23 = DAT_100d44f0;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        puVar11[10] = 1;
        FUN_100cc2d4(uVar23,0xb);
        return iVar22;
      }
      return iVar22;
    case 0x58:
      FUN_1014c818(DAT_100d48a4 | (DAT_100d48a0 - DAT_100d489c) * 0x20 & 0xff00U,DAT_100d48a8,0x58,
                   *(undefined2 *)(param_1 + 9),*DAT_100d48ec);
      if (*pbVar14 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 2) {
        *DAT_100d48ac = 1;
        *puVar11 = 0xf5;
        puVar11[1] = 0;
        puVar11[2] = 0;
        param_1[9] = 1;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        uVar23 = DAT_100d48b0;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        puVar11[10] = 1;
        FUN_100cc2d4(uVar23,0xb);
        return iVar22;
      }
      return iVar22;
    case 0x5a:
      local_4c = (uint)local_4c._2_2_ << 0x10;
      uVar29 = (DAT_100d48a0 - DAT_100d489c) * 0x20 & 0xff00;
      FUN_1014c818(DAT_100d48b4 | uVar29,DAT_100d48b8,0x5a,*(undefined2 *)(param_1 + 9),
                   *(undefined2 *)(param_1 + 1),*DAT_100d48ec);
      if (*pbVar14 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 1) {
        iVar25 = FUN_100e0618(0xcc,0x10,DAT_100d48bc);
        FUN_1014c818(DAT_100d48c0 | uVar29,DAT_100d48c4,iVar25);
        if (iVar25 != 0) {
          puVar11[0xb] = 3;
          uVar23 = FUN_101b605a(&local_4c,DAT_100d48bc,1,*(undefined2 *)(param_1 + 3));
          FUN_101ddb66(DAT_100d48bc,uVar23,local_4c & 0xffff,0x200);
        }
        *puVar11 = 0xf5;
        *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
        param_1[9] = 3;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        puVar11[10] = 3;
        uVar23 = DAT_100d48b0;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        FUN_100cc2d4(uVar23,(local_4c & 0xffff) + 0xb);
        return iVar22;
      }
      return iVar22;
    case 0x5b:
      FUN_1014c818(DAT_100d55d4 | (DAT_100d55d0 - DAT_100d55cc) * 0x20 & 0xff00U,DAT_100d55d8,0x5b,
                   *(undefined2 *)(param_1 + 9),*(undefined2 *)(param_1 + 1),*DAT_100d55e8);
      if (*pcVar18 == '\0') {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 1) {
        FUN_100e05ec(0xd2,puVar11[0xb]);
        *puVar11 = 0xf5;
        puVar11[1] = 0;
        puVar11[2] = 0;
        uVar3 = *(ushort *)(param_1 + 3);
        param_1[9] = 3;
        param_1[10] = 0;
        puVar11[10] = 3;
        uVar23 = DAT_100d55dc;
        *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        FUN_100cc2d4(uVar23,0xb);
        return iVar22;
      }
      return iVar22;
    case 0x5c:
      FUN_100cd508(param_1);
      return iVar22;
    case 0x5d:
      uVar29 = (uint)*DAT_100d48ec;
      local_4c = (uint)local_4c._2_2_ << 0x10;
      uVar37 = (DAT_100d48a0 - DAT_100d489c) * 0x20 & 0xff00;
      FUN_1014c818(DAT_100d48c8 | uVar37,DAT_100d48a8,0x5d,*(undefined2 *)(param_1 + 9),uVar29);
      if (*pbVar14 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 1) {
        FUN_1014c818(DAT_100d48cc | uVar37,DAT_100d48d0,puVar11[0xb],1,uVar29);
        if (puVar11[0xb] == '\0') {
          iVar25 = thunk_FUN_100d6460(0x104);
        }
        else {
          iVar25 = thunk_FUN_100d5e34(0x104,0);
        }
        FUN_1014c818(uVar37 | 0x7200031,DAT_100d48c4,iVar25);
        if (iVar25 != 0) {
          puVar11[0xb] = 3;
          uVar23 = FUN_101b605a(&local_4c,DAT_100d48bc,1,*(undefined2 *)(param_1 + 3));
          FUN_101ddb66(DAT_100d48bc,uVar23,local_4c & 0xffff,0x200);
        }
        *puVar11 = 0xf5;
        *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
        param_1[9] = 3;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        puVar11[10] = 3;
        uVar23 = DAT_100d48b0;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        FUN_100cc2d4(uVar23,(local_4c & 0xffff) + 0xb);
        return iVar22;
      }
      return iVar22;
    case 0x5e:
      FUN_100ce544(param_1);
      return iVar22;
    case 0x5f:
      local_4c = (uint)local_4c._2_2_ << 0x10;
      uVar29 = (DAT_100d48a0 - DAT_100d489c) * 0x20 & 0xff00;
      FUN_1014c818(DAT_100d48d4 | uVar29,DAT_100d48a8,0x5f,*(undefined2 *)(param_1 + 9),
                   *DAT_100d48ec);
      if (*pbVar14 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) != 1) {
        if (*(short *)(param_1 + 9) == 2) {
          param_1[9] = 1;
          iVar25 = DAT_100d48d8;
          param_1[10] = 0;
          if (*(char *)(iVar25 + 0x23) == -1) {
            puVar11[0xb] = 0;
          }
          else {
            puVar11[0xb] = *(char *)(iVar25 + 0x23);
          }
          uVar23 = FUN_101b605a(&local_4c,DAT_100d48bc,1,*(undefined2 *)(param_1 + 3));
          uVar3 = (ushort)local_4c;
          uVar37 = local_4c & 0xffff;
          FUN_101ddb66(DAT_100d48bc,uVar23,uVar37,0x200);
          *puVar11 = 0xf5;
          *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
          *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
          puVar11[5] = 0;
          puVar11[6] = 1;
          puVar11[8] = 1;
          puVar11[7] = 0;
          uVar23 = DAT_100d48b0;
          *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
          FUN_100cc2d4(uVar23,uVar37 + 0xb);
          FUN_1014c818(DAT_100d48e4 | uVar29,DAT_100d48e8,puVar11[0xb]);
          return iVar22;
        }
        return iVar22;
      }
      *(undefined1 *)(DAT_100d48d8 + 0x23) = puVar11[0xb];
      FUN_100e05ec(0xde,0);
      FUN_1014c818(DAT_100d48dc | uVar29,DAT_100d48e0,puVar11[0xb]);
      *puVar11 = 0xf5;
      *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
      param_1[9] = 3;
      param_1[10] = 0;
      uVar23 = DAT_100d48b0;
      *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
      puVar11[5] = 0;
      puVar11[6] = 1;
      puVar11[7] = 0;
      puVar11[8] = 1;
      puVar11[9] = 0;
      puVar11[10] = 3;
      FUN_100cc2d4(uVar23,(local_4c & 0xffff) + 0xb);
      return iVar22;
    case 0x60:
      FUN_1014c818(DAT_100d4c44 | (DAT_100d4c40 - DAT_100d4c3c) * 0x20 & 0xff00U,DAT_100d4c48,0x60,
                   *(undefined2 *)(param_1 + 9),*DAT_100d4c58);
      puVar15 = DAT_100d4c4c;
      if (*pcVar24 == '\0') {
        return iVar22;
      }
      local_60 = *(undefined4 *)(DAT_100d4c64 + 0x7f8);
      uStack_5c = *(uint *)(DAT_100d4c64 + 0x7fc);
      uStack_58 = *(uint *)(DAT_100d4c64 + 0x800);
      local_54 = *(uint *)(DAT_100d4c64 + 0x804);
      local_50 = *(uint *)(DAT_100d4c64 + 0x808);
      local_62 = 0;
      cVar1 = *(char *)(DAT_100d4c64 + 0x7f8);
      sVar9 = *(short *)(DAT_100d4c64 + 0x7fa);
      uVar29 = *(uint *)(DAT_100d4c64 + 0x804);
      uVar37 = *(uint *)(DAT_100d4c64 + 0x808);
      if (*(short *)(param_1 + 9) != 1) {
        if (*(short *)(param_1 + 9) == 2) {
          param_1[10] = 0;
          param_1[9] = 1;
          puVar11[0xb] = cVar1;
          puVar11[0xd] = (char)sVar9;
          puVar11[0xe] = (char)(uVar29 >> 0x18);
          puVar11[0xf] = (char)(uVar29 >> 0x10);
          puVar11[0x11] = (char)uVar29;
          puVar11[0x12] = (char)(uVar37 >> 0x18);
          puVar11[0x13] = (char)(uVar37 >> 0x10);
          puVar11[0xc] = (char)((ushort)sVar9 >> 8);
          puVar11[0x10] = (char)(uVar29 >> 8);
          puVar11[0x14] = (char)(uVar37 >> 8);
          puVar11[0x15] = (char)uVar37;
          uVar23 = FUN_101b605a(&local_62,DAT_100d4c54,0xb,*(undefined2 *)(param_1 + 3));
          uVar3 = local_62;
          uVar29 = (uint)local_62;
          FUN_101ddb66(DAT_100d4c54,uVar23,uVar29,0x200);
          *puVar11 = 0xf5;
          *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
          uVar3 = *(ushort *)(param_1 + 3);
          puVar11[5] = 0;
          *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
          puVar11[6] = 1;
          puVar11[7] = 0;
          puVar11[8] = 1;
          uVar23 = DAT_100d4c50;
          *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
          FUN_100cc2d4(uVar23,uVar29 + 0xb);
          return iVar22;
        }
        return iVar22;
      }
      cVar2 = puVar11[0xb];
      uVar31 = *(uint *)(puVar11 + 0xe);
      uVar35 = *(uint *)(puVar11 + 0x12);
      sVar10 = CONCAT11(puVar11[0xc],puVar11[0xd]);
      uVar31 = uVar31 << 0x18 | (uVar31 >> 8 & 0xff) << 0x10 | (uVar31 >> 0x10 & 0xff) << 8 |
               uVar31 >> 0x18;
      uVar35 = uVar35 << 0x18 | (uVar35 >> 8 & 0xff) << 0x10 | (uVar35 >> 0x10 & 0xff) << 8 |
               uVar35 >> 0x18;
      if (((cVar1 != cVar2) || (sVar10 != sVar9)) || ((uVar31 != uVar29 || (uVar37 != uVar35)))) {
        local_60._1_3_ = (undefined3)(local_60 >> 8);
        local_60 = CONCAT31(local_60._1_3_,cVar2);
        local_60 = CONCAT22(sVar10,(ushort)local_60);
        local_4c._1_3_ = (undefined3)(local_60 >> 8);
        local_4c = CONCAT31(local_4c._1_3_,cVar2);
        local_4c = CONCAT22(sVar10,(ushort)local_4c);
        *DAT_100d4c4c = local_4c;
        puVar15[1] = uStack_5c;
        puVar15[2] = uStack_58;
        puVar15[3] = uVar31;
        puVar15[4] = uVar35;
        local_54 = uVar31;
        local_50 = uVar35;
        local_48 = uStack_5c;
        local_44 = uStack_58;
        local_40 = uVar31;
        uStack_3c = uVar35;
      }
      puVar11[1] = 0;
      *puVar11 = 0xf5;
      puVar11[2] = 0;
      *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
      param_1[9] = 3;
      param_1[10] = 0;
      uVar23 = DAT_100d4c50;
      puVar11[10] = 3;
      puVar11[5] = 0;
      puVar11[6] = 1;
      puVar11[7] = 0;
      puVar11[8] = 1;
      puVar11[9] = 0;
      FUN_100cc2d4(uVar23,0xb);
      return iVar22;
    case 0x61:
      FUN_1014c818((DAT_100d4c40 - DAT_100d4c3c) * 0x20 & 0xff00U | 0x7580033,DAT_100d4c5c,0x61,
                   *(undefined2 *)(param_1 + 9),*DAT_100d4c58);
      puVar15 = DAT_100d4c60;
      if (*pcVar24 == '\0') {
        return iVar22;
      }
      local_60 = *(undefined4 *)(DAT_100d4c64 + 0x7e4);
      uStack_5c = *(uint *)(DAT_100d4c64 + 0x7e8);
      uStack_58 = *(uint *)(DAT_100d4c64 + 0x7ec);
      local_54 = *(uint *)(DAT_100d4c64 + 0x7f0);
      local_50 = *(uint *)(DAT_100d4c64 + 0x7f4);
      local_62 = 0;
      cVar1 = *(char *)(DAT_100d4c64 + 0x7e4);
      sVar9 = *(short *)(DAT_100d4c64 + 0x7e6);
      uVar29 = *(uint *)(DAT_100d4c64 + 0x7f0);
      uVar37 = *(uint *)(DAT_100d4c64 + 0x7f4);
      if (*(short *)(param_1 + 9) != 1) {
        if (*(short *)(param_1 + 9) == 2) {
          param_1[10] = 0;
          param_1[9] = 1;
          puVar11[0xb] = cVar1;
          puVar11[0xd] = (char)sVar9;
          puVar11[0xe] = (char)(uVar29 >> 0x18);
          puVar11[0xf] = (char)(uVar29 >> 0x10);
          puVar11[0x11] = (char)uVar29;
          puVar11[0x12] = (char)(uVar37 >> 0x18);
          puVar11[0x13] = (char)(uVar37 >> 0x10);
          puVar11[0xc] = (char)((ushort)sVar9 >> 8);
          puVar11[0x10] = (char)(uVar29 >> 8);
          puVar11[0x14] = (char)(uVar37 >> 8);
          puVar11[0x15] = (char)uVar37;
          uVar23 = FUN_101b605a(&local_62,DAT_100d4c54,0xb,*(undefined2 *)(param_1 + 3));
          uVar3 = local_62;
          uVar29 = (uint)local_62;
          FUN_101ddb66(DAT_100d4c54,uVar23,uVar29,0x200);
          *puVar11 = 0xf5;
          *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
          uVar3 = *(ushort *)(param_1 + 3);
          puVar11[5] = 0;
          *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
          puVar11[6] = 1;
          puVar11[7] = 0;
          puVar11[8] = 1;
          uVar23 = DAT_100d4c50;
          *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
          FUN_100cc2d4(uVar23,uVar29 + 0xb);
          return iVar22;
        }
        return iVar22;
      }
      cVar2 = puVar11[0xb];
      uVar31 = *(uint *)(puVar11 + 0xe);
      uVar35 = *(uint *)(puVar11 + 0x12);
      sVar10 = CONCAT11(puVar11[0xc],puVar11[0xd]);
      uVar31 = uVar31 << 0x18 | (uVar31 >> 8 & 0xff) << 0x10 | (uVar31 >> 0x10 & 0xff) << 8 |
               uVar31 >> 0x18;
      uVar35 = uVar35 << 0x18 | (uVar35 >> 8 & 0xff) << 0x10 | (uVar35 >> 0x10 & 0xff) << 8 |
               uVar35 >> 0x18;
      if ((((cVar1 != cVar2) || (sVar10 != sVar9)) || (uVar31 != uVar29)) || (uVar35 != uVar37)) {
        local_60._1_3_ = (undefined3)(local_60 >> 8);
        local_60 = CONCAT31(local_60._1_3_,cVar2);
        local_60 = CONCAT22(sVar10,(ushort)local_60);
        local_4c._1_3_ = (undefined3)(local_60 >> 8);
        local_4c = CONCAT31(local_4c._1_3_,cVar2);
        local_4c = CONCAT22(sVar10,(ushort)local_4c);
        *DAT_100d4c60 = local_4c;
        puVar15[1] = uStack_5c;
        puVar15[2] = uStack_58;
        puVar15[3] = uVar31;
        puVar15[4] = uVar35;
        local_54 = uVar31;
        local_50 = uVar35;
        local_48 = uStack_5c;
        local_44 = uStack_58;
        local_40 = uVar31;
        uStack_3c = uVar35;
        FUN_100e6d70();
      }
      *puVar11 = 0xf5;
      *(ushort *)(puVar11 + 1) = local_62 << 8 | local_62 >> 8;
      param_1[9] = 3;
      param_1[10] = 0;
      *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
      puVar11[10] = 3;
      uVar23 = DAT_100d4c50;
      puVar11[5] = 0;
      puVar11[6] = 1;
      puVar11[7] = 0;
      puVar11[8] = 1;
      puVar11[9] = 0;
      FUN_100cc2d4(uVar23,local_62 + 0xb);
      return iVar22;
    case 0x62:
      uVar23 = 0;
      uVar29 = (uint)*DAT_100d4ff8;
      uVar37 = (DAT_100d4f94 - DAT_100d4f90) * 0x20 & 0xff00;
      local_60 = (uint)local_60._2_2_ << 0x10;
      FUN_1014c818(DAT_100d4f98 | uVar37,DAT_100d4f9c,0x62,*(undefined2 *)(param_1 + 9),uVar29,0);
      iVar25 = DAT_100d4fa4;
      if (*pbVar16 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) != 1) {
        if (*(short *)(param_1 + 9) == 2) {
          param_1[9] = 1;
          param_1[10] = 0;
          uVar31 = DAT_100d4fa8;
          uVar28 = *(undefined1 *)(iVar25 + 0x35);
          puVar11[0xb] = uVar28;
          FUN_1014c818(uVar31 | uVar37,DAT_100d4fac,uVar28,uVar28,uVar29,uVar23);
          uVar23 = FUN_101b605a(&local_60,DAT_100d4fb0,1,*(undefined2 *)(param_1 + 3));
          uVar3 = (ushort)local_60;
          uVar29 = local_60 & 0xffff;
          FUN_101ddb66(DAT_100d4fb0,uVar23,uVar29,0x200);
          *puVar11 = 0xf5;
          *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
          uVar3 = *(ushort *)(param_1 + 3);
          puVar11[5] = 0;
          *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
          puVar11[6] = 1;
          puVar11[7] = 0;
          puVar11[8] = 1;
          uVar23 = DAT_100d4fa0;
          *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
          FUN_100cc2d4(uVar23,uVar29 + 0xb);
          return iVar22;
        }
        return iVar22;
      }
      local_4c = (uint)(byte)puVar11[0xb];
      ux2sys_api_6(0x77,&local_4c);
      *puVar11 = 0xf5;
      *(ushort *)(puVar11 + 1) = (ushort)local_60 << 8 | (ushort)local_60 >> 8;
      param_1[9] = 3;
      param_1[10] = 0;
      uVar23 = DAT_100d4fa0;
      *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
      puVar11[5] = 0;
      puVar11[6] = 1;
      puVar11[7] = 0;
      puVar11[8] = 1;
      puVar11[9] = 0;
      puVar11[10] = 3;
      FUN_100cc2d4(uVar23,(local_60 & 0xffff) + 0xb);
      FUN_100e05ec(0xd0,0);
      return iVar22;
    case 99:
      *(ushort *)(param_1 + 1) = local_64;
      FUN_100ce934(param_1);
      return iVar22;
    case 100:
      FUN_1014c818(DAT_100d4fb4 | (DAT_100d4f94 - DAT_100d4f90) * 0x20 & 0xff00U,DAT_100d4fb8);
      FUN_100e4d70(DAT_100d4fbc,(char)local_64 + -4,0);
      *DAT_100d4fc0 = 1;
      return iVar22;
    case 0x65:
      goto switchD_100d4090_caseD_65;
    }
  }
  if (uVar3 < 0xa0) {
    if (uVar3 < 0x95) {
      return iVar22;
    }
    switch(uVar3) {
    case 0x95:
      FUN_100d225c(param_1);
      return iVar22;
    default:
      return iVar22;
    case 0x99:
      uVar29 = (uint)*DAT_100d4ff8;
      local_60 = (uint)local_60._2_2_ << 0x10;
      uVar37 = (DAT_100d4f94 - DAT_100d4f90) * 0x20 & 0xff00;
      FUN_1014c818(DAT_100d4fe8 | uVar37,DAT_100d4fec,0x99,*(undefined2 *)(param_1 + 9),uVar29);
      FUN_1014c818(DAT_100d4ff0 | uVar37,DAT_100d4ff4,puVar11[0xb]);
      if (*pbVar16 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) != 1) {
        if (*(short *)(param_1 + 9) == 2) {
          param_1[9] = 1;
          param_1[10] = 0;
          local_4c = 0xff;
          ux2sys_api_7(9,&local_4c);
          uVar31 = DAT_100d533c;
          puVar11[0xb] = (undefined1)local_4c;
          FUN_1014c818(uVar31 | uVar37,DAT_100d5340,local_4c & 0xff,local_4c,uVar29);
          uVar23 = FUN_101b605a(&local_60,DAT_100d5344,1,*(undefined2 *)(param_1 + 3));
          uVar3 = (ushort)local_60;
          uVar29 = local_60 & 0xffff;
          FUN_101ddb66(DAT_100d5344,uVar23,uVar29,0x200);
          *puVar11 = 0xf5;
          *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
          uVar3 = *(ushort *)(param_1 + 3);
          puVar11[5] = 0;
          *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
          puVar11[6] = 1;
          puVar11[7] = 0;
          puVar11[8] = 1;
          uVar23 = DAT_100d5348;
          *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
          FUN_100cc2d4(uVar23,uVar29 + 0xb);
          return iVar22;
        }
        return iVar22;
      }
      local_4c = (uint)(byte)puVar11[0xb];
      if (local_4c < 2) {
        ux2sys_api_6(9,&local_4c);
        *puVar11 = 0xf5;
        *(ushort *)(puVar11 + 1) = (ushort)local_60 << 8 | (ushort)local_60 >> 8;
        param_1[9] = 3;
        param_1[10] = 0;
        uVar23 = DAT_100d4fa0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        puVar11[10] = 3;
        FUN_100cc2d4(uVar23,(local_60 & 0xffff) + 0xb);
        FUN_100e05ec(0xd0,0);
        return iVar22;
      }
      return iVar22;
    case 0x9a:
      FUN_1014c818(DAT_100d5354 | (DAT_100d5350 - DAT_100d534c) * 0x20 & 0xff00U,DAT_100d5358,0x9a,
                   *(undefined2 *)(param_1 + 9),*DAT_100d5388);
      if (*pbVar17 == 0) {
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 1) {
        *puVar11 = 0xf5;
        puVar11[1] = 0;
        puVar11[2] = 0;
        uVar3 = *(ushort *)(param_1 + 3);
        param_1[9] = 3;
        param_1[10] = 0;
        puVar11[10] = 3;
        uVar23 = DAT_100d5348;
        *(ushort *)(puVar11 + 3) = uVar3 << 8 | uVar3 >> 8;
        puVar11[5] = 0;
        puVar11[6] = 1;
        puVar11[7] = 0;
        puVar11[8] = 1;
        puVar11[9] = 0;
        FUN_100cc2d4(uVar23,0xb);
        FUN_1019b814(DAT_100d535c);
        ux2sys_api_5(2,0);
        return iVar22;
      }
      return iVar22;
    case 0x9b:
      goto switchD_100d40f8_caseD_9b;
    case 0x9f:
      if (*(short *)(param_1 + 9) == 1) {
        FUN_1014c818(DAT_100d5380 | (DAT_100d5350 - DAT_100d534c) * 0x20 & 0xff00U,DAT_100d5384,
                     (byte)puVar11[0xb] - 1);
        uVar3 = *(ushort *)(param_1 + 5);
        uVar4 = *(ushort *)(param_1 + 7);
        *puVar11 = 0xf5;
        puVar11[1] = 0;
        puVar11[2] = 0;
        param_1[9] = 3;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        *(ushort *)(puVar11 + 5) = uVar3 << 8 | uVar3 >> 8;
        uVar23 = DAT_100d5348;
        *(ushort *)(puVar11 + 7) = uVar4 << 8 | uVar4 >> 8;
        puVar11[9] = 0;
        puVar11[10] = 3;
        FUN_100cc2d4(uVar23,0xb);
        return iVar22;
      }
      if (*(short *)(param_1 + 9) == 2) {
        FUN_100d2d5c();
        return iVar22;
      }
      return iVar22;
    }
  }
  if (uVar3 != 0xde) {
    if (0xde < uVar3) {
      if (uVar3 != 0x155) {
        return iVar22;
      }
      local_4c = local_4c & 0xffff0000;
      uVar23 = FUN_100c148c();
      pbVar13 = DAT_100d44dc;
      uVar29 = (uint)*DAT_100d44dc;
      uVar37 = (DAT_100d44e4 - DAT_100d44e8) * 0x20 & 0xff00;
      FUN_1014c818(uVar37 | DAT_100d44f4,DAT_100d44f8,*(undefined2 *)(param_1 + 3),
                   *(undefined2 *)(param_1 + 9),uVar29);
      if (*pbVar13 != 0) {
        if (*(short *)(param_1 + 9) == 2) {
          iVar25 = ux2sys_api_7(0x97,0);
          param_1[10] = 0;
          param_1[9] = 1;
          uVar30 = *(undefined4 *)(iVar25 + 0x3c);
          *(undefined4 *)(puVar11 + 0xb) = uVar23;
          *(undefined4 *)(puVar11 + 0xf) = uVar30;
          uVar31 = DAT_100d4520;
          *(undefined4 *)(puVar11 + 0x13) = *(undefined4 *)(iVar25 + 0x40);
          *(undefined4 *)(puVar11 + 0x17) = *(undefined4 *)(iVar25 + 0x44);
          puVar33 = DAT_100d44fc;
          *(undefined4 *)(puVar11 + 0x1b) = *(undefined4 *)(iVar25 + 0x48);
          uVar23 = DAT_100d4524;
          *(undefined2 *)(puVar11 + 0x1f) = *(undefined2 *)(iVar25 + 0x4c);
          *(undefined2 *)(puVar11 + 0x21) = *(undefined2 *)(iVar25 + 0x4e);
          *(undefined2 *)(puVar11 + 0x23) = *(undefined2 *)(iVar25 + 0x50);
          iVar25 = 0;
          do {
            iVar34 = iVar25 + 1;
            FUN_1014c818(uVar37 | uVar31,uVar23,iVar25,*puVar33,uVar29);
            iVar25 = iVar34;
            puVar33 = puVar33 + 1;
          } while (iVar34 != 0x1a);
          uVar23 = FUN_101b605a(&local_4c,DAT_100d44fc,0x1a,*(undefined2 *)(param_1 + 3));
          uVar3 = (ushort)local_4c;
          uVar29 = local_4c & 0xffff;
          FUN_101ddb66(DAT_100d44fc,uVar23,uVar29,0x200);
          *puVar11 = 0xf5;
          *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
          *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
          puVar11[5] = 0;
          puVar11[6] = 1;
          puVar11[8] = 1;
          puVar11[7] = 0;
          uVar23 = DAT_100d44f0;
          *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
          FUN_100cc2d4(uVar23,uVar29 + 0xb);
          return iVar22;
        }
        return iVar22;
      }
      return iVar22;
    }
    if (uVar3 == 0xd5) {
      *(ushort *)(param_1 + 1) = local_64;
      FUN_100cec74(param_1);
      return iVar22;
    }
    if (uVar3 != 0xdc) {
      return iVar22;
    }
    iVar25 = 0;
    local_60 = (uint)local_60._2_2_ << 0x10;
    local_4c = 0;
    FUN_101bf3b2(&local_48,0,0x20);
    if (*DAT_100d44dc != 0) {
      if (*(short *)(param_1 + 9) == 1) {
        bVar36 = puVar11[0xb];
        if (0x23 < bVar36) {
          bVar36 = 0x24;
        }
        FUN_101ddb66(&local_4c,DAT_100d44e0,bVar36,0x24);
        uVar29 = (DAT_100d44e4 - DAT_100d44e8) * 0x20 & 0xff00U | DAT_100d451c;
        puVar33 = DAT_100d44fc;
        do {
          iVar34 = iVar25 + 1;
          FUN_1014c818(uVar29,DAT_100d44ec,iVar25,*puVar33);
          iVar25 = iVar34;
          puVar33 = puVar33 + 1;
        } while (iVar34 < (char)(bVar36 + 1));
        FUN_100c8dfc(bVar36,&local_4c);
        uVar29 = local_60 & 0xffff;
        uVar3 = *(ushort *)(param_1 + 5);
        uVar4 = *(ushort *)(param_1 + 7);
        *(ushort *)(puVar11 + 1) = (ushort)local_60 << 8 | (ushort)local_60 >> 8;
        *puVar11 = 0xf5;
        param_1[9] = 3;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        *(ushort *)(puVar11 + 5) = uVar3 << 8 | uVar3 >> 8;
        *(ushort *)(puVar11 + 7) = uVar4 << 8 | uVar4 >> 8;
        puVar11[9] = 0;
        puVar11[10] = 3;
      }
      else {
        if (*(short *)(param_1 + 9) != 2) goto LAB_100d4254;
        iVar34 = FUN_100c8db8(&local_4c);
        puVar11[0xb] = (char)iVar34;
        FUN_101ddb66(DAT_100d55e0,&local_4c,iVar34,0x1ff);
        uVar23 = DAT_100d55fc;
        uVar29 = iVar34 + 1U & 0xff;
        uVar37 = (DAT_100d55d0 - DAT_100d55cc) * 0x20 & 0xff00U | DAT_100d55f8;
        puVar33 = DAT_100d55e4;
        for (; iVar25 < (int)uVar29; iVar25 = iVar25 + 1) {
          FUN_1014c818(uVar37,uVar23,iVar25,*puVar33);
          puVar33 = puVar33 + 1;
        }
        uVar23 = FUN_101b605a(&local_60,DAT_100d55e4,uVar29,*(undefined2 *)(param_1 + 3));
        uVar5 = (ushort)local_60;
        uVar29 = local_60 & 0xffff;
        FUN_101ddb66(DAT_100d55e4,uVar23,uVar29,0x200);
        uVar3 = *(ushort *)(param_1 + 5);
        uVar4 = *(ushort *)(param_1 + 7);
        *(ushort *)(puVar11 + 1) = uVar5 << 8 | uVar5 >> 8;
        *puVar11 = 0xf5;
        param_1[9] = 1;
        param_1[10] = 0;
        *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
        *(ushort *)(puVar11 + 5) = uVar3 << 8 | uVar3 >> 8;
        *(ushort *)(puVar11 + 7) = uVar4 << 8 | uVar4 >> 8;
        puVar11[9] = 0;
        puVar11[10] = 1;
      }
      FUN_100cc2d4(DAT_100d44f0,uVar29 + 0xb);
    }
LAB_100d4254:
    FUN_100e05ec(0xda,0);
    return iVar22;
  }
  param_1[3] = 0xde;
  local_60 = (uint)local_60._2_2_ << 0x10;
  cVar1 = *DAT_100d55e8;
  param_1[4] = 0;
  if (cVar1 == '\0') {
    return iVar22;
  }
  if (*(short *)(param_1 + 9) != 2) {
    return iVar22;
  }
  param_1[9] = 1;
  param_1[10] = 0;
  local_4c = 0;
  FUN_101bf3b2(&local_48,0,0x1d);
  iVar25 = FUN_1010e214(DAT_100d55ec,&local_4c,0x20);
  if (iVar25 < 1) {
    FUN_1014c818((DAT_100d55d0 - DAT_100d55cc) * 0x20 & 0xff00U | 0x10e00012,DAT_100d55f0,
                 DAT_100d55f4);
    uVar29 = 1;
    puVar11[0xb] = 0;
  }
  else {
    puVar27 = &local_4c;
    puVar20 = DAT_100d55e0;
    do {
      uVar23 = *puVar27;
      uVar30 = puVar27[1];
      puVar27 = puVar27 + 2;
      *puVar20 = uVar23;
      puVar20[1] = uVar30;
      puVar20 = puVar20 + 2;
    } while (puVar27 != auStack_2c);
    *(undefined1 *)puVar20 = *(undefined1 *)puVar27;
    puVar11[0xb] = (char)iVar25;
    uVar29 = iVar25 + 1U & 0xff;
    if (uVar29 == 0) goto LAB_100d5586;
  }
  uVar23 = FUN_101b605a(&local_60,DAT_100d55e4,uVar29,*(undefined2 *)(param_1 + 3));
  FUN_101ddb66(DAT_100d55e4,uVar23,local_60 & 0xffff,0x200);
LAB_100d5586:
  *puVar11 = 0xf5;
  *(ushort *)(puVar11 + 1) = (ushort)local_60 << 8 | (ushort)local_60 >> 8;
  *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
  puVar11[5] = 0;
  puVar11[6] = 1;
  puVar11[8] = 1;
  puVar11[7] = 0;
  uVar23 = DAT_100d55dc;
  *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
  FUN_100cc2d4(uVar23,(local_60 & 0xffff) + 0xb);
  return iVar22;
switchD_100d4090_caseD_65:
  uVar37 = (uint)local_64;
  iVar25 = DAT_100d4f94 - DAT_100d4f90;
  uVar29 = (uint)*DAT_100d4ff8;
  *(ushort *)(param_1 + 1) = local_64;
  uVar31 = (uint)*(ushort *)(param_1 + 9);
  uVar35 = iVar25 * 0x20 & 0xff00;
  FUN_1014c818(DAT_100d4fc4 | uVar35,DAT_100d4fcc,DAT_100d4fc8,0x65,uVar31,uVar37,uVar29);
  if (*pbVar16 == 0) {
    return iVar22;
  }
  if (*(short *)(param_1 + 9) != 1) {
    return iVar22;
  }
  uVar32 = (uint)*(ushort *)(param_1 + 1);
  if (0x1f8 < uVar32 - 1) {
    FUN_1014c818(DAT_100d4fd0 | uVar35,DAT_100d4fd4,DAT_100d4fc8,uVar32,uVar31,uVar37,uVar29);
    return iVar22;
  }
  uVar38 = (uint)(byte)puVar11[0x11];
  uVar32 = uVar32 - uVar38 & 0xffff;
  uVar31 = uVar32 - 7 & 0xffff;
  FUN_1014c818(DAT_100d4fd8 | uVar35,DAT_100d4fdc,DAT_100d4fc8,uVar38,uVar31,uVar37,uVar29);
  if ((uVar38 - 1 < 0x3b) && (uVar32 - 8 < 0xff)) {
    FUN_100c45a4(DAT_100d4fb0,*(undefined2 *)(param_1 + 1));
    thunk_FUN_100d5e34(0x105,0);
    FUN_101d7326(0x16,0);
    *puVar11 = 0xf5;
    puVar11[1] = 0;
    puVar11[2] = 0;
    puVar11[3] = 0;
    puVar11[5] = 0;
    puVar11[7] = 0;
    puVar11[9] = 0;
    uVar23 = DAT_100d4fa0;
    puVar11[4] = 0x65;
    puVar11[6] = 1;
    puVar11[8] = 1;
    puVar11[10] = 3;
    FUN_100cc2d4(uVar23,0xb);
    return iVar22;
  }
  FUN_1014c818(DAT_100d4fe0 | uVar35,DAT_100d4fe4,DAT_100d4fc8,uVar38 - 1,uVar31,uVar37,uVar29);
  return iVar22;
switchD_100d351c_caseD_908a:
  local_60 = (uint)local_60._2_2_ << 0x10;
  local_4c = (uint)local_4c._2_2_ << 0x10;
  iVar22 = FUN_100cc284(DAT_100d4130,*(undefined2 *)(param_1 + 1));
  if (iVar22 != 0) {
    FUN_1014c818(DAT_100d414c | (DAT_100d4138 - DAT_100d4134) * 0x20 & 0xff00U,DAT_100d4150);
    return 0;
  }
  pcVar24 = (char *)FUN_101b608c(&local_60,DAT_100d4130,*(undefined2 *)(param_1 + 1),0x907f);
  if (pcVar24 == (char *)0x0) {
    FUN_1014c818(DAT_100d413c | (DAT_100d4138 - DAT_100d4134) * 0x20 & 0xff00U,DAT_100d4140);
    return 0;
  }
  if (*pcVar24 == '\0') {
    uVar28 = 2;
    puVar11[0xb] = 1;
    puVar11[0xc] = (byte)(((uint)*(byte *)(DAT_100d4144 + 0x67) << 0x1d) >> 0x1f);
  }
  else {
    if (*pcVar24 != '\x01') goto LAB_100d3f20;
    *(byte *)(DAT_100d4144 + 0x67) = *(byte *)(DAT_100d4144 + 0x67) & 0xfb | (pcVar24[1] & 1U) << 2;
    FUN_100e05ec(0xe4,0);
    puVar11[0xb] = 0;
    uVar28 = 1;
  }
  uVar23 = FUN_101b605a(&local_4c,DAT_100d4130,uVar28,0xa068);
  FUN_101ddb66(DAT_100d4130,uVar23,local_4c & 0xffff,0x200);
LAB_100d3f20:
  *puVar11 = 0xf5;
  *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
  puVar11[3] = 0xff;
  puVar11[4] = 0xff;
  puVar11[6] = 1;
  puVar11[8] = 1;
  puVar11[9] = 0xa0;
  uVar23 = DAT_100d4148;
  puVar11[5] = 0;
  puVar11[7] = 0;
  puVar11[10] = 0x8a;
  FUN_100cc2d4(uVar23,(local_4c & 0xffff) + 0xb);
  return 0;
switchD_100d40f8_caseD_9b:
  uVar29 = (uint)*DAT_100d5388;
  local_4c = (uint)local_4c._2_2_ << 0x10;
  uVar37 = (DAT_100d5350 - DAT_100d534c) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100d5360 | uVar37,DAT_100d5358,0x9b,*(undefined2 *)(param_1 + 9),uVar29);
  iVar25 = DAT_100d5364;
  if (*pbVar17 == 0) {
    return iVar22;
  }
  if (*(short *)(param_1 + 9) == 1) {
    cVar1 = puVar11[0xb];
    if (cVar1 == '\x01') {
      cVar1 = puVar11[0xc];
      if (*(char *)(DAT_100d5364 + 0x38) != cVar1) {
        *(char *)(DAT_100d5364 + 0x38) = cVar1;
        *(char *)(iVar25 + 0x55) = cVar1;
        FUN_101b9f64();
        FUN_100e05ec(0xd1,0);
      }
    }
    else if (cVar1 == '\x02') {
      cVar1 = puVar11[0xc];
      if (*(char *)(DAT_100d5364 + 0x39) != cVar1) {
        *(char *)(DAT_100d5364 + 0x39) = cVar1;
        if (cVar1 == '\0') {
          FUN_100e70b0();
        }
        else {
          FUN_100e6e48();
        }
      }
    }
    else if ((cVar1 == '\x04') && (cVar1 = puVar11[0xc], *(char *)(DAT_100d5364 + 100) != cVar1)) {
      *(char *)(DAT_100d5364 + 100) = cVar1;
      if (cVar1 == '\0') {
        FUN_100e8274();
      }
      else {
        FUN_100e82b0();
      }
    }
    *puVar11 = 0xf5;
    *(ushort *)(puVar11 + 1) = (ushort)local_4c << 8 | (ushort)local_4c >> 8;
    param_1[9] = 3;
    param_1[10] = 0;
    *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
    puVar11[10] = 3;
    uVar23 = DAT_100d5348;
    puVar11[5] = 0;
    puVar11[6] = 1;
    puVar11[7] = 0;
    puVar11[8] = 1;
    puVar11[9] = 0;
    FUN_100cc2d4(uVar23,(local_4c & 0xffff) + 0xb);
    return iVar22;
  }
  if (*(short *)(param_1 + 9) != 2) {
    return iVar22;
  }
  param_1[10] = 0;
  param_1[9] = 1;
  uVar32 = DAT_100d537c;
  uVar26 = DAT_100d5378;
  uVar35 = DAT_100d5374;
  uVar30 = DAT_100d5370;
  uVar31 = DAT_100d536c;
  uVar23 = DAT_100d5368;
  iVar25 = DAT_100d5364;
  cVar1 = puVar11[0xb];
  if (cVar1 == '\x01') {
    uVar28 = *(undefined1 *)(DAT_100d5364 + 0x38);
    puVar11[0xc] = uVar28;
    uVar32 = uVar31;
    uVar26 = uVar23;
  }
  else if (cVar1 == '\x02') {
    uVar28 = *(undefined1 *)(DAT_100d5364 + 0x39);
    puVar11[0xc] = uVar28;
    uVar32 = uVar35;
    uVar26 = uVar30;
  }
  else {
    if (cVar1 != '\x04') goto LAB_100d5226;
    uVar28 = *(undefined1 *)(DAT_100d5364 + 100);
    puVar11[0xc] = uVar28;
  }
  FUN_1014c818(uVar32 | uVar37,uVar26,uVar28,iVar25,uVar29);
LAB_100d5226:
  uVar23 = FUN_101b605a(&local_4c,DAT_100d5344,8,*(undefined2 *)(param_1 + 3));
  uVar3 = (ushort)local_4c;
  uVar29 = local_4c & 0xffff;
  FUN_101ddb66(DAT_100d5344,uVar23,uVar29,0x200);
  *puVar11 = 0xf5;
  *(ushort *)(puVar11 + 1) = uVar3 << 8 | uVar3 >> 8;
  *(ushort *)(puVar11 + 3) = *(ushort *)(param_1 + 3) << 8 | *(ushort *)(param_1 + 3) >> 8;
  puVar11[5] = 0;
  puVar11[6] = 1;
  puVar11[8] = 1;
  puVar11[7] = 0;
  uVar23 = DAT_100d5348;
  *(ushort *)(puVar11 + 9) = *(ushort *)(param_1 + 9) << 8 | *(ushort *)(param_1 + 9) >> 8;
  FUN_100cc2d4(uVar23,uVar29 + 0xb);
  return iVar22;
}


