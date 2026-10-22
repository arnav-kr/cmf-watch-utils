// entry 0x100deb34  FUN_100deb34

undefined4 FUN_100deb34(int param_1,undefined4 param_2,undefined4 param_3)

{
  int *piVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  uVar3 = ((int)PTR_DAT_100deba8 - (int)PTR_LAB_100debac) * 0x20 & 0xff00;
  FUN_1014c818(uVar3 | 0x350031,PTR_s__s_enter_100debb4,PTR_s_WfmPara_Load_100debb0,PTR_LAB_100debac
               ,param_1,param_2,param_3);
  piVar1 = DAT_100debb8;
  if (param_1 == 0) {
    uVar2 = 0xffffffed;
  }
  else {
    if ((*DAT_100debb8 == DAT_100debbc) &&
       (iVar5 = DAT_100debb8[0x1f], iVar4 = FUN_101b9036(DAT_100debb8,0x7c), iVar5 == iVar4)) {
      FUN_101bf3aa(param_1,piVar1,0x80);
      iVar4 = 1;
    }
    else {
      iVar4 = 0;
    }
    FUN_1014c818(uVar3 | 0x470033,DAT_100debc0,PTR_s_WfmPara_Load_100debb0,iVar4,
                 *(undefined1 *)(param_1 + 4),param_2,param_3);
    if (iVar4 == 0) {
      uVar2 = 0xfffffffd;
    }
    else {
      uVar2 = 0;
    }
  }
  return uVar2;
}


