// entry 0x10054dbc  FUN_10054dbc  (10054dbc)

undefined4 FUN_10054dbc(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  undefined4 *puVar8;
  undefined1 local_6f;
  short local_6e;
  undefined4 local_6c;
  undefined4 local_68 [4];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  
  local_54 = *DAT_10054f38;
  uStack_50 = DAT_10054f38[1];
  uStack_4c = DAT_10054f38[2];
  uStack_48 = DAT_10054f38[3];
  local_44 = DAT_10054f38[4];
  uStack_40 = DAT_10054f38[5];
  uStack_3c = DAT_10054f38[6];
  uStack_38 = DAT_10054f38[7];
  local_34 = DAT_10054f38[8];
  uStack_30 = DAT_10054f38[9];
  uStack_2c = DAT_10054f38[10];
  uVar2 = FUN_1019b2cc();
  iVar3 = FUN_101db1ae();
  iVar4 = FUN_101489d4(&local_54,0xb);
  if (iVar4 == 0) {
    local_6e = 0;
    local_6f = 0;
    FUN_10148d74(&local_6e,&local_6f);
    if (local_6e != 1) {
      local_6c = 6;
      ux2sys_api_8(0x27,&local_6c,4);
      if (local_6e != 1) {
        FUN_101d72f4(0x2a,0,3);
      }
    }
    sched_25(uVar2,iVar3 + 5);
    iVar4 = FUN_10054aa0();
    if ((iVar4 == 0) && (iVar4 = FUN_1015bb64(), iVar4 == 0)) {
      FUN_100f8524(1000);
      piVar1 = DAT_10054f68;
      local_68[0] = *DAT_10054f64;
      local_68[1] = DAT_10054f64[1];
      local_68[2] = DAT_10054f64[2];
      local_68[3] = DAT_10054f64[3];
      local_58 = DAT_10054f64[4];
      uVar7 = (DAT_10054f3c - DAT_10054f40) * 0x20 & 0xff00;
      iVar4 = 0;
      puVar8 = local_68;
      do {
        uVar6 = *puVar8;
        iVar5 = FUN_100f5df8(uVar6);
        *piVar1 = iVar5;
        FUN_1014c818(uVar7 | 0x5d0033,DAT_10054f48,DAT_10054f44,uVar6,iVar5);
        if (*piVar1 != 0) goto LAB_10054ec4;
        iVar4 = iVar4 + 1;
        puVar8 = puVar8 + 1;
      } while (iVar4 != 5);
      iVar4 = FUN_100583d8();
      *piVar1 = iVar4;
      FUN_1014c818(uVar7 | 0x6f0032,DAT_10054f4c,DAT_10054f44,iVar4,iVar5);
    }
LAB_10054ec4:
    FUN_1015a6fc();
    FUN_1015a1f0();
    uVar6 = FUN_1015a84c();
    *DAT_10054f50 = uVar6;
    FUN_10058438();
    sched_25(uVar2,iVar3);
    iVar3 = FUN_10054aa0();
    if (iVar3 == 0) {
      FUN_101be1ec(1000);
    }
    iVar3 = FUN_10054aa0();
    if (iVar3 != 0) {
      FUN_1014c618(DAT_10054f54,2);
      FUN_1014c818(DAT_10054f58 | (DAT_10054f3c - DAT_10054f40) * 0x20 & 0xff00U,DAT_10054f60,
                   DAT_10054f5c);
      FUN_101bacb4(1);
    }
    uVar2 = 0;
    if (local_6e != 1) {
      FUN_101d7312(0x2a);
    }
  }
  else {
    uVar2 = 0xffffffea;
  }
  return uVar2;
}


