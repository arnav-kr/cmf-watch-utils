// 0x1023e98b qrcodegen_10 :: qrcodegen_VERSION_MIN <= minVersion && minVersion <= maxVersion && maxVersion <= qrcodegen_VERSION_MAX
// entry 0x10134a84  qrcodegen_10

undefined4
qrcodegen_10(undefined1 *param_1,int param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            char param_7,int param_8,undefined1 *param_9)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  byte *pbVar11;
  undefined4 uVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  undefined4 uVar16;
  byte *pbVar17;
  uint uVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  int iVar22;
  undefined1 *puVar23;
  bool bVar24;
  undefined4 local_5c;
  byte local_58 [36];
  
  uVar16 = DAT_10134dac;
  if ((param_1 == (undefined1 *)0x0) && (param_2 != 0)) {
    uVar9 = 0xcd;
    uVar12 = DAT_10134da8;
  }
  else if (((int)param_4 < 1) || (((int)param_5 < (int)param_4 || (0x28 < (int)param_5)))) {
    uVar9 = 0xce;
    uVar12 = DAT_10134db4;
  }
  else if ((param_3 < 4) && (param_6 + 1 < 9)) {
    while( true ) {
      iVar4 = qrcodegen_5(param_4,param_3);
      uVar5 = qrcodegen_2(param_1,param_2,param_4);
      if ((uVar5 != 0xffffffff) && ((int)uVar5 <= iVar4 * 8)) break;
      if (param_4 == param_5) {
        *param_9 = 0;
        return 0;
      }
      param_4 = param_4 + 1;
    }
    uVar18 = 1;
    do {
      if (param_7 != '\0') {
        iVar4 = qrcodegen_5(param_4,uVar18 & 0xff);
        if ((int)uVar5 <= iVar4 * 8) {
          param_3 = uVar18 & 0xff;
        }
      }
      uVar18 = uVar18 + 1;
    } while (uVar18 != 4);
    iVar19 = 0;
    iVar13 = param_4 * 4 + 0x11;
    iVar13 = iVar13 * iVar13;
    iVar4 = iVar13 + 7;
    if (iVar4 < 0) {
      iVar4 = iVar13 + 0xe;
    }
    FUN_101bf3b2(param_9,0,(iVar4 >> 3) + 1);
    local_5c = 0;
    for (; iVar19 != param_2; iVar19 = iVar19 + 1) {
      qrcodegen_3(*param_1,4,param_9,&local_5c);
      uVar16 = qrcodegen_1(*param_1,param_4);
      qrcodegen_3(*(undefined4 *)(param_1 + 4),uVar16,param_9,&local_5c);
      for (uVar18 = 0; (int)uVar18 < *(int *)(param_1 + 0xc); uVar18 = uVar18 + 1) {
        qrcodegen_3((int)(uint)*(byte *)(*(int *)(param_1 + 8) + ((int)uVar18 >> 3)) >>
                    (~uVar18 & 7) & 1,1,param_9,&local_5c);
      }
      param_1 = param_1 + 0x10;
    }
    if (local_5c == uVar5) {
      iVar4 = qrcodegen_5(param_4,param_3);
      if (iVar4 * 8 < (int)uVar5) {
        uVar9 = 0xf3;
        uVar16 = DAT_10134dac;
        uVar12 = DAT_10134dc0;
      }
      else {
        iVar13 = iVar4 * 8 - uVar5;
        if (3 < iVar13) {
          iVar13 = 4;
        }
        qrcodegen_3(0,iVar13,param_9,&local_5c);
        uVar5 = local_5c & 7;
        if (-1 < (int)-local_5c) {
          uVar5 = -(-local_5c & 7);
        }
        qrcodegen_3(0,-uVar5 & 7,param_9,&local_5c);
        if ((local_5c & 7) == 0) {
          uVar5 = 0xec;
          while ((int)local_5c < iVar4 * 8) {
            qrcodegen_3(uVar5,8,param_9,&local_5c);
            uVar5 = uVar5 ^ 0xfd;
          }
          if (param_4 - 1 < 0x28) {
            iVar13 = (int)*(char *)(DAT_10134dd0 + param_3 * 0x29 + param_4);
            iVar19 = (int)*(char *)(param_3 * 0x29 + DAT_10134dd4 + param_4);
            iVar4 = qrcodegen_4(param_4);
            if (iVar4 < 0) {
              iVar4 = iVar4 + 7;
            }
            iVar6 = qrcodegen_5(param_4,param_3);
            iVar14 = (iVar4 >> 3) / iVar13;
            iVar4 = (iVar4 >> 3) - iVar13 * iVar14;
            uVar5 = iVar19 - 1;
            iVar10 = iVar13 - iVar4;
            iVar14 = iVar14 - iVar19;
            if (uVar5 < 0x1e) {
              uVar16 = 1;
              FUN_101ddb8c(local_58,0,iVar19,0x1e);
              *(undefined1 *)((int)&local_5c + iVar19 + 3) = 1;
              for (iVar21 = 0; iVar21 < iVar19; iVar21 = iVar21 + 1) {
                iVar22 = 0;
                pbVar17 = local_58;
                while( true ) {
                  bVar2 = FUN_101cb070(*pbVar17,uVar16);
                  iVar22 = iVar22 + 1;
                  *pbVar17 = bVar2;
                  if (iVar19 <= iVar22) break;
                  *pbVar17 = bVar2 ^ pbVar17[1];
                  pbVar17 = pbVar17 + 1;
                }
                uVar16 = FUN_101cb070(uVar16,2);
              }
              pbVar17 = param_9 + iVar6;
              puVar23 = param_9;
              for (iVar21 = 0; iVar21 < iVar13; iVar21 = iVar21 + 1) {
                iVar22 = iVar14;
                if (iVar10 <= iVar21) {
                  iVar22 = iVar14 + 1;
                }
                FUN_101bf3b2(pbVar17,0,iVar19);
                for (iVar7 = 0; iVar7 < iVar22; iVar7 = iVar7 + 1) {
                  bVar1 = puVar23[iVar7];
                  bVar2 = *pbVar17;
                  FUN_101bf3a2(pbVar17,pbVar17 + 1,uVar5);
                  pbVar17[uVar5] = 0;
                  pbVar11 = pbVar17 + -1;
                  for (iVar8 = 0; iVar8 < iVar19; iVar8 = iVar8 + 1) {
                    bVar3 = FUN_101cb070(local_58[iVar8],bVar2 ^ bVar1);
                    pbVar11 = pbVar11 + 1;
                    *pbVar11 = bVar3 ^ *pbVar11;
                  }
                }
                iVar7 = iVar21;
                for (iVar8 = 0; iVar8 < iVar22; iVar8 = iVar8 + 1) {
                  if (iVar14 == iVar8) {
                    iVar7 = iVar7 + (iVar4 - iVar13);
                  }
                  *(undefined1 *)(param_8 + iVar7) = puVar23[iVar8];
                  iVar7 = iVar7 + iVar13;
                }
                iVar7 = iVar21 + iVar6;
                for (iVar8 = 0; iVar8 < iVar19; iVar8 = iVar8 + 1) {
                  *(byte *)(param_8 + iVar7) = pbVar17[iVar8];
                  iVar7 = iVar7 + iVar13;
                }
                puVar23 = puVar23 + iVar22;
              }
              FUN_101cb158(param_4,param_9);
              uVar5 = qrcodegen_4(param_4);
              iVar4 = qrcodegen_8(param_9);
              uVar18 = 0;
              iVar13 = iVar4 + -1;
              if ((int)uVar5 < 0) {
                uVar5 = uVar5 + 7;
              }
              for (; 0 < iVar13; iVar13 = iVar13 + -2) {
                if (iVar13 == 6) {
                  iVar13 = 5;
                }
                uVar15 = iVar13 + 1U & 2;
                for (iVar19 = 1; iVar19 + -1 < iVar4; iVar19 = iVar19 + 1) {
                  iVar6 = iVar19 + -1;
                  if (uVar15 == 0) {
                    iVar6 = iVar4 - iVar19;
                  }
                  iVar10 = 0;
                  while( true ) {
                    iVar21 = iVar13 - iVar10;
                    iVar14 = qrcodegen_6(param_9);
                    if ((iVar14 == 0) && ((int)uVar18 < (int)(uVar5 & 0xfffffff8))) {
                      qrcodegen_7(param_9,iVar21,iVar6,
                                  (int)(uint)*(byte *)(param_8 + ((int)uVar18 >> 3)) >>
                                  (~uVar18 & 7) & 1,uVar15,iVar6);
                      uVar18 = uVar18 + 1;
                    }
                    if (iVar10 != 0) break;
                    iVar10 = 1;
                  }
                }
              }
              if ((uVar5 & 0xfffffff8) == uVar18) {
                iVar4 = qrcodegen_8(param_9);
                for (iVar13 = 7; iVar13 < iVar4 + -7; iVar13 = iVar13 + 2) {
                  qrcodegen_7(param_9,6,iVar13,0);
                  qrcodegen_7(param_9,iVar13,6,0);
                }
                iVar13 = -1;
                iVar19 = iVar4 + -8;
                do {
                  iVar6 = 8;
                  do {
                    iVar10 = FUN_101ddb02(4 - iVar6);
                    iVar14 = FUN_101ddb02(iVar13 + -3);
                    if (iVar10 < iVar14) {
                      iVar10 = FUN_101ddb02(iVar13 + -3);
                    }
                    if ((iVar10 == 2) || (iVar10 == 4)) {
                      FUN_101cb264(param_9,7 - iVar6,iVar13);
                      FUN_101cb264(param_9,iVar4 - iVar6,iVar13);
                      FUN_101cb264(param_9,7 - iVar6,iVar19);
                    }
                    bVar24 = iVar6 != 0;
                    iVar6 = iVar6 + -1;
                  } while (bVar24);
                  iVar13 = iVar13 + 1;
                  iVar19 = iVar19 + 1;
                } while (iVar13 != 8);
                iVar13 = FUN_101cb09a(param_4,local_58);
                iVar19 = iVar13 + -1;
                pbVar17 = local_58;
                for (iVar6 = 0; iVar6 < iVar13; iVar6 = iVar6 + 1) {
                  iVar10 = 0;
                  do {
                    if (((iVar6 != 0) || ((iVar10 != 0 && (iVar19 != iVar10)))) &&
                       ((iVar6 != iVar19 || (iVar10 != 0)))) {
                      iVar14 = -1;
                      do {
                        iVar21 = -1;
                        do {
                          bVar24 = iVar21 == 0;
                          iVar22 = (uint)*pbVar17 + iVar21;
                          iVar21 = iVar21 + 1;
                          qrcodegen_7(param_9,iVar22,(uint)local_58[iVar10] + iVar14,
                                      bVar24 && iVar14 == 0);
                        } while (iVar21 != 2);
                        iVar14 = iVar14 + 1;
                      } while (iVar14 != 2);
                    }
                    iVar10 = iVar10 + 1;
                  } while (iVar13 != iVar10);
                  pbVar17 = pbVar17 + 1;
                }
                if (6 < (int)param_4) {
                  iVar13 = 0xc;
                  uVar5 = param_4;
                  do {
                    iVar13 = iVar13 + -1;
                    uVar5 = ((int)uVar5 >> 0xb) * 0x1f25 ^ uVar5 << 1;
                  } while (iVar13 != 0);
                  uVar5 = uVar5 | param_4 << 0xc;
                  if ((int)uVar5 >> 0x12 != 0) {
                    uVar9 = 0x1ed;
                    uVar16 = DAT_101352b4;
                    uVar12 = DAT_101352b0;
                    goto LAB_10134aaa;
                  }
                  iVar13 = 0;
                  do {
                    iVar19 = iVar4 + -0xb;
                    do {
                      uVar18 = uVar5 & 1;
                      qrcodegen_7(param_9,iVar19,iVar13);
                      iVar6 = iVar19 + 1;
                      qrcodegen_7(param_9,iVar13,iVar19,uVar18);
                      uVar5 = (int)uVar5 >> 1;
                      iVar19 = iVar6;
                    } while (iVar4 + -8 != iVar6);
                    iVar13 = iVar13 + 1;
                  } while (iVar13 != 6);
                }
                FUN_101cb158(param_4,param_8);
                if (param_6 == 0xffffffff) {
                  uVar5 = 0;
                  iVar4 = 0x7fffffff;
                  do {
                    iVar19 = (int)(char)uVar5;
                    FUN_1013486c(param_8,param_9,iVar19);
                    qrcodegen_9(param_3,iVar19,param_9);
                    iVar13 = qrcodegen_8(param_9);
                    iVar6 = 0;
                    for (iVar10 = 0; iVar10 < iVar13; iVar10 = iVar10 + 1) {
                      bVar2 = 0;
                      iVar14 = 0;
                      iVar21 = 0;
                      local_58[0] = 0;
                      local_58[1] = 0;
                      local_58[2] = 0;
                      local_58[3] = 0;
                      local_58[4] = 0;
                      local_58[5] = 0;
                      local_58[6] = 0;
                      do {
                        iVar22 = qrcodegen_6(param_9,iVar21,iVar10);
                        if (iVar22 == iVar14) {
                          bVar2 = bVar2 + 1;
                          if (bVar2 == 5) {
                            iVar6 = iVar6 + 3;
                          }
                          else if (5 < bVar2) {
                            iVar6 = iVar6 + 1;
                          }
                        }
                        else {
                          FUN_101cb252(bVar2,local_58);
                          if ((iVar14 == 0) && (iVar14 = FUN_101cb0e6(local_58), iVar14 != 0)) {
                            iVar6 = iVar6 + 0x28;
                          }
                          iVar14 = qrcodegen_6(param_9,iVar21,iVar10);
                          bVar2 = 1;
                        }
                        iVar21 = iVar21 + 1;
                      } while (iVar13 != iVar21);
                      FUN_101cb252(bVar2,local_58);
                      if (iVar14 != 0) {
                        FUN_101cb252(0,local_58);
                      }
                      iVar14 = FUN_101cb0e6(local_58);
                      if (iVar14 != 0) {
                        iVar6 = iVar6 + 0x28;
                      }
                    }
                    for (iVar10 = 0; iVar10 < iVar13; iVar10 = iVar10 + 1) {
                      bVar2 = 0;
                      iVar14 = 0;
                      iVar21 = 0;
                      local_58[0] = 0;
                      local_58[1] = 0;
                      local_58[2] = 0;
                      local_58[3] = 0;
                      local_58[4] = 0;
                      local_58[5] = 0;
                      local_58[6] = 0;
                      do {
                        iVar22 = qrcodegen_6(param_9,iVar10,iVar21);
                        if (iVar22 == iVar14) {
                          bVar2 = bVar2 + 1;
                          if (bVar2 == 5) {
                            iVar6 = iVar6 + 3;
                          }
                          else if (5 < bVar2) {
                            iVar6 = iVar6 + 1;
                          }
                        }
                        else {
                          FUN_101cb252(bVar2,local_58);
                          if ((iVar14 == 0) && (iVar14 = FUN_101cb0e6(local_58), iVar14 != 0)) {
                            iVar6 = iVar6 + 0x28;
                          }
                          iVar14 = qrcodegen_6(param_9,iVar10,iVar21);
                          bVar2 = 1;
                        }
                        iVar21 = iVar21 + 1;
                      } while (iVar13 != iVar21);
                      FUN_101cb252(bVar2,local_58);
                      if (iVar14 != 0) {
                        FUN_101cb252(0,local_58);
                      }
                      iVar14 = FUN_101cb0e6(local_58);
                      if (iVar14 != 0) {
                        iVar6 = iVar6 + 0x28;
                      }
                    }
                    iVar10 = iVar13 + -1;
                    for (iVar14 = 1; iVar21 = iVar14 + -1, iVar21 < iVar10; iVar14 = iVar14 + 1) {
                      iVar22 = 0;
                      do {
                        iVar7 = qrcodegen_6(param_9,iVar22,iVar21);
                        iVar20 = iVar22 + 1;
                        iVar8 = qrcodegen_6(param_9,iVar20,iVar21);
                        if (((iVar7 == iVar8) &&
                            (iVar22 = qrcodegen_6(param_9,iVar22,iVar14), iVar7 == iVar22)) &&
                           (iVar22 = qrcodegen_6(param_9,iVar20,iVar14), iVar7 == iVar22)) {
                          iVar6 = iVar6 + 3;
                        }
                        iVar22 = iVar20;
                      } while (iVar20 < iVar10);
                    }
                    iVar10 = 0;
                    for (iVar14 = 0; iVar14 < iVar13; iVar14 = iVar14 + 1) {
                      iVar21 = 0;
                      do {
                        iVar22 = qrcodegen_6(param_9,iVar21,iVar14);
                        if (iVar22 != 0) {
                          iVar10 = iVar10 + 1;
                        }
                        iVar21 = iVar21 + 1;
                      } while (iVar13 != iVar21);
                    }
                    iVar13 = iVar13 * iVar13;
                    iVar10 = FUN_101ddb5e(iVar10 * 0x14 + iVar13 * -10);
                    iVar6 = ((iVar10 + iVar13 + -1) / iVar13 + -1) * 10 + iVar6;
                    if (iVar6 < iVar4) {
                      iVar4 = iVar6;
                      param_6 = uVar5;
                    }
                    uVar5 = uVar5 + 1;
                    FUN_1013486c(param_8,param_9,iVar19);
                  } while (uVar5 != 8);
                }
                if (param_6 < 8) {
                  FUN_1013486c(param_8,param_9,(int)(char)param_6);
                  qrcodegen_9(param_3,(int)(char)param_6,param_9);
                  return 1;
                }
                uVar9 = 0x115;
                uVar16 = DAT_101352ac;
                uVar12 = DAT_101352a8;
              }
              else {
                uVar9 = 0x252;
                uVar16 = DAT_10134de4;
                uVar12 = DAT_10134de0;
              }
            }
            else {
              uVar9 = 0x163;
              uVar16 = DAT_10134ddc;
              uVar12 = DAT_10134dd8;
            }
          }
          else {
            uVar9 = 0x125;
            uVar16 = DAT_10134dcc;
            uVar12 = DAT_10134dc8;
          }
        }
        else {
          uVar9 = 0xf9;
          uVar16 = DAT_10134dac;
          uVar12 = DAT_10134dc4;
        }
      }
    }
    else {
      uVar9 = 0xef;
      uVar16 = DAT_10134dac;
      uVar12 = DAT_10134dbc;
    }
  }
  else {
    uVar9 = 0xcf;
    uVar12 = DAT_10134db8;
  }
LAB_10134aaa:
                    /* WARNING: Subroutine does not return */
  FUN_101a9a68(DAT_10134db0,uVar9,uVar16,uVar12);
}


