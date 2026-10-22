// 0x10223bb1 WFaceRes :: WEST_TOPDIR/application/bt_watch/src/jx_src/ui/watch_face/WFaceRes.c
// entry 0x101b8ffa  WFaceRes

undefined4 * WFaceRes(int param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  byte bVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  int *piVar9;
  uint uVar10;
  undefined8 uVar11;
  undefined4 local_28;
  uint local_24 [2];
  
  local_28 = 0;
  local_24[0] = 0;
  puVar1 = (undefined4 *)*param_2;
  do {
    puVar3 = puVar1;
    if (puVar3 == (undefined4 *)0x0) {
      puVar3 = (undefined4 *)FUN_10188234(2,0x3e,DAT_100dba80);
      if (puVar3 == (undefined4 *)0x0) {
        return (undefined4 *)0x0;
      }
      FUN_101bf3b2(puVar3,0,0x3e);
      puVar3[1] = param_1;
      FUN_101b90e2(param_2,puVar3);
      uVar11 = FUN_100db43c(param_1,local_24,4);
      if ((int)uVar11 != 4) {
        return (undefined4 *)0x0;
      }
      bVar2 = FUN_101b8fd2(local_24[0] & 0x1f,(int)((ulonglong)uVar11 >> 0x20),4);
      *(byte *)(puVar3 + 6) = *(byte *)(puVar3 + 6) & 0xe0 | bVar2 & 0x1f;
      *(byte *)((int)puVar3 + 0x19) =
           *(byte *)((int)puVar3 + 0x19) & 3 | (byte)(((local_24[0] & 0xffff) >> 10) << 2);
      bVar2 = (byte)((local_24[0] << 0xb) >> 0x1b);
      *(byte *)((int)puVar3 + 0x1a) = *(byte *)((int)puVar3 + 0x1a) & 0xe0 | bVar2;
      *(char *)((int)puVar3 + 0x1b) = (char)(local_24[0] >> 0x18);
      *(byte *)((int)puVar3 + 0x1a) = bVar2 | (byte)(local_24[0] >> 0x10) & 0xe0;
      iVar4 = FUN_100db43c(param_1 + 4,&local_28);
      if (iVar4 != 4) {
        return (undefined4 *)0x0;
      }
      puVar3[2] = local_28;
      goto LAB_100dbabe;
    }
    puVar1 = (undefined4 *)*puVar3;
  } while (param_1 != puVar3[1]);
  if (puVar3[4] != 0) goto LAB_100dba72;
  local_28 = puVar3[2];
LAB_100dbabe:
  piVar9 = (int *)ui_mem(2,0x40,puVar3[2],DAT_100dbaec);
  if (piVar9 == (int *)0x0) {
    FUN_1014c818(DAT_100dbaf8 | (DAT_100dbaf4 - DAT_100dbaf0) * 0x20 & 0xff00U,DAT_100dbafc);
    return (undefined4 *)0x0;
  }
  iVar4 = FUN_100db43c(param_1 + 8,piVar9,puVar3[2]);
  if (puVar3[2] != iVar4) {
    FUN_1014c818((DAT_100dba88 - DAT_100dba84) * 0x20 & 0xff00U | 0x1780032,DAT_100dba8c,param_1 + 8
                );
    goto LAB_100db838;
  }
  bVar2 = *(byte *)(puVar3 + 6) & 0x1f;
  if (bVar2 == 1) {
    *(byte *)(puVar3 + 6) = *(byte *)(puVar3 + 6) & 0xe0 | 4;
  }
  uVar5 = FUN_101c8a3a((*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 |
                       (uint)(*(byte *)((int)puVar3 + 0x19) >> 2),
                       (uint)*(byte *)((int)puVar3 + 0x1b) << 3 |
                       (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5),*(byte *)(puVar3 + 6) & 0x1f);
  puVar3[3] = uVar5;
  if ((((*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 | (uint)(*(byte *)((int)puVar3 + 0x19) >> 2)) ==
       0x1d2) &&
     (((uint)*(byte *)((int)puVar3 + 0x1b) << 3 | (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5)) ==
      0x1d2)) {
    uVar6 = 0;
  }
  else {
    uVar6 = 2;
  }
  uVar5 = ui_mem(uVar6,0x40,uVar5,DAT_100dba80);
  puVar3[4] = uVar5;
  if (puVar3[4] == 0) {
    iVar4 = puVar3[3];
    uVar7 = (DAT_100dba88 - DAT_100dba84) * 0x20 & 0xff00U | 0x18e0000;
    uVar5 = DAT_100dba90;
LAB_100db8f8:
    uVar7 = uVar7 | 0x31;
  }
  else {
    uVar7 = (*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 | (uint)(*(byte *)((int)puVar3 + 0x19) >> 2)
    ;
    if ((400 < uVar7) &&
       (uVar10 = (uint)*(byte *)((int)puVar3 + 0x1b) << 3 |
                 (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5), 400 < uVar10)) {
      FUN_1014c818(DAT_100dba94 | (DAT_100dba88 - DAT_100dba84) * 0x20 & 0xff00U,DAT_100dba98,uVar7,
                   uVar10,*(byte *)(puVar3 + 6) & 0x1f,puVar3[4]);
    }
    if (bVar2 == 1) {
      if ((*piVar9 != DAT_100dba9c) &&
         ((((*(char *)((int)piVar9 + 6) != 'J' || (*(char *)((int)piVar9 + 7) != 'F')) ||
           ((char)piVar9[2] != 'I')) || (*(char *)((int)piVar9 + 9) != 'F')))) {
        FUN_1013a098(3,DAT_100dbaa4,0x19f,DAT_100dba80,DAT_100dbaa0,*piVar9);
        goto LAB_100db838;
      }
      uVar7 = (*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 |
              (uint)(*(byte *)((int)puVar3 + 0x19) >> 2);
      iVar4 = FUN_100ffcc4(piVar9,local_28,puVar3[4],1,uVar7,0,0,uVar7,
                           (uint)*(byte *)((int)puVar3 + 0x1b) << 3 |
                           (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5));
      if (iVar4 == ((*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 |
                   (uint)(*(byte *)((int)puVar3 + 0x19) >> 2)) *
                   ((uint)*(byte *)((int)puVar3 + 0x1b) << 3 |
                   (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5)) * 2) goto LAB_100dba62;
      uVar7 = (DAT_100dba88 - DAT_100dba84) * 0x20 & 0xff00U | 0x1a80000;
      uVar5 = DAT_100dbaa8;
      goto LAB_100db8f8;
    }
    iVar4 = lz4(piVar9,puVar3[4],local_28,puVar3[3]);
    uVar10 = (*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 |
             (uint)(*(byte *)((int)puVar3 + 0x19) >> 2);
    bVar2 = *(byte *)(puVar3 + 6) & 0x1f;
    uVar7 = (uint)*(byte *)((int)puVar3 + 0x1b) << 3 | (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5);
    iVar8 = FUN_101c8a3a(uVar10,uVar7,bVar2);
    if (bVar2 == 0xb) {
      iVar8 = (int)(uVar7 * uVar10) >> 3;
    }
    else if (bVar2 == 0xc) {
      iVar8 = (int)(uVar7 * uVar10) >> 2;
    }
    else if (bVar2 == 0xd) {
      iVar8 = (int)(uVar7 * uVar10) >> 1;
    }
    if (iVar4 == iVar8) {
LAB_100dba62:
      FUN_101bee00(puVar3[4],puVar3[3]);
      FUN_10188380(2,piVar9);
LAB_100dba72:
      puVar3[5] = puVar3[5] + 1;
      return puVar3;
    }
    uVar7 = DAT_100dbaac | (DAT_100dba88 - DAT_100dba84) * 0x20 & 0xff00U;
    uVar5 = DAT_100dbab0;
  }
  FUN_1014c818(uVar7,uVar5,iVar4);
LAB_100db838:
  FUN_10188380(2,piVar9);
  if (puVar3[4] != 0) {
    if ((((*(byte *)((int)puVar3 + 0x1a) & 0x1f) << 6 | (uint)(*(byte *)((int)puVar3 + 0x19) >> 2))
         == 0x1d2) &&
       (((uint)*(byte *)((int)puVar3 + 0x1b) << 3 | (uint)(*(byte *)((int)puVar3 + 0x1a) >> 5)) ==
        0x1d2)) {
      uVar5 = 0;
    }
    else {
      uVar5 = 2;
    }
    FUN_10188380(uVar5);
    *(undefined1 *)(puVar3 + 4) = 0;
    *(undefined1 *)((int)puVar3 + 0x11) = 0;
    *(undefined1 *)((int)puVar3 + 0x12) = 0;
    *(undefined1 *)((int)puVar3 + 0x13) = 0;
  }
  return (undefined4 *)0x0;
}


