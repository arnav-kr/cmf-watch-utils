// entry 0x100ccc0c  FUN_100ccc0c

int FUN_100ccc0c(short *param_1,undefined4 param_2,int param_3,int param_4)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  short sVar6;
  undefined4 *puVar7;
  int iVar8;
  uint uVar9;
  undefined4 *puVar10;
  int iVar11;
  undefined4 uVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  int iVar16;
  undefined1 auStack_c8 [176];
  undefined4 *puVar15;
  
  puVar7 = DAT_100ccd2c;
  if (param_3 + 0x10 < 0x200) {
    puVar10 = DAT_100ccd30 + 4;
    puVar13 = DAT_100ccd2c;
    puVar14 = DAT_100ccd30;
    do {
      puVar15 = puVar14 + 1;
      *puVar13 = *puVar14;
      puVar13 = puVar13 + 1;
      puVar14 = puVar15;
    } while (puVar15 != puVar10);
    FUN_101ddb66(DAT_100ccd34,param_2,param_3,0x1f0);
    if (param_4 != 0) {
      FUN_100fdb68(auStack_c8,param_4);
    }
    iVar8 = DAT_100ccd38;
    iVar11 = FUN_101bdaf6(DAT_100ccd38,param_3,DAT_100ccd34,param_3,puVar7,auStack_c8);
    if (iVar11 == 1) {
      bVar5 = *(byte *)(iVar8 + param_3 + -1);
      sVar6 = ((short)param_3 - (ushort)bVar5) + -4;
      iVar16 = (int)sVar6;
      iVar11 = iVar8 + iVar16;
      bVar1 = *(byte *)(iVar11 + 2);
      bVar2 = *(byte *)(iVar11 + 1);
      bVar4 = *(byte *)(iVar8 + iVar16);
      bVar3 = *(byte *)(iVar11 + 3);
      uVar9 = DAT_100ccd40;
      uVar12 = DAT_100ccd3c;
      if ((bVar5 - 1 < 0x10) && (uVar9 = DAT_100ccd50, uVar12 = DAT_100ccd4c, 0 < iVar16)) {
        iVar11 = FUN_100ccafc(iVar8,iVar16,0);
        if ((uint)bVar1 * 0x10000 + (uint)bVar2 * 0x100 + (uint)bVar4 + (uint)bVar3 * 0x1000000 ==
            iVar11) {
          *param_1 = sVar6;
          return iVar8;
        }
        uVar9 = (DAT_100ccd20 - DAT_100ccd24) * 0x20 & 0xff00U | 0x1440011;
        uVar12 = DAT_100ccd54;
      }
      else {
        uVar9 = uVar9 | (DAT_100ccd20 - DAT_100ccd24) * 0x20 & 0xff00U;
      }
      FUN_1014c818(uVar9,uVar12);
      uVar9 = DAT_100ccd44 | (DAT_100ccd20 - DAT_100ccd24) * 0x20 & 0xff00U;
      uVar12 = DAT_100ccd48;
      goto LAB_100ccc38;
    }
  }
  uVar9 = (DAT_100ccd20 - DAT_100ccd24) * 0x20 & 0xff00U | 0x1700011;
  uVar12 = DAT_100ccd28;
LAB_100ccc38:
  FUN_1014c818(uVar9,uVar12);
  return 0;
}


