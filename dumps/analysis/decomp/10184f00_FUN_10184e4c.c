// entry 0x10184f00  FUN_10184e4c  (10184e4c)

int FUN_10184e4c(int param_1,int param_2,uint param_3,uint param_4,undefined4 *param_5)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  
  uVar5 = (DAT_10184f0c - DAT_10184f10) * 0x20 & 0xff00;
  if ((param_2 == 0) || (param_4 < param_3)) {
    FUN_1014c818(DAT_10184f14 | uVar5,DAT_10184f18,param_2,param_3,param_4);
    iVar6 = -1;
  }
  else {
    iVar6 = 0;
    uVar7 = uVar5 | DAT_10184f2c;
    for (; param_3 <= param_4; param_3 = param_3 + 1) {
      iVar1 = FUN_10186838(*(undefined4 *)(param_2 + 0x14),*(undefined4 *)(param_2 + 0x10),param_3);
      if (iVar1 == 0) {
        FUN_1014c818(uVar7,DAT_10184f1c,param_3);
      }
      else {
        iVar2 = FUN_1018433c(*param_5,iVar1);
        if (iVar2 == 0) {
          FUN_101d7876(iVar1);
        }
        else {
          puVar3 = (undefined4 *)FUN_101d780c(1,0x1c,DAT_10184f20);
          if (puVar3 == (undefined4 *)0x0) {
            FUN_1014c818(DAT_10184f24 | uVar5,DAT_10184f28);
            return iVar6;
          }
          if (param_1 == 0) {
            *puVar3 = 1;
          }
          else {
            puVar3[2] = param_1;
            *puVar3 = 2;
          }
          puVar3[6] = 0;
          uVar4 = *(undefined4 *)(param_2 + 0x14);
          puVar3[1] = iVar1;
          puVar3[5] = uVar4;
          iVar6 = iVar6 + 1;
          *(undefined4 **)(iVar2 + 0x18) = puVar3;
        }
      }
    }
  }
  return iVar6;
}


