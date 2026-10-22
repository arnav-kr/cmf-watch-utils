// 0x1021eb6b FUN_100c419c :: alarms_update snooze!!!\n
// entry 0x100c419c  FUN_100c419c

undefined4 FUN_100c419c(uint param_1)

{
  byte bVar1;
  char cVar2;
  byte bVar3;
  byte *pbVar4;
  byte bVar5;
  undefined4 uVar6;
  byte bVar7;
  byte bVar8;
  int iVar9;
  byte bVar10;
  byte *pbVar11;
  char *pcVar12;
  uint uVar13;
  undefined1 auStack_8c [5];
  char local_87;
  undefined2 local_7c;
  undefined2 local_50;
  
  if (param_1 < 5) {
    FUN_1014c818(((int)PTR_DAT_100c4304 - (int)PTR_LAB_100c4300) * 0x20 & 0xff00U | 0x3f40031,
                 PTR_s_alarms_update_snooze____100c4308);
    FUN_1014bd9c(auStack_8c);
    pbVar4 = DAT_100c4310;
    iVar9 = param_1 * 0x2a + DAT_100c430c;
    cVar2 = *(char *)(iVar9 + 0x937);
    bVar3 = *(byte *)(iVar9 + 0x936);
    bVar8 = cVar2 + DAT_100c4310[param_1] * '\n';
    bVar10 = bVar3;
    if (0x3b < bVar8) {
      bVar10 = bVar3 + 1;
      if (0x17 < bVar10) {
        bVar10 = 0;
      }
      bVar8 = bVar8 - 0x3c;
    }
    if (local_87 == '\0') {
      local_87 = '\a';
    }
    uVar13 = 0;
    iVar9 = DAT_100c430c;
    pbVar11 = DAT_100c4310;
    pcVar12 = DAT_100c4314;
    do {
      if ((*(char *)(iVar9 + 0x938) != '\0') && (param_1 != uVar13)) {
        bVar7 = *(byte *)(iVar9 + 0x936);
        if (*pcVar12 == '\x01') {
          bVar1 = *pbVar11;
          bVar5 = *(char *)(iVar9 + 0x937) + bVar1 * '\n';
          if (0x3b < bVar5) {
            bVar7 = bVar7 + 1;
            if (0x17 < bVar7) {
              bVar7 = 0;
            }
            bVar5 = bVar5 - 0x3c;
          }
          local_7c = CONCAT11(bVar5,bVar7);
          local_50 = CONCAT11(bVar8,bVar10);
          if ((local_50 == local_7c) && (bVar1 < 4)) {
            *pbVar11 = bVar1 + 1;
          }
        }
        else if ((((bVar3 == bVar7) && (*(char *)(iVar9 + 0x937) == cVar2)) &&
                 (-1 < *(char *)(iVar9 + 0x93a))) &&
                ((*(byte *)(iVar9 + 0x93a) == 0 ||
                 (((uint)*(byte *)(iVar9 + 0x93a) & 1 << (uint)(byte)(local_87 - 1)) != 0)))) {
          *pcVar12 = '\x01';
          if (*pbVar11 < 4) {
            *pbVar11 = *pbVar11 + 1;
          }
        }
      }
      uVar13 = uVar13 + 1;
      iVar9 = iVar9 + 0x2a;
      pbVar11 = pbVar11 + 1;
      pcVar12 = pcVar12 + 1;
    } while (uVar13 != 5);
    DAT_100c4314[param_1] = '\x01';
    bVar3 = pbVar4[param_1];
    if (bVar3 < 4) {
      pbVar4[param_1] = bVar3 + 1;
    }
    iVar9 = FUN_100c3980(param_1,0);
    uVar6 = 0;
    if (iVar9 != 0) {
      uVar6 = 1;
    }
  }
  else {
    uVar6 = 0xffffffff;
  }
  return uVar6;
}


