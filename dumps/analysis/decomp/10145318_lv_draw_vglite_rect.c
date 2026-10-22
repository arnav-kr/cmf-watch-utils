// 0x10240f68 lv_draw_vglite_rect :: Update gradient failed. (err = %d)
// entry 0x10145318  lv_draw_vglite_rect

undefined4 lv_draw_vglite_rect(int *param_1,undefined4 *param_2,int *param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint in_fpscr;
  undefined4 uVar9;
  undefined4 uVar10;
  float fVar11;
  float fVar12;
  undefined4 local_2c4;
  undefined4 local_2c0;
  undefined1 auStack_2bc [4];
  undefined1 auStack_2b8 [4];
  uint local_2b4;
  uint local_2b0;
  undefined1 auStack_2a4 [36];
  undefined1 auStack_280 [72];
  undefined1 auStack_238 [176];
  undefined1 auStack_188 [348];
  
  iVar5 = param_1[2];
  iVar3 = *param_3;
  iVar6 = *param_1;
  iVar7 = param_1[3];
  iVar8 = param_1[1];
  local_2c4 = 0;
  uVar1 = FUN_101455f0();
  if (-1 < *param_3) {
    uVar2 = FUN_10145160(auStack_238,&local_2c0,iVar3,param_1);
    fVar12 = (float)VectorSignedToFloat(param_2[3],(byte)(in_fpscr >> 0x16) & 3);
    fVar11 = (float)VectorSignedToFloat(param_2[2],(byte)(in_fpscr >> 0x16) & 3);
    uVar10 = VectorSignedToFloat(param_2[1],(byte)(in_fpscr >> 0x16) & 3);
    uVar9 = VectorSignedToFloat(*param_2,(byte)(in_fpscr >> 0x16) & 3);
    if (*param_3 < 1) {
      uVar4 = 3;
    }
    else {
      uVar4 = 0;
    }
    iVar3 = FUN_10192bf8(uVar9,uVar10,fVar11 + 1.0,fVar12 + 1.0,auStack_280,2,uVar4,local_2c0,uVar2)
    ;
    if (iVar3 == 0) {
      FUN_101da30c(auStack_2a4);
      if ((*(byte *)((int)param_3 + 0x11) & 7) == 0) {
        uVar2 = FUN_101cdd60(*(undefined2 *)((int)param_3 + 6));
        iVar3 = FUN_101cdec0(&local_2c4,uVar2,*(undefined1 *)((int)param_3 + 5),0x400);
        if (iVar3 != 1) {
          return 0;
        }
      }
      else {
        uVar2 = FUN_101cdd60((short)param_3[2]);
        local_2b4 = (uint)*(byte *)((int)param_3 + 10);
        iVar3 = FUN_101cdec0(auStack_2bc,uVar2,*(undefined1 *)((int)param_3 + 5),0x401);
        if (iVar3 != 1) {
          return 0;
        }
        uVar2 = FUN_101cdd60((short)param_3[3]);
        local_2b0 = (uint)*(byte *)((int)param_3 + 0xe);
        iVar3 = FUN_101cdec0(auStack_2b8,uVar2,*(undefined1 *)((int)param_3 + 5),0x401);
        if (iVar3 != 1) {
          return 0;
        }
        FUN_101bf3b2(auStack_188,0);
        iVar3 = FUN_101da0d4(auStack_188);
        if (iVar3 != 0) {
          uVar2 = 0x92;
          uVar1 = DAT_101455d0;
          goto LAB_1014542e;
        }
        iVar3 = FUN_101da0fa(auStack_188,2,auStack_2bc,&local_2b4);
        if (iVar3 != 0) {
          uVar2 = 0x95;
          uVar1 = DAT_101455d4;
          goto LAB_1014542e;
        }
        iVar3 = FUN_101da14e(auStack_188);
        if (iVar3 != 0) {
          uVar2 = 0x98;
          uVar1 = DAT_101455d8;
          goto LAB_1014542e;
        }
        uVar2 = FUN_101da29e(auStack_188);
        FUN_101da30c();
        uVar10 = VectorSignedToFloat(param_1[1],(byte)(in_fpscr >> 0x16) & 3);
        uVar9 = VectorSignedToFloat(*param_1,(byte)(in_fpscr >> 0x16) & 3);
        FUN_101da328(uVar9,uVar10,uVar2);
        if ((*(byte *)((int)param_3 + 0x11) & 7) == 1) {
          fVar11 = (float)VectorSignedToFloat((iVar7 + 1) - iVar8,(byte)(in_fpscr >> 0x16) & 3);
          FUN_101da356(0x3f800000,fVar11 * DAT_101455dc,uVar2);
          FUN_10192b88(DAT_101455e0,uVar2);
        }
        else {
          fVar11 = (float)VectorSignedToFloat((iVar5 + 1) - iVar6,(byte)(in_fpscr >> 0x16) & 3);
          FUN_101da356(fVar11 * DAT_101455dc,0x3f800000,uVar2);
        }
      }
      if ((*(byte *)((int)param_3 + 5) < 0xfd) || (uVar2 = 0, (char)param_3[1] != '\0')) {
        uVar2 = FUN_101cdf1e();
      }
      FUN_10192aa4(*param_2,param_2[1],param_2[2] + 1,param_2[3] + 1);
      if ((*(byte *)((int)param_3 + 0x11) & 7) == 0) {
        iVar3 = FUN_10192cf8(uVar1,auStack_280,0x1900,auStack_2a4,uVar2,local_2c4);
      }
      else {
        FUN_10192b2c();
        iVar3 = FUN_101da452(uVar1,auStack_280,0x1900,auStack_2a4,auStack_188,uVar2);
        if (iVar3 != 0) {
          FUN_101da28a(auStack_188);
        }
        FUN_10192b5c();
      }
      if (iVar3 != 0) {
        uVar2 = 0xc4;
        uVar1 = DAT_101455e4;
LAB_1014542e:
        FUN_1013a098(3,DAT_101455cc,uVar2,DAT_101455c8,uVar1,iVar3);
        return 0;
      }
      iVar3 = lv_vglite_utils(param_2);
      if (iVar3 != 1) {
        return 0;
      }
      FUN_101cdd9c();
      iVar3 = FUN_101da398(auStack_280);
      if (iVar3 == 0) {
        if ((*(byte *)((int)param_3 + 0x11) & 7) == 0) {
          return 1;
        }
        iVar3 = FUN_101da28a(auStack_188);
        if (iVar3 != 0) {
          FUN_1013a098(3,DAT_101455cc,0xd0,DAT_101455c8,DAT_101455ec,iVar3);
          return 0;
        }
        return 1;
      }
      uVar2 = 0xcc;
      uVar1 = DAT_101455e8;
    }
    else {
      uVar2 = 0x76;
      uVar1 = DAT_101455c4;
    }
    FUN_1013a098(3,DAT_101455cc,uVar2,DAT_101455c8,uVar1,iVar3);
  }
  return 0;
}


