// entry 0x100eb676  FUN_100eb50c  (100eb50c)

int FUN_100eb50c(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_1c;
  
  iVar2 = DAT_100eb6e8;
  iVar5 = *DAT_100eb6b8;
  iVar6 = *(ushort *)(param_1 + 1) - 4;
  local_1c = param_4;
  if (*(char *)(iVar5 + 0x12) != '\x03') {
    FUN_1014c818(DAT_100eb6c4 | (DAT_100eb6c0 - DAT_100eb6bc) * 0x20 & 0xff00U,DAT_100eb6c8);
    FUN_100eb12c(10);
    return 0x10;
  }
  iVar4 = *(int *)(iVar5 + 0xc);
  if ((uint)(iVar4 + iVar6) < 0xc01) {
    iVar4 = FUN_100eaf48(DAT_100eb6e8 + iVar4,iVar6,iVar4,iVar4 + iVar6,param_1,param_2,param_3);
    if (iVar4 == 0) {
      iVar4 = FUN_100eaf48(&local_1c,4);
      if (iVar4 == 0) {
        if ((ushort)(*(byte *)(iVar5 + 0x10) + 1) == *(short *)(param_1 + 7)) {
          iVar2 = FUN_10105fc8(0,*(int *)(iVar5 + 0xc) + iVar2,iVar6);
          if (local_1c == iVar2) {
            *(undefined1 *)(iVar5 + 0x11) = 0;
            *(char *)(iVar5 + 0x10) = (char)*(undefined2 *)(param_1 + 7);
            iVar6 = *(int *)(iVar5 + 0xc) + iVar6;
            *(int *)(iVar5 + 0xc) = iVar6;
            if ((iVar6 != 0xc00) && (*(int *)(iVar5 + 8) + iVar6 != *(int *)(DAT_100eb6e4 + 1))) {
              FUN_101be22e(iVar5 + 0x50,3000);
              return 0;
            }
            iVar4 = FUN_100eac84(iVar5 + 0x1c,*(int *)(iVar5 + 8),DAT_100eb6e8);
            iVar6 = DAT_100eb6c0;
            iVar2 = DAT_100eb6bc;
            if (iVar4 != 0) {
              return iVar4;
            }
            *(int *)(iVar5 + 8) = *(int *)(iVar5 + 8) + *(int *)(iVar5 + 0xc);
            FUN_1014c818((iVar6 - iVar2) * 0x20 & 0xff00U | 0x3380031,DAT_100eb6ec);
                    /* WARNING: Subroutine does not return */
            FUN_101dafe0(iVar5 + 0x50);
          }
          FUN_1014c818((DAT_100eb6c0 - DAT_100eb6bc) * 0x20 & 0xff00U | 0x3280013,DAT_100eb6e0,
                       *(undefined2 *)(DAT_100eb6dc + 7),local_1c,iVar2);
        }
        goto LAB_100eb56e;
      }
      uVar1 = (DAT_100eb6c0 - DAT_100eb6bc) * 0x20 & 0xff00U | 0x31c0011;
      uVar3 = DAT_100eb6d8;
    }
    else {
      uVar1 = DAT_100eb6d4 | (DAT_100eb6c0 - DAT_100eb6bc) * 0x20 & 0xff00U;
      uVar3 = DAT_100eb6d0;
    }
    FUN_1014c818(uVar1,uVar3,iVar4);
  }
  else {
    FUN_1014c818((DAT_100eb6c0 - DAT_100eb6bc) * 0x20 & 0xff00U | 0x3100012,DAT_100eb6cc,iVar4,iVar6
                );
  }
LAB_100eb56e:
                    /* WARNING: Subroutine does not return */
  FUN_101dafe0(iVar5 + 0x50);
}


