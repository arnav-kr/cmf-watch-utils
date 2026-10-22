// entry 0x10156948  FUN_10156948

undefined4 FUN_10156948(int *param_1,uint param_2,int param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int *piVar7;
  
  if ((param_1 == (int *)0x0) || (iVar2 = *param_1, iVar2 == 0)) {
LAB_1015698a:
    uVar3 = 0xffffffea;
  }
  else {
    if ((param_1[5] - 1U < 2) || (param_1[5] == 4)) {
      if ((param_2 & 0x1ff) != 0) {
        FUN_1014c818((DAT_10156ac8 - DAT_10156acc) * 0x20 & 0xff00U | 0x5c0011,DAT_10156ad0,param_2)
        ;
        goto LAB_1015698a;
      }
      uVar6 = param_4;
      if ((int)param_4 < 0) {
        uVar6 = param_4 + 0x1ff;
      }
      uVar4 = param_4 & 0x1ff;
      if (-1 < (int)-param_4) {
        uVar4 = -(param_4 * -0x800000 >> 0x17);
      }
      if ((int)param_2 < 0) {
        param_2 = param_2 + 0x1ff;
      }
      iVar1 = (int)uVar6 >> 9;
      iVar5 = (int)param_2 >> 9;
      if ((iVar1 != 0) &&
         (iVar2 = (**(code **)(*(int *)(iVar2 + 8) + 4))(iVar2,iVar5 << 9,param_3,iVar1 << 9),
         iVar2 != 0)) {
        FUN_1014c818((DAT_10156ac8 - DAT_10156acc) * 0x20 & 0xff00U | 0x670013,DAT_10156ad4,iVar2,
                     iVar5,iVar1);
        return 0xfffffffb;
      }
      if (uVar4 != 0) {
        uVar6 = (DAT_10156ac8 - DAT_10156acc) * 0x20 & 0xff00;
        FUN_1014c818(uVar6 | 0x6d0031,DAT_10156ad8,uVar4);
        iVar2 = thunk_FUN_10146a3c(0x200,DAT_10156adc);
        if (iVar2 != 0) {
          FUN_101bf3b2(iVar2,0,0x200);
          FUN_101bf3aa(iVar2,param_3 + iVar1 * 0x200,uVar4);
          uVar3 = (**(code **)(*(int *)(*param_1 + 8) + 4))
                            (*param_1,(iVar5 + iVar1) * 0x200,iVar2,0x200);
          FUN_101ce04a(iVar2);
          return uVar3;
        }
        FUN_1014c818(uVar6 | 0x700011,DAT_10156ae0);
        return 0xfffffff4;
      }
    }
    else {
      uVar6 = param_1[2];
      piVar7 = param_1;
      uVar4 = param_2;
      for (; 0 < (int)param_4; param_4 = param_4 - uVar6) {
        if ((int)param_4 < param_1[2]) {
          uVar6 = param_4;
        }
        iVar2 = (**(code **)(*(int *)(*param_1 + 8) + 4))
                          (*param_1,param_2,param_3,uVar6,piVar7,uVar4);
        if (iVar2 < 0) {
          FUN_1014c818((DAT_10156ac8 - DAT_10156acc) * 0x20 & 0xff00U | 0x940014,DAT_10156ae4,iVar2,
                       param_2,param_3,param_4);
          return 0xfffffffb;
        }
        param_2 = param_2 + uVar6;
        param_3 = param_3 + uVar6;
      }
    }
    uVar3 = 0;
  }
  return uVar3;
}


