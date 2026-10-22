// entry 0x100dd17c  FUN_100dd17c

undefined4 FUN_100dd17c(undefined4 param_1,int param_2,int param_3)

{
  byte *pbVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  byte bVar6;
  uint uVar7;
  uint uVar8;
  uint local_60;
  undefined1 auStack_5c [60];
  
  pbVar1 = DAT_100dd350;
  uVar8 = (DAT_100dd34c - DAT_100dd348) * 0x20 & 0xff00;
  uVar5 = (*DAT_100dd350 & 0x1f) >> 4;
  FUN_1014c818(uVar8 | 0x1d40034,DAT_100dd358,DAT_100dd354,param_2,(*DAT_100dd350 & 0xf) >> 3,uVar5)
  ;
  bVar6 = *pbVar1;
  *pbVar1 = bVar6 & 0xdf;
  if ((bVar6 & 0x18) == 0) {
    FUN_101bf3b2(DAT_100dd35c,bVar6 & 0x18,0x54);
    if (-1 < param_2) {
      bVar6 = pbVar1[8];
      uVar7 = (uint)bVar6;
      if (param_2 < (int)uVar7) {
        if (uVar7 != 0) {
          bVar6 = (char)param_2 - bVar6 * (char)(param_2 / (int)uVar7);
        }
        uVar2 = *(undefined4 *)(pbVar1 + param_2 * 0x10 + 0x10);
        pbVar1[9] = bVar6;
        bVar6 = pbVar1[param_2 * 0x10 + 0x14];
        *(undefined4 *)(pbVar1 + 0xc) = uVar2;
        pbVar1[10] = bVar6;
        bVar6 = FUN_100dc534();
        pbVar1[0xb] = bVar6;
        FUN_100dd16c();
      }
    }
    *(undefined4 *)(DAT_100dd35c + 0x50) = *(undefined4 *)(pbVar1 + 0xc);
  }
  bVar6 = pbVar1[0xb];
  FUN_1014c818(DAT_100dd360 | uVar8,DAT_100dd364,pbVar1[9],*(undefined4 *)(pbVar1 + 0xc),bVar6,uVar5
              );
  iVar3 = DAT_100dd35c;
  if (bVar6 == 1) {
    iVar3 = FUN_1009a0a4(DAT_100dd35c,param_1,pbVar1 + (uint)pbVar1[9] * 0x10 + 0x16);
    if (iVar3 == 0) {
      FUN_100dc5e4(1000);
      return 0;
    }
  }
  else {
    uVar5 = *pbVar1 & 0x18;
    if ((*pbVar1 & 0x18) != 0) {
LAB_100dd2c2:
      uVar2 = FUN_10127cfc(param_1);
      lv_obj_pos_29(uVar2,0x1d2);
      FUN_101c5de4(uVar2,9,0);
      lv_obj_23(uVar2,0x4000);
      if (param_3 == 0) {
        *(undefined4 *)(iVar3 + 0x44) = uVar2;
      }
      else {
        *(undefined4 *)(iVar3 + 0x48) = uVar2;
      }
      if ((*pbVar1 & 0x18) == 0) {
        *(byte *)(iVar3 + 0x4c) = *(byte *)(iVar3 + 0x4c) | 1;
      }
      if (param_3 == 0) {
        bVar6 = *pbVar1 | 8;
      }
      else {
        bVar6 = *pbVar1 | 0x10;
      }
      *pbVar1 = bVar6;
      FUN_1014c818(uVar8 | 0x2280031,DAT_100dd370,DAT_100dd354);
      return 0;
    }
    local_60 = uVar5;
    FUN_101bf3b2(auStack_5c,uVar5,0x3c);
    uVar2 = *(undefined4 *)(pbVar1 + 0xc);
    FUN_100de99c(uVar2,&local_60,0x40,uVar5);
    FUN_1014c818(uVar8 | 0x1fe0031,DAT_100dd368,&local_60);
    FUN_101bf3b2(iVar3 + 4,uVar5,0x2c);
    iVar4 = FUN_10107030(iVar3 + 4,&local_60,1);
    if (-1 < iVar4) {
      FUN_101b8ff6(iVar3 + 0x3c);
      FUN_101b8ff6(iVar3 + 0x40);
      iVar4 = FUN_100dc3c0(iVar3 + 4,iVar3 + 0x30);
      if (iVar4 == 0) {
        FUN_1014c818(uVar8 | 0x2140032,DAT_100dd36c,*(undefined1 *)(*(int *)(iVar3 + 0x30) + 0x17),
                     uVar2);
        goto LAB_100dd2c2;
      }
    }
    FUN_100dc82c(param_3);
    FUN_1014c818(DAT_100dd374 | uVar8,DAT_100dd378,DAT_100dd354);
  }
  return 0xffffffed;
}


