// entry 0x100de874  FUN_100de874  (100de874)

int FUN_100de874(int param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int local_b0 [11];
  undefined1 auStack_84 [44];
  undefined4 local_58;
  undefined1 auStack_54 [60];
  
  FUN_101bf3b2(local_b0,0,0x2c);
  if (param_1 == -1) {
    iVar4 = 1;
  }
  else {
    iVar4 = 0;
    piVar2 = DAT_100de974;
    do {
      if (param_1 == *piVar2) {
        iVar4 = 0;
        goto LAB_100de8a0;
      }
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar4 != 6);
    iVar4 = 2;
  }
LAB_100de8a0:
  uVar5 = (DAT_100de97c - DAT_100de978) * 0x20 & 0xff00;
  FUN_1014c818(uVar5 | 0x2d0032,DAT_100de984,DAT_100de980,param_1);
  local_58 = 0;
  FUN_101bf3b2(auStack_54,0,0x3c);
  FUN_100de99c(param_1,&local_58,0x40,param_2);
  FUN_101bf3b2(local_b0,0,0x2c);
  iVar1 = FUN_10107030(local_b0,&local_58,1);
  if (iVar1 < 0) {
    FUN_1014c818(uVar5 | 0x360031,DAT_100de988,&local_58);
    iVar4 = -0x13;
  }
  else {
    FUN_100db4a4(auStack_84);
    FUN_100db4c0(local_b0);
    if (iVar4 == 1) {
      iVar4 = FUN_100dc158(&local_58);
    }
    else {
      iVar4 = FUN_101b8f72(0);
    }
    FUN_100db4c0(auStack_84);
    if (local_b0[0] != 0) {
      FUN_101071e0(local_b0);
      local_b0[0] = 0;
    }
    uVar3 = DAT_100de990;
    if (iVar4 == 0) {
      uVar3 = DAT_100de98c;
    }
    FUN_1014c818(uVar5 | 0x520032,DAT_100de994,uVar3,&local_58);
    FUN_1014c818(uVar5 | 0x530031,DAT_100de998,DAT_100de980);
  }
  return iVar4;
}


