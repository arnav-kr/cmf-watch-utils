// 0x10240ea2 lv_draw_vglite_line :: Update stroke failed. (err = %d)
// entry 0x10144eec  lv_draw_vglite_line

undefined4
lv_draw_vglite_line(undefined4 *param_1,undefined4 *param_2,undefined4 *param_3,ushort *param_4)

{
  byte bVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint in_fpscr;
  undefined4 uVar9;
  undefined4 uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  undefined4 uVar14;
  uint local_a4;
  undefined4 local_a0;
  float local_9c;
  float local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined1 auStack_78 [76];
  
  uVar3 = FUN_101455f0();
  bVar1 = *(byte *)((int)param_4 + 0x11);
  uVar7 = 0x1701;
  uVar8 = 0x1801;
  if ((bVar1 & 0xc) == 0) {
    uVar7 = 0x1700;
    uVar8 = 0x1800;
  }
  if (*(int *)(param_4 + 4) == 0) {
    bVar2 = false;
  }
  else {
    bVar2 = false;
    if (*(int *)(param_4 + 6) != 0) {
      bVar2 = true;
    }
  }
  local_9c = DAT_1014513c;
  local_98 = DAT_1014513c;
  if (bVar2) {
    local_9c = (float)VectorSignedToFloat(*(int *)(param_4 + 4),(byte)(in_fpscr >> 0x16) & 3);
    local_98 = (float)VectorSignedToFloat(*(undefined4 *)(param_4 + 6),(byte)(in_fpscr >> 0x16) & 3)
    ;
    uVar6 = 2;
    fVar13 = local_9c * 0.5;
  }
  else {
    uVar6 = 0;
    fVar13 = DAT_1014513c;
  }
  if (((byte)param_4[8] < 0xfd) || (local_a4 = bVar1 & 3, (bVar1 & 3) != 0)) {
    local_a4 = FUN_101cdf1e(bVar1 & 3);
  }
  fVar11 = (float)VectorSignedToFloat(param_3[3],(byte)(in_fpscr >> 0x16) & 3);
  fVar12 = (float)VectorSignedToFloat(param_3[2],(byte)(in_fpscr >> 0x16) & 3);
  local_90 = *param_1;
  local_8c = param_1[1];
  local_88 = 4;
  local_84 = *param_2;
  uVar10 = VectorSignedToFloat(param_3[1],(byte)(in_fpscr >> 0x16) & 3);
  local_80 = param_2[1];
  uVar14 = *(undefined4 *)(param_4 + 2);
  uVar9 = VectorSignedToFloat(*param_3,(byte)(in_fpscr >> 0x16) & 3);
  local_94 = 2;
  local_7c = 0;
  iVar4 = FUN_10192bf8(uVar9,uVar10,fVar12 + 1.0,fVar11 + 1.0,auStack_78,2,0,0x1c,&local_94);
  if (iVar4 == 0) {
    iVar4 = FUN_101cdec0(&local_a0,
                         ((int)((uint)(*(byte *)((int)param_4 + 1) >> 3) * 0x107 + 7) >> 5 & 0xffU)
                         << 0x10 | ((int)(((*param_4 & 0x7ff) >> 5) * 0x103 + 3) >> 6 & 0xffU) << 8
                         | (int)(((byte)*param_4 & 0x1f) * 0x107 + 7) >> 5 & 0xffU | 0xff000000,
                         (byte)param_4[8],0x400);
    if (iVar4 != 1) {
      return 0;
    }
    iVar4 = FUN_101da384(auStack_78,1);
    if (iVar4 == 0) {
      uVar9 = VectorSignedToFloat(uVar14,(byte)(in_fpscr >> 0x16) & 3);
      iVar4 = FUN_10196334(uVar9,0x41000000,fVar13,auStack_78,uVar7,uVar8,&local_9c,uVar6,local_a0);
      if (iVar4 != 0) {
        uVar3 = 0x74;
        uVar7 = DAT_10145150;
        goto LAB_10144fe4;
      }
      iVar4 = FUN_10194c60(auStack_78);
      if (iVar4 == 0) {
        FUN_10192aa4(*param_3,param_3[1],param_3[2] + 1,param_3[3] + 1);
        iVar4 = FUN_10192cf8(uVar3,auStack_78,0x1901,0,local_a4,local_a0);
        if (iVar4 == 0) {
          iVar4 = lv_vglite_utils(param_3);
          if (iVar4 != 1) {
            FUN_101da398(auStack_78);
            return 0;
          }
          puVar5 = (undefined4 *)FUN_101455f0();
          if (puVar5 != (undefined4 *)0x0) {
            FUN_10192aa4(0,0,*puVar5,puVar5[1]);
          }
          iVar4 = FUN_101da398(auStack_78);
          if (iVar4 == 0) {
            return 1;
          }
          uVar3 = 0x8c;
          uVar7 = DAT_1014515c;
          goto LAB_10144fe4;
        }
        FUN_101da398(auStack_78);
        uVar3 = 0x81;
        uVar7 = DAT_10145158;
      }
      else {
        FUN_101da398(auStack_78);
        uVar3 = 0x79;
        uVar7 = DAT_10145154;
      }
    }
    else {
      uVar3 = 0x70;
      uVar7 = DAT_1014514c;
    }
    FUN_1013a098(3,DAT_10145148,uVar3,DAT_10145144,uVar7,iVar4);
  }
  else {
    uVar3 = 0x67;
    uVar7 = DAT_10145140;
LAB_10144fe4:
    FUN_1013a098(3,DAT_10145148,uVar3,DAT_10145144,uVar7,iVar4);
  }
  return 0;
}


