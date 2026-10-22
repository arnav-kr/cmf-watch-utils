// entry 0x100eb12c  FUN_100eb12c

int FUN_100eb12c(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 local_98;
  undefined1 auStack_94 [60];
  undefined4 local_58;
  undefined1 auStack_54 [64];
  
  local_98 = 0;
  FUN_101bf3b2(auStack_94,0,0x3c);
  local_58 = 0;
  FUN_101bf3b2(auStack_54,0,0x3c);
  iVar3 = *DAT_100eb1e4;
  uVar2 = (DAT_100eb1e8 - DAT_100eb1ec) * 0x20 & 0xff00;
  FUN_1014c818(DAT_100eb1f0 | uVar2,DAT_100eb1f8,DAT_100eb1f4,param_1);
  FUN_100eae74(*(undefined4 *)(iVar3 + 0x48));
  *(char *)(DAT_100eb1fc + 0xb) = (char)param_1;
  iVar1 = FUN_100eb074(1,0xa060);
  if (iVar1 != 0) {
    FUN_1014c818(DAT_100eb200 | uVar2,DAT_100eb204,0xa060,iVar1);
  }
  FUN_101071e0(iVar3 + 0x1c);
  *(undefined4 *)(iVar3 + 0x1c) = 0;
  *(undefined1 *)(iVar3 + 0x12) = 0;
  if (param_1 == 1) {
    FUN_101aa780(&local_98,0x40,DAT_100eb20c,DAT_100eb208);
    FUN_101aa780(&local_58,0x40,DAT_100eb210,DAT_100eb208);
    FUN_10107704(&local_98,&local_58);
    if (*DAT_100eb214 == '\x01') {
      FUN_100eae44(0xd8,0,0);
    }
  }
  FUN_101bacba(0);
  return iVar1;
}


