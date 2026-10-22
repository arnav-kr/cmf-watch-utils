// entry 0x10186dd4  FUN_10186dd4  (10186dd4)

undefined4 FUN_10186dd4(int param_1,int *param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined1 auStack_38 [9];
  char local_2f;
  undefined4 local_28;
  ushort local_24;
  
  if ((param_1 == 0) || (param_2 == (int *)0x0)) {
    FUN_1014c818(DAT_10186f28 | (DAT_10186f20 - DAT_10186f24) * 0x20 & 0xff00U,DAT_10186f2c);
  }
  else {
    iVar1 = FUN_10185ec8(param_1,*(undefined4 *)(*param_2 + 0xc));
    param_2[1] = iVar1;
    if (iVar1 != 0) {
      return 0;
    }
    iVar1 = param_1 + 0x58;
    FUN_101072f0(iVar1,0,0);
    FUN_101bf3b2(auStack_38,0,0x10);
    uVar2 = FUN_10107238(iVar1,auStack_38,0x10);
    if ((uVar2 < 0x10) || (local_2f != '\x02')) {
      FUN_1014c818(DAT_10186f30 | (DAT_10186f20 - DAT_10186f24) * 0x20 & 0xff00U,DAT_10186f34);
    }
    else {
      FUN_101bf3b2(&local_28,0,0x10);
      FUN_101072f0(iVar1,*(int *)(*param_2 + 0xc) << 4,0);
      uVar2 = FUN_10107238(iVar1,&local_28,0x10);
      if (uVar2 < 0x10) {
        FUN_1014c818(DAT_10186f38 | (DAT_10186f20 - DAT_10186f24) * 0x20 & 0xff00U,DAT_10186f3c,
                     *(undefined4 *)(param_1 + 0x98),*(undefined4 *)(*param_2 + 0xc));
      }
      else {
        FUN_101072f0(iVar1,local_28,0);
        iVar3 = FUN_10186d40(param_1,*(undefined4 *)(*param_2 + 0xc),local_24 + 1);
        uVar2 = (uint)local_24;
        param_2[1] = iVar3;
        if (iVar3 == 0) {
          iVar1 = DAT_10186f20 - DAT_10186f24;
          uVar7 = *(undefined4 *)(*param_2 + 0xc);
          uVar6 = *(undefined4 *)(param_1 + 0x98);
          uVar4 = DAT_10186f44;
          uVar5 = DAT_10186f40;
        }
        else {
          FUN_101bf3b2(iVar3,0,uVar2 + 1);
          iVar1 = FUN_10107238(iVar1,param_2[1],local_24);
          uVar2 = (uint)local_24;
          if ((int)uVar2 <= iVar1) {
            return 0;
          }
          iVar1 = DAT_10186f20 - DAT_10186f24;
          uVar7 = *(undefined4 *)(*param_2 + 0xc);
          uVar6 = *(undefined4 *)(param_1 + 0x98);
          uVar4 = DAT_10186f4c;
          uVar5 = DAT_10186f48;
        }
        FUN_1014c818(uVar4 | iVar1 * 0x20 & 0xff00U,uVar5,uVar6,uVar7,local_28,uVar2);
      }
    }
  }
  return 0xffffffff;
}


