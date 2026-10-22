// 0x10226d10 FUN_100e2ee8 :: _sensor_res_callback MSG_APP_UPDATE_ALGO_DATA
// entry 0x100e2ee8  FUN_100e2ee8

/* WARNING: Heritage AFTER dead removal. Example location: s1 : 0x100e3768 */
/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined4 FUN_100e2ee8(undefined4 param_1,undefined4 param_2,int param_3,int *param_4)

{
  byte bVar1;
  short sVar2;
  ushort uVar3;
  undefined1 uVar4;
  undefined4 *puVar5;
  char *pcVar6;
  int *piVar7;
  int *piVar8;
  byte *pbVar9;
  char *pcVar10;
  int iVar11;
  undefined4 uVar12;
  uint *puVar13;
  int iVar14;
  byte bVar15;
  uint uVar16;
  int iVar17;
  uint uVar18;
  uint uVar19;
  int iVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint in_fpscr;
  float fVar28;
  undefined4 uVar29;
  undefined8 uVar30;
  int local_44;
  uint local_40;
  undefined4 local_3c;
  uint local_38;
  undefined2 local_34;
  
  iVar17 = DAT_100e31e4;
  uVar3 = *(ushort *)(DAT_100e31e4 + 0x52c2);
  if (param_3 == 0) {
    return 0;
  }
  FUN_101ba194(DAT_100e31ec,DAT_100e31e8,param_3);
  pbVar9 = DAT_100e379c;
  if (0x16 < param_3) {
    if (param_3 == 100) {
      if (*param_4 < 1) {
        return 0;
      }
      iVar11 = FUN_100c148c();
      piVar7 = DAT_100e3774;
      iVar20 = *param_4;
      iVar14 = *DAT_100e3770;
      uVar12 = FUN_100c148c();
      uVar12 = FUN_100ca61c(iVar20,uVar12,(uint)(iVar11 - iVar14) < 0xb);
      uVar21 = (DAT_100e3778 - DAT_100e377c) * 0x20 & 0xff00;
      FUN_1014c818(DAT_100e3780 | uVar21,DAT_100e3784,uVar12);
      iVar11 = FUN_100c148c();
      iVar11 = FUN_101ddb02(iVar11 - *piVar7);
      if (iVar11 < 3) {
        return 0;
      }
      *(int *)(iVar17 + 0x4230) = *param_4;
      uVar12 = FUN_100c148c();
      *(undefined4 *)(iVar17 + 0x422c) = uVar12;
      uVar18 = *(uint *)(iVar17 + 0x4230);
      if (*(byte *)(iVar17 + 0x4228) < uVar18) {
        *(char *)(iVar17 + 0x4228) = (char)uVar18;
      }
      if ((uVar18 < *(byte *)(iVar17 + 0x4229)) || (*(byte *)(iVar17 + 0x4229) == 0)) {
        *(char *)(iVar17 + 0x4229) = (char)uVar18;
      }
      if (uVar18 != *DAT_100e3788) {
        *DAT_100e3788 = (ushort)uVar18;
        FUN_100e05ec(0xd7,0);
      }
      local_40 = *(undefined4 *)(iVar17 + 0x4230);
      local_44 = *(int *)(iVar17 + 0x422c);
      if (local_44 != *piVar7) {
        *piVar7 = local_44;
        FUN_100c6114(4,&local_44,8);
      }
      FUN_100c14b4(&local_3c);
      iVar11 = DAT_100e378c;
      uVar18 = (uint)local_3c._2_1_ * 0x3c + (uint)local_3c._1_1_;
      uVar19 = uVar18 / 0x1e;
      uVar18 = uVar18 / 0x3c;
      if (0x2e < uVar19) {
        uVar19 = 0x2f;
      }
      if (0x16 < uVar18) {
        uVar18 = 0x17;
      }
      uVar24 = *(uint *)(iVar17 + 0x4230);
      uVar4 = (undefined1)uVar24;
      *(undefined1 *)(DAT_100e378c + uVar19) = uVar4;
      *(short *)(iVar17 + 0x4234) = (short)uVar19 + 1;
      iVar17 = DAT_100e3790;
      iVar14 = uVar18 + 0x18;
      if (*(byte *)(iVar11 + iVar14 * 2) < uVar24) {
        *(undefined1 *)(iVar11 + iVar14 * 2) = uVar4;
      }
      uVar19 = (uint)*(byte *)(iVar17 + iVar14 * 2);
      if ((uVar24 < uVar19) || (uVar19 == 0)) {
        *(undefined1 *)(iVar17 + iVar14 * 2) = uVar4;
      }
      FUN_1014c818(DAT_100e3794 | uVar21,DAT_100e3798,uVar18,*(undefined1 *)(iVar11 + iVar14 * 2),
                   *(undefined1 *)(iVar17 + iVar14 * 2),uVar24);
      return 0;
    }
    if (param_3 != 0x65) {
      return 0;
    }
    *(undefined1 *)(iVar17 + 0x533a) = *(undefined1 *)((int)param_4 + 0xe);
    *(undefined1 *)(iVar17 + 0x533b) = *(undefined1 *)((int)param_4 + 0xf);
    *(char *)(iVar17 + 0x533c) = (char)param_4[4];
    *(short *)(iVar17 + 0x5338) = (short)param_4[3];
LAB_100e370e:
    FUN_100e05ec(0xd7,0);
    return 0;
  }
  if (param_3 < 5) {
    return 0;
  }
  switch(param_3) {
  case 5:
    uVar21 = param_4[1];
    iVar11 = FUN_100c148c();
    piVar7 = DAT_100e356c;
    pcVar6 = DAT_100e3568;
    if (*DAT_100e3568 == '\x02') {
      uVar12 = 1;
    }
    else if ((uint)(iVar11 - *DAT_100e356c) < 0x1f) {
      uVar12 = 1;
    }
    else {
      uVar12 = 0;
    }
    if (uVar21 == 0) {
      return 0;
    }
    iVar11 = FUN_100c148c();
    uVar29 = FUN_100c148c();
    uVar12 = FUN_100ca284(uVar21,uVar29,uVar12);
    uVar18 = (DAT_100e355c - DAT_100e3558) * 0x20 & 0xff00;
    FUN_1014c818(DAT_100e3570 | uVar18,DAT_100e3574,uVar12);
    FUN_1014c818(DAT_100e3578 | uVar18,DAT_100e357c,*pcVar6);
    piVar8 = DAT_100e3580;
    bVar15 = (byte)uVar21;
    *(byte *)(iVar17 + 0x43bb) = bVar15;
    pbVar9 = DAT_100e3584;
    *(int *)(iVar17 + 0x68) = iVar11;
    *pbVar9 = bVar15;
    *piVar8 = *piVar8 + uVar21;
    *(short *)(piVar8 + 1) = (short)piVar8[1] + 1;
    pcVar10 = DAT_100e3588;
    if (*(byte *)((int)piVar8 + 6) < uVar21) {
      *(byte *)((int)piVar8 + 6) = bVar15;
      *pcVar10 = '\x01';
    }
    puVar13 = DAT_100e358c;
    if (uVar21 < *DAT_100e358c) {
      *DAT_100e358c = uVar21;
      *DAT_100e3588 = '\x02';
    }
    else if (*DAT_100e358c == 0) {
      *DAT_100e358c = uVar21;
    }
    if (*DAT_100e3590 != uVar21) {
      *DAT_100e3590 = (ushort)uVar21;
      FUN_100e05ec(0xd7,0);
    }
    FUN_100c14b4(&local_3c);
    uVar19 = (uint)local_3c._2_1_ * 0x3c + (uint)local_3c._1_1_;
    uVar18 = uVar19 / 5;
    if (0x11e < uVar18) {
      uVar18 = 0x11f;
    }
    uVar19 = uVar19 / 0x1e;
    bVar1 = *(byte *)(iVar17 + 0x43bb);
    if (0x2e < uVar19) {
      uVar19 = 0x2f;
    }
    if (*(byte *)(iVar17 + 0x43b8) < bVar1) {
      *(byte *)(iVar17 + 0x43b8) = bVar1;
LAB_100e34ea:
      if ((bVar1 < *(byte *)(iVar17 + 0x43b9)) || (*(byte *)(iVar17 + 0x43b9) == 0)) {
        *(byte *)(iVar17 + 0x43b9) = bVar1;
      }
      *(byte *)(DAT_100e3594 + uVar18) = bVar15;
      *(short *)(iVar17 + 0x43bc) = (short)uVar18 + 1;
      local_40 = uVar21;
      local_44 = FUN_100c148c();
      FUN_100c6114(3,&local_44,8);
    }
    else if ((bVar1 < *(byte *)(iVar17 + 0x43b9)) || (*(byte *)(iVar17 + 0x43b9) == 0))
    goto LAB_100e34ea;
    iVar14 = uVar19 + 0x90;
    if (*(byte *)(DAT_100e3594 + iVar14 * 2) < uVar21) {
      *(byte *)(DAT_100e3594 + iVar14 * 2) = bVar15;
      local_40 = uVar21;
      local_44 = FUN_100c148c();
      FUN_100c6114(3,&local_44,8);
    }
    uVar18 = (uint)*(byte *)(DAT_100e3598 + iVar14 * 2);
    if ((uVar21 < uVar18) || (uVar18 == 0)) {
      *(byte *)(DAT_100e3598 + iVar14 * 2) = bVar15;
      local_40 = uVar21;
      local_44 = FUN_100c148c();
      FUN_100c6114(3,&local_44,8);
    }
    if (((((*DAT_100e359c != '\0') && (iVar14 = FUN_100c9628(), iVar14 == 0)) &&
         (0x3c < (uint)(iVar11 - *piVar7))) && (uVar21 - 0x28 < 0x3d)) &&
       ((*(byte *)(iVar17 + 0x43ba) == 0 || (uVar21 < *(byte *)(iVar17 + 0x43ba))))) {
      *(byte *)(iVar17 + 0x43ba) = bVar15;
    }
    pbVar9 = DAT_100e35a4;
    piVar7 = DAT_100e35a0;
    if (*pcVar6 != '\x02') {
      return 0;
    }
    iVar17 = *DAT_100e35a0;
    if (*DAT_100e35a4 == 0) {
      *DAT_100e35a4 = bVar15;
    }
    pcVar6 = DAT_100e3588;
    if ((iVar11 - iVar17 & 0xffU) < 5) {
      return 0;
    }
    *piVar7 = iVar11;
    if (*pcVar6 == '\x01') {
      *pcVar6 = '\0';
      local_38 = (uint)*(byte *)((int)piVar8 + 6);
      puVar13 = DAT_100e35a8;
LAB_100e34ba:
      *puVar13 = local_38;
    }
    else {
      if (*pcVar6 == '\x02') {
        *pcVar6 = '\0';
        local_38 = *puVar13;
        puVar13 = DAT_100e35b0;
        goto LAB_100e34ba;
      }
      local_38 = (uint)*pbVar9;
    }
    *pbVar9 = 0;
    if (*DAT_100e35ac == '\0') {
      local_3c = iVar11;
      FUN_100c6114(8,&local_3c);
    }
    break;
  case 6:
    uVar30 = FUN_10045be8(param_4[5]);
    uVar12 = FUN_10045be8(param_4[6]);
    FUN_1004bd78(uVar12,param_2,(int)uVar30,(int)((ulonglong)uVar30 >> 0x20));
    break;
  case 7:
    uVar18 = param_4[2];
    uVar21 = (DAT_100e3778 - DAT_100e377c) * 0x20 & 0xff00;
    FUN_1014c818(DAT_100e37a0 | uVar21,DAT_100e37a4,uVar18,*DAT_100e379c);
    uVar12 = DAT_100e37ac;
    if (*pbVar9 == uVar18) {
      return 0;
    }
    uVar21 = DAT_100e37a8 | uVar21;
    *pbVar9 = (byte)uVar18;
    FUN_1014c818(uVar21,uVar12);
    goto LAB_100e370e;
  case 0xe:
    uVar12 = VectorFloatToUnsigned(param_4[0x22],3);
    iVar14 = param_4[0x1d];
    uVar21 = (DAT_100e31f4 - DAT_100e31f0) * 0x20 & 0xff00;
    FUN_1014c818(DAT_100e31f8 | uVar21,DAT_100e31fc,0xe,uVar12,*(undefined4 *)(iVar17 + 0x5324),
                 iVar14);
    iVar11 = FUN_100c85a8();
    if (iVar11 == 0) {
      iVar11 = VectorFloatToUnsigned(param_4[0x22],3);
      iVar11 = iVar11 + *(int *)(iVar17 + 0x5324);
      if ((*(int *)(iVar17 + 0x34e4) != iVar11) && ((param_4[0x1d] & 0xffffU) == 0x3e9)) {
        fVar28 = (float)VectorUnsignedToFloat
                                  (*(undefined4 *)(iVar17 + 0x532c),(byte)(in_fpscr >> 0x16) & 3);
        uVar12 = VectorFloatToUnsigned(fVar28 + (float)param_4[0x23],3);
        *(int *)(iVar17 + 0x34e4) = iVar11;
        *(undefined4 *)(iVar17 + 0x368c) = uVar12;
        fVar28 = (float)VectorUnsignedToFloat
                                  (*(undefined4 *)(iVar17 + 0x5328),(byte)(in_fpscr >> 0x16) & 3);
        uVar29 = VectorFloatToUnsigned(fVar28 + (float)param_4[0x24],3);
        *(undefined4 *)(iVar17 + 0x3688) = uVar29;
        FUN_1014c818(DAT_100e3220 | uVar21,DAT_100e3224,iVar11,uVar12,uVar29,iVar14);
        uVar12 = FUN_100c148c();
        *DAT_100e3210 = uVar12;
        FUN_100e05ec(0xd7,0);
      }
    }
    else if ((param_4[0x1d] & 0xffffU) == (uint)uVar3) {
      FUN_1014c818(DAT_100e3200 | uVar21,DAT_100e3204);
      FUN_100c70fc(param_4,DAT_100e320c,DAT_100e3208);
      FUN_100c85f0(param_4);
      uVar12 = FUN_100c148c();
      *DAT_100e3210 = uVar12;
    }
    puVar5 = DAT_100e3214;
    if (param_4[0x1d] != 0x66) {
      return 0;
    }
    uVar18 = param_4[0x20];
    if (3 < uVar18) {
      return 0;
    }
    *DAT_100e3214 = 0;
    puVar5[2] = 0;
    puVar5[1] = uVar18;
    *(undefined1 *)puVar5 = 5;
    thunk_FUN_100d5e34(0x118);
    uVar18 = DAT_100e321c;
    uVar12 = DAT_100e3218;
    iVar11 = param_4[0x20];
    goto LAB_100e2fec;
  case 0x10:
    FUN_1014c818(DAT_100e3560 | (DAT_100e355c - DAT_100e3558) * 0x20 & 0xff00U,DAT_100e3564,
                 DAT_100e353c,(char)param_4[0x1f]);
    break;
  case 0x16:
    uVar21 = (DAT_100e31f4 - DAT_100e31f0) * 0x20 & 0xff00;
    FUN_1014c818(DAT_100e3228 | uVar21,DAT_100e322c,DAT_100e31e8,param_4[0xb],param_4[0x1e]);
    if (0x14 < (uint)param_4[0x1e]) {
      sVar2 = *DAT_100e3230;
      *DAT_100e3230 = sVar2 + 1;
      uVar12 = FUN_100c148c();
      FUN_100ca784(sVar2 + 1,uVar12);
    }
    iVar11 = param_4[0x1e];
    if (iVar11 == 0x14) {
      if ((param_4[0xb] & 0xfffffffdU) != 1) {
        if (param_4[0xb] != 9) {
          return 0;
        }
        FUN_1014c818(DAT_100e3534 | uVar21,DAT_100e3538,DAT_100e353c,9,
                     *(undefined1 *)(iVar17 + 0x3694));
        return 0;
      }
      local_3c = 0;
      local_38 = 0;
      local_34 = 0;
      FUN_100c14b4(&local_3c);
      uVar22 = (local_3c >> 8 & 0xff) + (local_3c >> 0x10 & 0xff) * 0x3c;
      uVar27 = *(uint *)(iVar17 + 0x7fc);
      uVar16 = *(uint *)(iVar17 + 0x808) / 0x3c;
      uVar26 = *(uint *)(iVar17 + 0x800);
      uVar3 = *(ushort *)(iVar17 + 0x7fa);
      uVar25 = *(uint *)(iVar17 + 0x804) / 0x3c;
      FUN_1014c818(DAT_100e3234 | uVar21,DAT_100e3238,DAT_100e31e8,*(undefined1 *)(iVar17 + 0x5360),
                   *(undefined4 *)(iVar17 + 0x5364));
      uVar19 = uVar16 & 0xffff;
      uVar23 = (uint)(short)uVar22;
      uVar24 = uVar25 & 0xffff;
      uVar18 = uVar26;
      FUN_1014c818(DAT_100e323c | uVar21,DAT_100e3240,uVar23,uVar27,uVar26,uVar24,uVar19);
      if (uVar27 / 0x3c < uVar23) {
        if ((uVar23 < uVar26 / 0x3c) && (uVar27 < uVar26)) goto LAB_100e3162;
LAB_100e315e:
        if (uVar26 < uVar27) goto LAB_100e3162;
      }
      else if (uVar23 < uVar26 / 0x3c) goto LAB_100e315e;
      if (uVar27 != uVar26) {
        return 0;
      }
LAB_100e3162:
      uVar25 = uVar25 & 0xffff;
      if (((uVar25 <= uVar22) && (uVar16 = uVar16 & 0xffff, uVar22 <= uVar16)) && (uVar25 < uVar16))
      {
        *(undefined1 *)(iVar17 + 0x5360) = 0;
        *(undefined4 *)(iVar17 + 0x5364) = 0;
        return 0;
      }
      if (((*(char *)(iVar17 + 0x5360) == '\0') || (uVar23 <= *(int *)(iVar17 + 0x5364) + 0x3bU)) ||
         (*(int *)(iVar17 + 0x5364) + 0x3dU <= uVar23)) {
        *(undefined1 *)(iVar17 + 0x5360) = 1;
      }
      else {
        *(char *)(iVar17 + 0x5360) = *(char *)(iVar17 + 0x5360) + '\x01';
      }
      *(uint *)(iVar17 + 0x5364) = uVar23;
      FUN_1014c818(DAT_100e3244 | uVar21,DAT_100e3248,*(undefined1 *)(iVar17 + 0x5360),uVar3,uVar18,
                   uVar24,uVar19);
      if ((ushort)((ushort)*(byte *)(iVar17 + 0x5360) * 0x3c) < uVar3) {
        return 0;
      }
      thunk_FUN_100d5e34(0x102,0);
      *(undefined1 *)(iVar17 + 0x5360) = 0;
      *(undefined4 *)(iVar17 + 0x5364) = 0;
      return 0;
    }
    if (iVar11 != 0x28) {
      if (1 < iVar11 - 0x15U) {
        return 0;
      }
      FUN_1014c818(uVar21 | 0x4380033,DAT_100e3554,DAT_100e353c,param_4[0x10],param_4[0x18]);
      FUN_100ca9d0(param_4);
      return 0;
    }
    FUN_100c14b4(&local_3c);
    iVar14 = DAT_100e3540 + (uint)local_3c._2_1_ * 0x10;
    uVar18 = DAT_100e3550;
    uVar12 = DAT_100e354c;
    iVar11 = DAT_100e353c;
    if (*(char *)(iVar14 + 0x18) == '\0') {
      *(undefined1 *)(iVar14 + 0x18) = 1;
      uVar18 = DAT_100e3544;
      bVar15 = *(char *)(iVar17 + 0x3694) + 1;
      if (0x18 < bVar15) {
        bVar15 = 0x18;
      }
      *(byte *)(iVar17 + 0x3694) = bVar15;
      FUN_1014c818(uVar18 | uVar21,DAT_100e3548,DAT_100e353c,param_4[0xc],
                   *(undefined1 *)(iVar17 + 0x3694));
      uVar18 = DAT_100e3550;
      uVar12 = DAT_100e354c;
      iVar11 = DAT_100e353c;
    }
LAB_100e2fec:
    FUN_1014c818(uVar18 | uVar21,uVar12,iVar11);
  }
  return 0;
}


