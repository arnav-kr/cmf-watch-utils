// entry 0x10158390  FUN_10158390

int FUN_10158390(int *param_1)

{
  code *pcVar1;
  undefined1 *puVar2;
  char *pcVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int extraout_r2;
  int iVar12;
  int iVar13;
  uint uVar14;
  int *piVar15;
  uint uVar16;
  int *piVar17;
  int *piVar18;
  uint uVar19;
  uint uVar20;
  int iVar21;
  int local_50;
  int local_40;
  undefined1 uStack_3c;
  char local_3b;
  ushort local_3a;
  int local_38;
  byte local_34;
  
  local_40 = 0;
  piVar18 = param_1 + 0xb9;
  uVar16 = (DAT_1015866c - DAT_10158670) * 0x20 & 0xff00;
  FUN_1014c818(DAT_10158674 | uVar16,DAT_10158678);
  if (*param_1 != 0) {
    FUN_1014c818(DAT_1015867c | uVar16,DAT_10158680,*param_1);
    return -0x16;
  }
  iVar5 = FUN_101d099c(param_1[5]);
  if (iVar5 == 0) {
    FUN_1014c818(DAT_10158684 | uVar16,DAT_10158688);
    return -0x16;
  }
  iVar5 = FUN_101d099c(param_1[5]);
  iVar6 = FUN_10159770(param_1[5]);
  if (iVar6 != 0) {
    iVar7 = FUN_10157390(iVar5);
    if (iVar7 == 2) {
      FUN_10157764(iVar5);
    }
    FUN_1014c818(DAT_1015868c | uVar16,DAT_10158690,iVar6);
    iVar6 = -5;
    goto LAB_1015842a;
  }
  if (param_1[2] << 0x1e < 0) {
    iVar6 = FUN_101595b0(param_1[5]);
    if (iVar6 == 0) {
      if (-1 < param_1[2] << 0x1e) goto LAB_10158498;
      goto LAB_1015849c;
    }
    FUN_1014c818(DAT_101586b8 | uVar16,DAT_101586bc);
LAB_101585b6:
    uVar8 = 0xe;
LAB_10158d44:
    FUN_1015abac(piVar18,uVar8);
  }
  else {
LAB_10158498:
    FUN_1015a6fc();
LAB_1015849c:
    piVar15 = param_1 + 0xb;
    iVar6 = FUN_10159fe8(piVar15,param_1[5],DAT_101586a0);
    if (iVar6 == 0) {
      iVar6 = FUN_100f731c();
      FUN_1014c818(DAT_101586c0 | uVar16,DAT_101586c4);
      FUN_100f7374(piVar15);
      FUN_1014c818(DAT_101586c8 | uVar16,DAT_101586cc);
      FUN_100f7374(iVar6);
      uVar8 = FUN_101d099c(param_1[5]);
      uVar9 = FUN_10157390();
      if (((uVar9 == 3) ||
          (((uVar9 == 1 && (param_1[2] << 0x1e < 0)) ||
           (uVar19 = (uint)*(byte *)((int)param_1 + 0x2e6), uVar19 == 0)))) || (uVar9 == uVar19)) {
        iVar7 = FUN_101bf37a(iVar6 + 0x50,param_1 + 0x1f);
        uVar9 = DAT_101586e4;
        uVar8 = DAT_101586e0;
        if ((iVar7 != 0) ||
           ((-1 < param_1[2] << 0x1d &&
            (uVar9 = DAT_101586ec, uVar8 = DAT_101586e8, (uint)param_1[0xc] <= *(uint *)(iVar6 + 4))
            ))) {
          FUN_1014c818(uVar9 | uVar16,uVar8);
          goto LAB_10158626;
        }
        if ((*(byte *)((int)param_1 + 0x2e7) - 2 < 3) &&
           (((param_1[0xbb] != 0 && (param_1[0xbb] != param_1[0xc])) ||
            (iVar7 = *(int *)((int)param_1 + 0x35f), iVar6 = FUN_101d09a0(param_1[5]),
            iVar7 != iVar6)))) {
          FUN_1014c818(DAT_101589e4 | uVar16,DAT_101589e8);
          FUN_1014c818(DAT_101589ec | uVar16,DAT_101589f0);
          FUN_1015abac(piVar18,3);
          FUN_10157278(param_1,3);
          FUN_101574a4(param_1);
          FUN_10157278(param_1,0);
        }
        FUN_1014c818(DAT_101589f4 | uVar16,DAT_101589f8);
        if (param_1[9] == 0) {
          iVar6 = thunk_FUN_10146a3c(param_1[8],DAT_101589fc);
          param_1[9] = iVar6;
          if (iVar6 == 0) {
            FUN_1014c818(uVar16 | 0x7180011,DAT_10158a00,param_1[8]);
            goto LAB_101584bc;
          }
        }
        if ((param_1[2] & 2U) == 0) {
          for (uVar9 = 0; (int)(uVar9 & 0xff) < param_1[0x2b]; uVar9 = uVar9 + 1) {
            iVar7 = (uVar9 & 0xff) * 0x25;
            iVar6 = FUN_1015aaec(piVar18,*(undefined1 *)((int)param_1 + iVar7 + 0xc6));
            if ((iVar6 != 0) && (iVar13 = *(int *)((int)param_1 + iVar7 + 0xd9), iVar6 != iVar13)) {
              FUN_1014c818(DAT_10158a04 | uVar16,DAT_10158a08,iVar7 + 0x8c + (int)piVar15,iVar6,
                           iVar13);
              FUN_101d0a72(piVar18);
              FUN_1015a660();
              break;
            }
          }
        }
        uVar4 = FUN_10157390(iVar5);
        iVar6 = 0;
        *(undefined1 *)((int)param_1 + 0x2e6) = uVar4;
        param_1[0xbb] = param_1[0xc];
        uVar8 = FUN_101d09a0(param_1[5]);
        uVar9 = DAT_10158a64;
        iVar13 = 0;
        *(undefined4 *)((int)param_1 + 0x35f) = uVar8;
        piVar17 = param_1 + 0x2e;
        for (iVar7 = 0; iVar7 < param_1[0x2b]; iVar7 = iVar7 + 1) {
          iVar21 = partition_16(*(undefined1 *)((int)piVar17 + 0xe));
          if (iVar21 == 0) {
            FUN_1014c818(DAT_10158a44 | uVar16,DAT_10158a48,*(undefined1 *)((int)piVar17 + 0xe));
            if (-1 < param_1[2] << 0x1f) goto LAB_1015881a;
            uVar19 = FUN_101bbde0();
            iVar21 = partition_15(*(undefined1 *)((int)piVar17 + 0xe));
            if ((uVar19 != *(byte *)((int)piVar17 + 0xe)) && (iVar21 != 0)) {
              FUN_1014c818(uVar16 | 0x5d80032,DAT_10158a54);
              goto LAB_1015895c;
            }
            FUN_1014c818(DAT_10158a4c | uVar16,DAT_10158a50);
          }
          else {
LAB_1015895c:
            if ((param_1[2] << 0x1e < 0) || (iVar21 = FUN_1015a480(piVar17), iVar21 != 0)) {
              iVar21 = FUN_1015a9e8(piVar18,*(undefined1 *)((int)piVar17 + 0xe));
              if (iVar21 - 6U < 2) {
                uVar19 = *(uint *)((int)param_1 + iVar7 * 0x25 + 0xcc);
LAB_101589b8:
                iVar6 = iVar6 + uVar19;
              }
              else if ((iVar21 == 5) || (iVar21 - 2U < 2)) {
                uVar19 = *(uint *)((int)param_1 + 0x317);
                if (*(int *)((int)param_1 + iVar7 * 0x25 + 0xcc) ==
                    *(int *)((int)param_1 + iVar7 * 0x25 + 0xd0)) {
                  iVar21 = *(int *)((int)param_1 + iVar7 * 0x25 + 200);
                  uVar19 = uVar19 + iVar21 & 0xfffff000;
                  iVar6 = iVar6 - iVar21;
                }
                goto LAB_101589b8;
              }
              iVar21 = *(int *)((int)param_1 + iVar7 * 0x25 + 0xcc);
              iVar13 = iVar13 + iVar21;
              FUN_1014c818(uVar16 | uVar9,DAT_10158a60,*(undefined1 *)((int)piVar17 + 0xe),piVar17,
                           iVar21,iVar6);
            }
            else {
              FUN_1014c818(DAT_10158a58 | uVar16,DAT_10158a5c,piVar17);
            }
          }
          piVar17 = (int *)((int)piVar17 + 0x25);
        }
        if (iVar13 != 0) {
          FUN_10159264(param_1[5],iVar13,iVar6);
        }
LAB_1015881a:
        FUN_10157278(param_1,1);
        if ((param_1[2] & 2U) == 0) {
          iVar6 = param_1[5];
          FUN_101978cc(&uStack_3c);
          FUN_1014c818(DAT_10158a0c | uVar16,DAT_10158a10,param_1[0x2b]);
          FUN_101bf3b2(DAT_10158a14,0,0x1d);
          uVar9 = 0;
          for (uVar19 = 0; puVar2 = DAT_10158a14, uVar20 = uVar19 & 0xff,
              (int)uVar20 < param_1[0x2b]; uVar19 = uVar19 + 1) {
            iVar7 = (int)piVar15 + uVar20 * 0x25 + 0x8c;
            iVar13 = FUN_101bf36a(iVar7);
            if (iVar13 == 0) {
              iVar7 = 0;
            }
            iVar7 = FUN_101591c4(iVar6,iVar7);
            uVar20 = iVar7 + *(int *)((int)param_1 + uVar20 * 0x25 + 0xcc);
            if ((uVar9 < uVar20) && (uVar14 = uVar20 & 0x1ff, uVar9 = uVar20, uVar14 != 0)) {
              if (-1 < (int)-uVar20) {
                uVar14 = -(uVar20 * -0x800000 >> 0x17);
              }
              uVar9 = (uVar20 + 0x200) - uVar14;
            }
          }
          FUN_1014c818(DAT_10158a18 | uVar16,DAT_10158a1c,uVar9);
          FUN_101594e4(iVar6,uVar9,puVar2 + 5,0x18);
          uVar8 = *(undefined4 *)(puVar2 + 0x15);
          FUN_1014c818(DAT_10158a20 | uVar16,DAT_10158a24,*(undefined4 *)(puVar2 + 5),
                       *(undefined4 *)(puVar2 + 0xd),uVar8);
          iVar7 = *(int *)(puVar2 + 5);
          if (iVar7 == DAT_10158a28) {
            iVar13 = 0;
            if (*(int *)(puVar2 + 0xd) != 0) {
              if ((local_3b == '\0') || (*(uint *)(puVar2 + 0x15) <= (uint)local_3a)) {
                FUN_1014c818(DAT_10158a2c | uVar16,DAT_10158a30,local_3b,*(uint *)(puVar2 + 0x15),
                             (uint)local_3a);
                goto LAB_101588d2;
              }
              FUN_101d0a72(piVar18);
              *puVar2 = 1;
              puVar2[1] = 1;
              iVar7 = 0;
              while( true ) {
                if (param_1[0x2b] <= iVar7) {
                    /* WARNING: Does not return */
                  pcVar1 = (code *)software_udf(0xff,0x10158ad8);
                  (*pcVar1)();
                }
                if (*(char *)((int)param_1 + iVar7 * 0x25 + 0xc6) == -2) break;
                iVar7 = iVar7 + 1;
              }
              iVar13 = uVar9 + 0x18;
              uVar10 = FUN_10156e44(*(undefined1 *)((int)piVar15 + iVar7 * 0x25 + 0x9b));
              uVar20 = *(uint *)(puVar2 + 0xd);
              FUN_10159264(iVar6,uVar20,0);
              iVar7 = partition_15(0xfe);
              iVar21 = 0;
              uVar9 = 0xc00;
              FUN_101572b8(param_1,iVar7,0);
              uVar19 = DAT_10158e34 | uVar16;
              uVar11 = uVar8;
              for (; iVar12 = DAT_10158e38, uVar8 = *(undefined4 *)(iVar7 + 0xc), uVar20 != 0;
                  uVar20 = uVar20 - uVar9) {
                if (uVar20 <= uVar9) {
                  uVar9 = uVar20;
                }
                FUN_1014c818(uVar19,DAT_10158e40,iVar13,iVar21,uVar8);
                iVar12 = param_1[9];
                *(char *)(DAT_10158e38 + 2) =
                     (char)((uint)(iVar21 * 10) / *(uint *)(DAT_10158e38 + 0xd));
                iVar12 = FUN_101594e4(iVar6,iVar13,iVar12,uVar9);
                if (iVar12 != 0) {
                  FUN_1014c818(DAT_10158e44 | uVar16,DAT_10158e48,iVar13);
                  iVar7 = DAT_10158e38;
                  iVar13 = 0;
                  iVar6 = -0xb;
                  *(undefined1 *)(DAT_10158e38 + 1) = 0;
                  goto LAB_10158bda;
                }
                iVar12 = FUN_10156948(uVar10,*(int *)(iVar7 + 0xc) + iVar21,param_1[9],uVar9);
                if (iVar12 != 0) {
                  FUN_1014c818(DAT_10158e54 | uVar16,DAT_10158e58,iVar21);
                  iVar13 = DAT_10158e38;
                  *(undefined1 *)(DAT_10158e38 + 1) = 0;
                  iVar7 = 0;
                  goto LAB_10158c0c;
                }
                iVar21 = iVar21 + uVar9;
                iVar13 = iVar13 + uVar9;
                uVar11 = uVar8;
              }
              *(undefined1 *)(DAT_10158e38 + 1) = 0;
              *(undefined1 *)(iVar12 + 3) = 1;
              FUN_10158360(0xd8);
              pcVar3 = DAT_10158e98;
              while( true ) {
                FUN_101be1ec(1000);
                FUN_101978cc(&uStack_3c);
                iVar21 = local_38;
                FUN_1014c818(uVar16 | 0x6a80032,DAT_10158e3c,local_38,*pcVar3);
                iVar13 = 0;
                iVar7 = extraout_r2;
                uVar8 = uVar11;
                if (*pcVar3 == '\0') break;
                if (iVar21 != 0) {
                  if (iVar21 < 1) {
                    if (1 < iVar21 + 3U) {
LAB_10158c20:
                      iVar13 = 0;
                      *(undefined1 *)(iVar12 + 3) = 0;
                      break;
                    }
                  }
                  else if (iVar21 != 1) goto LAB_10158c20;
                  *(undefined1 *)(iVar12 + 3) = 0;
                  goto LAB_101588d2;
                }
                *(byte *)(iVar12 + 4) = local_34 / 5 + 10;
                FUN_101594e4(iVar6,0,DAT_10158e5c,0x18);
              }
            }
LAB_10158c0c:
            iVar6 = -5;
LAB_10158bda:
            FUN_1014c818(DAT_10158e4c | uVar16,DAT_10158e50,iVar7,iVar13,uVar8);
            goto LAB_101584c0;
          }
        }
LAB_101588d2:
        iVar6 = thunk_FUN_10146a3c(0x600,DAT_10158a34);
        param_1[0xd9] = iVar6;
        if (iVar6 == 0) {
          uVar8 = 0x600;
          uVar9 = DAT_10158a38;
          uVar11 = DAT_10158a00;
        }
        else {
          iVar6 = FUN_10188518(0x10000);
          param_1[0xda] = iVar6;
          if (iVar6 == 0) {
            uVar8 = 0x10000;
            uVar9 = DAT_10158e64;
            uVar11 = DAT_10158e60;
          }
          else {
            param_1[0xdb] = 0x10000;
            iVar6 = FUN_10188518(0x10000);
            param_1[0xdc] = iVar6;
            if (iVar6 != 0) {
              param_1[0xdd] = 0x10000;
              iVar6 = FUN_10188518(0x8000);
              param_1[0xde] = iVar6;
              if (iVar6 == 0) {
                FUN_1014c818(uVar16 | 0x2780011,DAT_10158e60,0x8000);
              }
              else {
                param_1[0xdf] = 0x8000;
              }
              FUN_101dae76(param_1 + 0xe0,0,5);
              FUN_101dae76(param_1 + 0xe6,0,1);
              iVar6 = param_1[0xda];
              uVar9 = param_1[0xdb];
              if ((int)uVar9 < 0) {
                FUN_101ba194(DAT_10158e74,DAT_10158e6c,DAT_10158e70,0x9e);
                FUN_101ba194(DAT_10158e78);
                FUN_100eeb30(DAT_10158e70,0x9e);
              }
              FUN_101bf3b2(param_1 + 0xec,0,0x20);
              param_1[0xf0] = uVar9;
              param_1[0xf1] = iVar6;
              uVar19 = uVar9;
              if ((uVar9 != 0) && (uVar19 = uVar9 - 1, (uVar9 & uVar19) != 0)) {
                uVar19 = 0;
              }
              param_1[0xf2] = uVar19;
              if ((param_1[2] & 3U) == 1) {
                FUN_1015abac(piVar18,2);
                iVar6 = FUN_10157e84(param_1);
                if (iVar6 == 0) {
                  piVar15 = param_1;
                  for (iVar6 = 0; iVar6 < param_1[0x2b]; iVar6 = iVar6 + 1) {
                    if ((*(char *)((int)piVar15 + 0xc5) == '\x05') &&
                       (*(char *)((int)piVar15 + 0xc6) == -2)) {
                      uVar8 = 3;
                      goto LAB_10158e9e;
                    }
                    piVar15 = (int *)((int)piVar15 + 0x25);
                  }
                  goto LAB_10158d56;
                }
                FUN_1014c818(DAT_10158e84 | uVar16,DAT_10158e88,iVar6);
                if (iVar6 == -5) goto LAB_10158d1e;
                if (iVar6 != -0xb) goto LAB_101585b6;
              }
              else {
                FUN_1015abac(piVar18,4);
                iVar6 = FUN_10157e84(param_1);
                if (iVar6 == 0) {
                  FUN_101576f8(param_1,4);
LAB_10158d56:
                  uVar8 = 10;
LAB_10158e9e:
                  FUN_1015abac(piVar18,uVar8);
                  FUN_1014c818(DAT_10158ecc | uVar16,DAT_10158ed0);
                  if ((param_1[2] & 2U) == 0) {
                    piVar18 = param_1;
                    for (iVar6 = 0; iVar6 < param_1[0x2b]; iVar6 = iVar6 + 1) {
                      if ((*(char *)((int)piVar18 + 0xc5) == '\x02') &&
                         (*(char *)((int)piVar18 + 0xc6) == '\x02')) {
                        piVar15 = piVar18 + 0x2e;
                        iVar13 = *(int *)((int)param_1 + iVar6 * 0x25 + 0xd9);
                        iVar7 = FUN_1015a600(piVar15);
                        if ((iVar13 != iVar7) ||
                           (iVar13 = *(int *)((int)param_1 + iVar6 * 0x25 + 0xd4),
                           iVar7 = FUN_1015a630(piVar15), iVar13 != iVar7)) {
                          FUN_1014c818(DAT_10158e8c | uVar16,DAT_10158e90,piVar15);
                          uVar8 = 1;
                          goto LAB_10158de0;
                        }
                      }
                      piVar18 = (int *)((int)piVar18 + 0x25);
                    }
                    uVar8 = 0;
LAB_10158de0:
                    FUN_1015a66c(uVar8);
                    FUN_1015a238(param_1 + 0x2e,0xf);
                    FUN_1015a1f0();
                  }
                  FUN_101592d4(param_1[5],0,1);
                  FUN_10159224(param_1[5],0x10001,1);
                  FUN_10157278(param_1,2);
                  iVar6 = 0;
                  *(undefined1 *)(DAT_10158e94 + 0x6d) = 1;
                }
                else {
                  FUN_1014c818(DAT_10158e7c | uVar16,DAT_10158e80,iVar6);
                  if (iVar6 == -5) {
LAB_10158d1e:
                    iVar6 = -5;
                  }
                  else if (iVar6 != -0xb) {
                    uVar8 = 0xf;
                    goto LAB_10158d44;
                  }
                }
              }
              goto LAB_101584c0;
            }
            uVar8 = 0x10000;
            uVar9 = DAT_10158e68;
            uVar11 = DAT_10158e60;
          }
        }
        FUN_1014c818(uVar9 | uVar16,uVar11,uVar8);
        FUN_1014c818(DAT_10158a3c | uVar16,DAT_10158a40,0xffffffea);
      }
      else {
        uVar8 = FUN_10157390(uVar8);
        FUN_1014c818(DAT_101586d0 | uVar16,DAT_101586d4,uVar19,uVar8);
LAB_10158626:
        FUN_1014c818(DAT_101586d8 | uVar16,DAT_101586dc);
        FUN_1015abac(piVar18,0xe);
      }
      iVar6 = -0x16;
    }
    else {
      FUN_1014c818(DAT_101586a4 | uVar16,DAT_101586a8);
LAB_101584bc:
      iVar6 = -0xb;
    }
  }
LAB_101584c0:
  if (param_1[0xd9] != 0) {
    FUN_101ce04a();
    param_1[0xd9] = 0;
  }
  if (param_1[0xda] != 0) {
    FUN_10188578();
    param_1[0xda] = 0;
    param_1[0xdb] = 0;
  }
  if (param_1[0xdc] != 0) {
    FUN_10188578();
    param_1[0xdc] = 0;
    param_1[0xdd] = 0;
  }
  if (param_1[0xde] != 0) {
    FUN_10188578();
    param_1[0xde] = 0;
    param_1[0xdf] = 0;
  }
  if (iVar6 != 0) {
    FUN_10159224(param_1[5],0x10001,0);
    FUN_10157278(param_1,3);
    if (iVar6 != -0xb) {
      FUN_101574a4(param_1);
    }
    FUN_1014c818(uVar16 | 0x7780031,DAT_101586ac);
    FUN_10157278(param_1,0);
  }
  if (param_1[9] != 0) {
    iVar7 = FUN_10157390(iVar5);
    if (iVar7 == 2) {
      FUN_10157764(iVar5);
    }
    FUN_101ce04a(param_1[9]);
    param_1[9] = 0;
  }
  if (((iVar6 == 0) && (iVar5 != 0)) && (iVar7 = FUN_10157390(iVar5), iVar7 == 2)) {
    FUN_101be1ec(500);
  }
  FUN_101598b4(param_1[5]);
  if (iVar6 == 0) {
    FUN_1014c818(DAT_101586b0 | uVar16,DAT_101586b4);
    if (param_1[5] != 0) {
      FUN_101599c0();
    }
    if (param_1[6] != 0) {
      FUN_10156f50();
      return 0;
    }
    return 0;
  }
LAB_1015842a:
  iVar7 = FUN_10157390(iVar5);
  uVar11 = DAT_101586f0;
  uVar8 = DAT_10158698;
  if (iVar7 == 2) {
    uVar16 = DAT_10158694 | uVar16;
    local_50 = 0;
    do {
      FUN_101573bc(iVar5,uVar11,&local_40);
      FUN_1014c818(uVar16,uVar8,local_40,local_50);
      if (local_40 != 2) goto LAB_1015847a;
      FUN_101be1ec(0x32);
      local_50 = local_50 + 1;
    } while (local_50 != 10);
    FUN_101573bc(iVar5,DAT_1015869c,0);
  }
LAB_1015847a:
  iVar5 = param_1[5];
  uVar8 = FUN_101d099c(iVar5);
  FUN_10159948(iVar5,uVar8);
  FUN_1015a1f0();
  return iVar6;
}


