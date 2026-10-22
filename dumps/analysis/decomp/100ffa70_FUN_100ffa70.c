// entry 0x100ffa70  FUN_100ffa70  (100ffa70)

undefined4
FUN_100ffa70(undefined4 *param_1,int *param_2,undefined4 param_3,int param_4,undefined4 param_5,
            int param_6,int param_7,int param_8,undefined4 param_9,int param_10)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  short sVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  ushort *puVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  uint local_90;
  short local_78;
  short local_76;
  short local_74;
  short local_72;
  short local_70;
  short local_6e;
  short local_6c;
  short local_6a;
  short local_68;
  short local_66;
  short local_64;
  short local_62;
  ushort local_60 [26];
  
  if (param_2 + -0xd000000 < (int *)0x8000000) {
    FUN_101bee52(param_2,param_3);
    FUN_101beeae();
  }
  if (param_4 + 0xcc000000U < 0x8000000) {
    FUN_101bee52(param_4,param_6 * param_10 * 2);
    FUN_101beeae();
  }
  if (*param_2 == DAT_100ffc80) {
    uVar6 = (int)((uint)*(ushort *)((int)param_2 + 6) + (uint)*(ushort *)((int)param_2 + 10) + -1) /
            (int)(uint)*(ushort *)((int)param_2 + 10) & 0xff;
    uVar7 = (int)((uint)*(ushort *)(param_2 + 1) + (uint)*(ushort *)(param_2 + 2) + -1) /
            (int)(uint)*(ushort *)(param_2 + 2) & 0xff;
    FUN_101ddb66(local_60,(int)param_2 + 0xe,uVar7 * uVar6 * 4,0x30);
    iVar12 = 0;
    sVar5 = 1;
    for (local_90 = 0; uVar6 != local_90; local_90 = local_90 + 1) {
      uVar11 = 0;
      puVar9 = local_60 + iVar12 * 2;
      while (uVar11 != uVar7) {
        uVar1 = puVar9[1];
        local_78 = (short)uVar11 * (short)param_2[2];
        uVar11 = uVar11 + 1;
        local_76 = *(short *)((int)param_2 + 10) * (short)local_90;
        local_72 = sVar5 * *(short *)((int)param_2 + 10) + -1;
        iVar3 = *(ushort *)((int)param_2 + 6) - 1;
        local_74 = (short)uVar11 * (short)param_2[2] + -1;
        if (iVar3 < local_72) {
          local_72 = (short)iVar3;
        }
        if ((int)(*(ushort *)(param_2 + 1) - 1) < (int)local_74) {
          local_74 = (short)(*(ushort *)(param_2 + 1) - 1);
        }
        local_6e = (short)param_8;
        local_6a = local_6e + -1 + (short)param_10;
        local_70 = (short)param_7;
        local_6c = (short)param_7 + (short)param_9 + -1;
        iVar3 = FUN_101de8a0(&local_68,&local_70,&local_78);
        if (iVar3 != 0) {
          iVar3 = (int)local_66;
          iVar4 = (int)local_68;
          iVar8 = (local_62 + 1) - iVar3;
          iVar10 = (local_64 + 1) - iVar4;
          iVar3 = FUN_100ff968(param_1,(uint)*puVar9 + (int)param_2,uVar1,
                               param_4 + (param_6 * (iVar3 - param_8) + (iVar4 - param_7)) * 2,
                               param_5,param_6,iVar4 - local_78,iVar3 - local_76,iVar10,iVar8);
          if (iVar3 != 0) {
            FUN_1014c818((DAT_100ffc88 - DAT_100ffc84) * 0x20 & 0xff00U | 0xbb0011,DAT_100ffc8c,
                         iVar3);
            return 0xfffffffd;
          }
          if (iVar8 * iVar10 < 0x100) {
            uVar2 = 1;
          }
          else {
            uVar2 = 0x32;
          }
          FUN_101be684(*param_1,uVar2);
        }
        puVar9 = puVar9 + 2;
      }
      sVar5 = sVar5 + 1;
      iVar12 = iVar12 + uVar7;
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_100ff968(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,
                         param_10);
  }
  return uVar2;
}


