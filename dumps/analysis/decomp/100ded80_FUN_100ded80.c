// entry 0x100ded80  FUN_100ded80

undefined4 FUN_100ded80(undefined4 *param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 local_12c [11];
  undefined4 local_100;
  undefined1 auStack_fc [60];
  int local_c0;
  char local_bb;
  undefined2 local_ba;
  undefined2 local_b8;
  undefined2 local_b6;
  undefined2 local_b4;
  
  local_100 = 0;
  FUN_101bf3b2(auStack_fc,0,0x3c);
  FUN_101bf3b2(local_12c,0,0x2c);
  FUN_101bf3b2(&local_c0,0,0xa6);
  uVar2 = (DAT_100deec8 - DAT_100deecc) * 0x20 & 0xff00;
  FUN_1014c818(uVar2 | 0xe50031,DAT_100deed4,DAT_100deed0);
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    *(undefined2 *)(param_1 + 2) = 0;
    FUN_100dea3c(0xffffffff,&local_100,0x40,0);
    FUN_1014c818(uVar2 | 0xf90031,DAT_100deed8,&local_100);
    iVar1 = FUN_10107030(local_12c,&local_100,1);
    if (-1 < iVar1) {
      FUN_101072f0(local_12c,0,2);
      iVar1 = FUN_10107350(local_12c);
      if (iVar1 == 0xa6) {
        FUN_101072f0(local_12c,0,0);
        FUN_10107238(local_12c,&local_c0,0xa6);
        FUN_1014c818(uVar2 | 0x10c0036,DAT_100deedc,local_c0,local_bb,local_ba,local_b8,local_b6,
                     local_b4);
        if ((local_c0 == DAT_100deee0) && (local_bb != '\0')) {
          *(undefined1 *)((int)param_1 + 1) = 0;
          *(char *)param_1 = (char)local_ba;
          *(undefined1 *)((int)param_1 + 2) = 0;
          *(undefined2 *)(param_1 + 1) = local_b8;
          *(undefined2 *)((int)param_1 + 6) = local_b6;
          *(undefined2 *)(param_1 + 2) = local_b4;
        }
        else {
          *(undefined2 *)(param_1 + 2) = 0xffff;
          *(undefined2 *)(param_1 + 1) = 0x60;
          *(undefined2 *)((int)param_1 + 6) = 0xb0;
        }
      }
      else {
        FUN_1014c818(DAT_100deeec | uVar2,DAT_100deef0,iVar1,0xa6);
      }
      FUN_101071e0(local_12c);
      local_12c[0] = 0;
      FUN_1014c818(DAT_100deee4 | uVar2,DAT_100deee8,DAT_100deed0);
      return 0;
    }
    FUN_1014c818(DAT_100deef4 | uVar2,DAT_100deef8);
  }
  return 0xffffffed;
}


