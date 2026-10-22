// entry 0x10156094  FUN_10156094  (10156094)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10156094(int param_1,int param_2,uint *param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  code *pcVar3;
  uint uVar4;
  
  uVar4 = ((int)PTR_DAT_10156138 - (int)PTR_LAB_10156134) * 0x20 & 0xff00;
  FUN_1014c818(uVar4 | 0x4e80032,PTR_s_cmd_0x_x__param__d_1015613c,param_2,param_3,param_4);
  switch(param_2) {
  case 0x10001:
    iVar1 = FUN_1015600c(param_1 + 0xc,param_3);
    if (iVar1 == 0) {
      return 0;
    }
    FUN_1014c818(_LAB_10156140 | uVar4,_LAB_10156144,0x10001);
    return 0xfffffffb;
  case 0x10002:
    uVar2 = 2;
    pcVar3 = *(code **)(param_1 + 8);
    *(char *)(param_1 + 0x30) = (char)param_3;
    goto LAB_101560fa;
  case 0x10003:
    uVar4 = (uint)*(ushort *)(param_1 + 0x2e);
    break;
  case 0x10004:
    if (0xe6 < *(ushort *)(param_1 + 0x2e)) {
      return 0;
    }
    uVar4 = (uint)*(ushort *)(param_1 + 0x2e) * 0x32;
    break;
  case 0x10005:
    uVar4 = (uint)*(byte *)(param_1 + 0x48);
    break;
  case 0x10006:
    param_3 = (uint *)0x0;
    uVar2 = 3;
    pcVar3 = *(code **)(param_1 + 8);
LAB_101560fa:
    (*pcVar3)(param_1,uVar2,param_3);
    return 0;
  default:
    FUN_1014c818(_LAB_10156148 | uVar4,_LAB_1015614c,param_2,param_2 + -0x10001,param_4);
    return 0xffffffea;
  }
  *param_3 = uVar4;
  return 0;
}


