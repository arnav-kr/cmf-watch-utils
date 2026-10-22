// entry 0x100e7c6c  FUN_100e7c6c

undefined4 FUN_100e7c6c(void)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  char *pcVar11;
  undefined4 *puVar12;
  char local_208 [4];
  undefined4 auStack_204 [120];
  
  puVar2 = DAT_100e7c48;
  if (*DAT_100e7c84 == *(int *)(DAT_100e7c88 + 0xa94)) {
    return 0;
  }
  uVar9 = (DAT_100e7c44 - DAT_100e7c40) * 0x20 & 0xff00;
  FUN_1014c818(uVar9 | 0x4a0031,DAT_100e7c4c);
  iVar7 = DAT_100e7c50;
  uVar10 = 0;
  puVar12 = puVar2;
  do {
    if (*(byte *)(puVar2 + 2) <= uVar10) {
      FUN_101bf3aa(DAT_100e7c5c,DAT_100e7c58,0x19a8);
      pcVar11 = DAT_100e7c60;
      *puVar2 = puVar2[1];
      FUN_101bf3aa(local_208,pcVar11,0x1e0);
      FUN_101bf3b2(DAT_100e7c60,0,0x1e0);
      uVar3 = DAT_100e7c68;
      uVar10 = 0;
      pcVar11 = DAT_100e7c60;
      puVar12 = puVar2;
      do {
        if (*(byte *)(puVar2 + 2) <= uVar10) {
          FUN_1014c818(uVar9 | 0x6e0031,DAT_100e7c64);
          return 1;
        }
        iVar7 = 0;
        cVar1 = *(char *)(puVar12 + 3);
        *pcVar11 = cVar1;
        do {
          if (local_208[iVar7 * 0xc] == cVar1) {
            uVar6 = auStack_204[iVar7 * 3];
            *(undefined4 *)pcVar11 = *(undefined4 *)(local_208 + iVar7 * 0xc);
            uVar4 = auStack_204[iVar7 * 3 + 1];
            *(undefined4 *)(pcVar11 + 4) = uVar6;
            *(undefined4 *)(pcVar11 + 8) = uVar4;
            FUN_1014c818(uVar9 | 0x690033,uVar3,uVar10,pcVar11[1],*(undefined4 *)(pcVar11 + 8));
            break;
          }
          iVar7 = iVar7 + 1;
        } while (iVar7 != 0x28);
        uVar10 = uVar10 + 1;
        pcVar11 = pcVar11 + 0xc;
        puVar12 = puVar12 + 0x29;
      } while( true );
    }
    iVar8 = DAT_100e7c50;
    for (uVar5 = 0; uVar5 < *(byte *)(iVar7 + 0xa98); uVar5 = uVar5 + 1) {
      pcVar11 = (char *)(iVar8 + 0xa9c);
      iVar8 = iVar8 + 0xa4;
      if (*(char *)(puVar12 + 3) == *pcVar11) {
        iVar8 = FUN_101bf39a(puVar12 + 3,uVar5 * 0xa4 + iVar7 + 0xa9c);
        if (iVar8 != 0) {
          puVar2[uVar10 * 3 + 0x66b] = 0;
          puVar2[uVar10 * 3 + 0x66c] = 0;
          puVar2[uVar10 * 3 + 0x66d] = 0;
          FUN_1014c818(uVar9 | 0x520031,DAT_100e7c54,uVar10);
        }
        break;
      }
    }
    uVar10 = uVar10 + 1;
    puVar12 = puVar12 + 0x29;
  } while( true );
}


