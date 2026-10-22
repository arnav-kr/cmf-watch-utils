// 0x1023e72d lv_qrcode_1 :: lv_qrcode_update
// entry 0x10134258  lv_qrcode_1

undefined4 lv_qrcode_1(undefined4 param_1,undefined4 param_2,uint param_3,undefined1 param_4)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  uint uVar15;
  int iVar16;
  int local_5c;
  int local_58;
  int local_50;
  int local_48;
  
  lv_canvas_5(param_1,1,0xff);
  if (param_3 < 0xf4f) {
    puVar1 = (uint *)lv_canvas_3(param_1);
    uVar2 = FUN_101cb2ca(1,param_3);
    if ((0 < (int)uVar2) && (iVar3 = FUN_101cb30e(), 0 < iVar3)) {
      uVar7 = (*puVar1 & 0x1fffff) >> 10;
      iVar8 = (int)uVar7 / iVar3;
      if (iVar8 != 0) {
        uVar7 = (uVar7 - iVar3 * iVar8) / (uint)(iVar8 << 2);
        if (((uVar7 != 0) && ((int)uVar2 < 0x28)) && (uVar2 = uVar2 + uVar7, 0x27 < uVar2)) {
          uVar2 = 0x28;
        }
        iVar3 = uVar2 * 4 + 0x11;
        iVar3 = iVar3 * iVar3;
        iVar8 = iVar3 + 7;
        if (iVar8 < 0) {
          iVar8 = iVar3 + 0xe;
        }
        iVar8 = (iVar8 >> 3) + 1;
        iVar3 = FUN_1013a14c(iVar8,DAT_101344f0);
        if (iVar3 == 0) {
          FUN_1013a098(3,DAT_10134500,0x6a,DAT_101344f0,DAT_101344fc,DAT_101344f8,DAT_101344f4);
          FUN_101ba194(DAT_10134508,DAT_10134504,DAT_10134500,0x6a);
          FUN_101ba194(DAT_1013450c);
          FUN_100eeb30(DAT_10134500,0x6a);
        }
        iVar8 = FUN_1013a14c(iVar8,DAT_101344f0);
        if (iVar8 == 0) {
          FUN_1013a098(3,DAT_10134500,0x6c,DAT_101344f0,DAT_101344fc,DAT_10134510,DAT_101344f4);
          FUN_101ba194(DAT_10134508,DAT_10134504,DAT_10134500,0x6c);
          FUN_101ba194(DAT_1013450c);
          FUN_100eeb30(DAT_10134500,0x6c);
        }
        thunk_FUN_101bf3aa(iVar8,param_2,param_3);
        iVar4 = FUN_101cb280(iVar8,param_3,iVar3,param_4,uVar2,uVar2,0xffffffff,1);
        if (iVar4 != 0) {
          uVar2 = *puVar1;
          iVar5 = qrcodegen_8(iVar3);
          uVar7 = (uVar2 & 0x1fffff) >> 10;
          iVar16 = (int)uVar7 / iVar5;
          uVar2 = puVar1[2];
          iVar5 = iVar16 * iVar5;
          iVar11 = (int)(((*puVar1 & 0x1fffff) >> 10) + 7) >> 3;
          uVar7 = (int)(uVar7 - iVar5) >> 1;
          local_48 = uVar2 + uVar7 * iVar11 + iVar11;
          local_58 = uVar7 * iVar11;
          iVar9 = iVar5 + uVar7;
          if (iVar9 < (int)uVar7) {
            iVar5 = 0;
          }
          for (local_50 = 0; (int)(local_50 + uVar7) < iVar9; local_50 = local_50 + iVar16) {
            uVar10 = 0;
            local_5c = 0;
            uVar15 = uVar10;
            for (uVar13 = uVar7; (int)uVar13 < iVar9; uVar13 = uVar13 + 1) {
              uVar6 = qrcodegen_11(iVar3,(int)(uVar13 - uVar7) / iVar16,local_50 / iVar16);
              if ((local_5c == 0) && ((uVar13 & 7) != 0)) {
                lv_canvas_1(param_1,uVar13,local_50 + uVar7,(uVar6 ^ 1) & 0xff);
              }
              else {
                if (uVar6 == 0) {
                  uVar15 = 1 << (7 - uVar10 & 0xff) & 0xffU | uVar15;
                }
                uVar10 = uVar10 + 1 & 0xff;
                local_5c = iVar4;
                if (uVar10 == 8) {
                  *(char *)(uVar2 + 8 + ((int)uVar13 >> 3) + local_58) = (char)uVar15;
                  uVar15 = 0;
                  uVar10 = 0;
                }
              }
            }
            if (uVar10 != 0) {
              *(byte *)(uVar2 + 8 + ((int)(iVar5 + uVar7) >> 3) + local_58) =
                   (char)(1 << (8 - uVar10 & 0xff)) - 1U | (byte)uVar15;
            }
            iVar14 = local_48;
            for (iVar12 = 1; iVar12 < iVar16; iVar12 = iVar12 + 1) {
              thunk_FUN_101bf3aa(iVar14 + 8,uVar2 + 8 + local_58,iVar11);
              iVar14 = iVar14 + iVar11;
            }
            local_48 = local_48 + iVar16 * iVar11;
            local_58 = local_58 + iVar16 * iVar11;
          }
          FUN_1013a160(iVar3,DAT_101344f0);
          FUN_1013a160(iVar8,DAT_101344f0);
          return 1;
        }
        FUN_1013a160(iVar3,DAT_101344f0);
        FUN_1013a160(iVar8,DAT_101344f0);
      }
    }
  }
  return 0;
}


