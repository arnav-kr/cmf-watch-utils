// entry 0x100dd5ac  FUN_100dd5ac

/* WARNING: Type propagation algorithm not settling */

undefined4 FUN_100dd5ac(int param_1)

{
  byte bVar1;
  undefined2 uVar2;
  short sVar3;
  byte bVar4;
  bool bVar5;
  undefined4 uVar6;
  int iVar7;
  uint *puVar8;
  int iVar9;
  uint uVar10;
  undefined4 *puVar11;
  uint uVar12;
  undefined4 uVar13;
  int iVar14;
  int *piVar15;
  int *piVar16;
  int *piVar17;
  uint uVar18;
  undefined4 *puVar19;
  uint local_64;
  int local_58 [11];
  
  puVar19 = DAT_100dd864;
  if (DAT_100dd860[0xb] == 1) {
    FUN_1009a1b4(*DAT_100dd864);
  }
  else if (((int)((uint)*(byte *)(DAT_100dd864 + 0x13) << 0x1f) < 0) &&
          (local_64 = *DAT_100dd860 & 0x20, (*DAT_100dd860 & 0x20) == 0)) {
    bVar4 = DAT_100dd860[9];
    if (param_1 == 0) {
      FUN_100db55c(DAT_100dd864[0xc] + 0x24,DAT_100dd864 + 0xd,local_64);
      uVar13 = puVar19[0x11];
      piVar15 = (int *)puVar19[0xd];
      puVar19 = puVar19 + 0xf;
    }
    else {
      FUN_100db55c(DAT_100dd864[0xc] + 0x24,DAT_100dd864 + 0xe,1);
      uVar13 = puVar19[0x12];
      piVar15 = (int *)puVar19[0xe];
      puVar19 = puVar19 + 0x10;
    }
    FUN_101bf3b2(local_58,0,0x28);
    uVar6 = DAT_100dd86c;
    bVar5 = false;
LAB_100dd61a:
    piVar16 = piVar15;
    if (piVar16 != (int *)0x0) {
      piVar15 = (int *)*piVar16;
      uVar18 = *(byte *)((int)piVar16 + 6) & 0x3f;
      if (uVar18 - 1 < 2) {
        piVar17 = (int *)piVar16[0x1f];
        if (piVar17 == (int *)0x0) {
          piVar17 = (int *)FUN_10188234(1,0xc,uVar6);
          if ((piVar17 == (int *)0x0) || (iVar7 = FUN_1005a014(uVar13), iVar7 == 0))
          goto LAB_100dd644;
          lv_obj_pos_29(iVar7,DAT_100dd868);
          lv_obj_23(iVar7,0x4000);
          puVar8 = (uint *)FUN_10188234(1,0xc,uVar6);
          if (puVar8 == (uint *)0x0) goto LAB_100dd64a;
          *puVar8 = 0;
          puVar8[1] = 0;
          puVar8[2] = 0;
          iVar9 = 0;
          if (((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) &&
             (iVar9 = WFaceRes_1(piVar16[9],puVar19), iVar9 != 0)) {
            *puVar8 = *puVar8 & 0xffe003ff |
                      ((*(byte *)(iVar9 + 0x1a) & 0x1f) << 6 | (uint)(*(byte *)(iVar9 + 0x19) >> 2))
                      << 10;
            bVar1 = *(byte *)(iVar9 + 0x18);
            *(ushort *)((int)puVar8 + 2) =
                 *(ushort *)((int)puVar8 + 2) & 0x1f |
                 ((ushort)*(byte *)(iVar9 + 0x1b) << 3 | (ushort)(*(byte *)(iVar9 + 0x1a) >> 5)) <<
                 5;
            *(byte *)puVar8 = (byte)*puVar8 & 0xe0 | (byte)(((uint)bVar1 << 0x1b) >> 0x1b);
            puVar8[1] = *(uint *)(iVar9 + 0xc);
            puVar8[2] = *(uint *)(iVar9 + 0x10);
          }
          bVar5 = true;
          piVar17[2] = iVar9;
          *piVar17 = iVar7;
          piVar17[1] = (int)puVar8;
          piVar16[0x1f] = (int)piVar17;
          FUN_101b91f6(iVar7,piVar16 + 6,bVar4,DAT_100dd864[0x14]);
        }
        else {
          iVar7 = *piVar17;
          puVar8 = (uint *)piVar17[1];
          if (((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) &&
             ((iVar9 = piVar17[2], iVar9 == 0 || (piVar16[9] != *(int *)(iVar9 + 4))))) {
            FUN_100dbbc0(iVar9,puVar19);
            puVar11 = (undefined4 *)piVar17[1];
            piVar17[2] = 0;
            *puVar11 = 0;
            puVar11[1] = 0;
            puVar11[2] = 0;
            iVar9 = WFaceRes_1(piVar16[9],puVar19);
            if (iVar9 != 0) {
              *puVar8 = *puVar8 & 0xffe003ff |
                        ((*(byte *)(iVar9 + 0x1a) & 0x1f) << 6 |
                        (uint)(*(byte *)(iVar9 + 0x19) >> 2)) << 10;
              bVar1 = *(byte *)(iVar9 + 0x18);
              *(ushort *)((int)puVar8 + 2) =
                   *(ushort *)((int)puVar8 + 2) & 0x1f |
                   ((ushort)*(byte *)(iVar9 + 0x1b) << 3 | (ushort)(*(byte *)(iVar9 + 0x1a) >> 5))
                   << 5;
              *(byte *)puVar8 = (byte)*puVar8 & 0xe0 | (byte)(((uint)bVar1 << 0x1b) >> 0x1b);
              puVar8[1] = *(uint *)(iVar9 + 0xc);
              puVar8[2] = *(uint *)(iVar9 + 0x10);
              piVar17[2] = iVar9;
            }
          }
          if ((*DAT_100dd860 & 0x12) == 2) {
            FUN_101b91f6(*piVar17,piVar16 + 6,bVar4,DAT_100dd864[0x14]);
          }
        }
        if (*(char *)((int)piVar16 + 6) < '\0') {
          if ((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) {
            if (iVar7 != 0) {
              FUN_100dc5e4((uint)*(ushort *)(piVar16 + 2) * 10);
              lv_obj_13(iVar7,1);
              simple_img_6(iVar7,puVar8);
              if (((*(byte *)((int)piVar16 + 6) & 0x3f) == 2) &&
                 ((*(byte *)(piVar16 + 0xb) & 0x30) == 0)) {
                simple_img_7(iVar7,(int)(short)piVar16[0xd],(int)*(short *)((int)piVar16 + 0x36));
                simple_img_8(iVar7,(int)-*(short *)((int)piVar16 + 0x2a));
              }
              lv_obj_pos_26(iVar7,(int)(short)piVar16[4] - ((int)(short)piVar16[4] >> 0x1f) &
                                  0xfffffffe,(int)*(short *)((int)piVar16 + 0x12));
              goto LAB_100dd792;
            }
          }
          else if (iVar7 != 0) {
            lv_obj_23(iVar7,1);
          }
          FUN_100dbbc0(piVar17[2],puVar19);
          piVar17[2] = 0;
        }
LAB_100dd792:
        bVar1 = *(byte *)((int)piVar16 + 7);
joined_r0x100dd796:
        if ((((int)((uint)bVar1 << 0x1f) < 0) && (iVar7 != 0)) && (bVar5)) {
LAB_100dd7ac:
          lv_obj_tree_17(iVar7);
        }
        goto LAB_100dd61a;
      }
      if (uVar18 == 7) {
        if ((int *)piVar16[0x1f] == (int *)0x0) {
          piVar17 = (int *)FUN_10188234(1,0xc,uVar6);
          if ((piVar17 == (int *)0x0) || (iVar7 = FUN_10127cfc(uVar13), iVar7 == 0))
          goto LAB_100dd644;
          lv_obj_pos_29(iVar7,(short)piVar16[5],*(undefined2 *)((int)piVar16 + 0x16));
          lv_obj_23(iVar7,2);
          lv_obj_pos_26(iVar7,(int)(short)piVar16[4],(int)*(short *)((int)piVar16 + 0x12));
          lv_event_11(iVar7,DAT_100dda64);
          bVar5 = true;
          piVar17[1] = 0;
          piVar17[2] = 0;
          *piVar17 = iVar7;
          piVar16[0x1f] = (int)piVar17;
        }
        else {
          iVar7 = *(int *)piVar16[0x1f];
        }
        if (*(char *)((int)piVar16 + 6) < '\0') {
          if (-1 < (int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f)) {
            if (iVar7 != 0) {
              lv_obj_23(iVar7,1);
              goto LAB_100dd8fe;
            }
            goto LAB_100dd61a;
          }
          if (iVar7 == 0) goto LAB_100dd61a;
          lv_obj_13(iVar7,1);
        }
      }
      else {
        if (uVar18 == 8) {
          if ((int *)piVar16[0x1f] == (int *)0x0) {
            piVar17 = (int *)FUN_10188234(1,0xc,uVar6);
            if ((piVar17 == (int *)0x0) || (iVar7 = FUN_1005a014(uVar13), iVar7 == 0))
            goto LAB_100dd644;
            puVar8 = (uint *)FUN_10188234(1,0xc,uVar6);
            if (puVar8 == (uint *)0x0) goto LAB_100dd64a;
            *puVar8 = 0;
            puVar8[1] = 0;
            puVar8[2] = 0;
            iVar9 = WFaceRes_1(piVar16[6],puVar19);
            if (iVar9 != 0) {
              *puVar8 = *puVar8 & 0xffe003ff |
                        ((*(byte *)(iVar9 + 0x1a) & 0x1f) << 6 |
                        (uint)(*(byte *)(iVar9 + 0x19) >> 2)) << 10;
              bVar1 = *(byte *)(iVar9 + 0x18);
              *(ushort *)((int)puVar8 + 2) =
                   *(ushort *)((int)puVar8 + 2) & 0x1f |
                   ((ushort)*(byte *)(iVar9 + 0x1b) << 3 | (ushort)(*(byte *)(iVar9 + 0x1a) >> 5))
                   << 5;
              *(byte *)puVar8 = (byte)*puVar8 & 0xe0 | bVar1 & 0x1f;
              puVar8[1] = *(uint *)(iVar9 + 0xc);
              puVar8[2] = *(uint *)(iVar9 + 0x10);
            }
            piVar17[2] = iVar9;
            simple_img_6(iVar7,puVar8);
            lv_obj_23(iVar7,2);
            *(int **)(iVar7 + 0x10) = piVar16 + 4;
            lv_obj_pos_26(iVar7,(int)(short)piVar16[4],(int)*(short *)((int)piVar16 + 0x12));
            lv_event_11(iVar7,DAT_100dda68,4,piVar17);
            *piVar17 = iVar7;
            piVar17[1] = (int)puVar8;
            bVar5 = true;
            piVar16[0x1f] = (int)piVar17;
          }
          else {
            iVar7 = *(int *)piVar16[0x1f];
          }
          if (-1 < *(char *)((int)piVar16 + 6)) goto LAB_100dda2a;
          if (-1 < (int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f)) goto LAB_100dda54;
          if (iVar7 != 0) {
            lv_obj_13(iVar7,1);
            goto LAB_100dda2a;
          }
          goto LAB_100dd61a;
        }
        if (uVar18 != 4) {
          if (uVar18 == 6) {
            piVar17 = (int *)piVar16[0x1f];
            if (piVar17 == (int *)0x0) {
              if ((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) {
                piVar17 = (int *)FUN_10188234(1,0x30,uVar6);
                if ((piVar17 == (int *)0x0) || (iVar7 = FUN_10127cfc(uVar13), iVar7 == 0))
                goto LAB_100dd644;
                bVar5 = true;
                lv_obj_23(iVar7,0x4000);
                lv_obj_13(iVar7,0x10);
                *piVar17 = iVar7;
                piVar16[0x1f] = (int)piVar17;
              }
              else {
                iVar7 = 0;
              }
            }
            else {
              iVar7 = *piVar17;
            }
            if (*(char *)((int)piVar16 + 6) < '\0') {
              if ((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) {
                if (iVar7 != 0) {
                  *(short *)(piVar17 + 1) = (short)piVar16[10];
                  *(undefined2 *)((int)piVar17 + 6) = *(undefined2 *)((int)piVar16 + 0x2a);
                  *(short *)(piVar17 + 2) = (short)piVar16[0xb];
                  *(undefined2 *)((int)piVar17 + 10) = *(undefined2 *)((int)piVar16 + 0x2e);
                  *(short *)(piVar17 + 3) = (short)piVar16[0xc];
                  *(undefined2 *)((int)piVar17 + 0xe) = *(undefined2 *)((int)piVar16 + 0x32);
                  *(short *)(piVar17 + 4) = (short)piVar16[0xd];
                  *(undefined2 *)((int)piVar17 + 0x12) = *(undefined2 *)((int)piVar16 + 0x36);
                  *(short *)(piVar17 + 5) = (short)piVar16[0xe];
                  *(undefined2 *)((int)piVar17 + 0x16) = *(undefined2 *)((int)piVar16 + 0x3a);
                  *(char *)(piVar17 + 6) = (char)piVar16[0xf];
                  piVar17[7] = piVar16[9];
                  piVar17[10] = *(int *)((int)piVar16 + 0x3d);
                  lv_obj_13(iVar7,1);
                  if (((char)piVar17[6] == -0x78) && ((short)piVar17[1] == 2)) {
                    if (-1 < (int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f)) {
                      FUN_101b9824(piVar17);
                    }
                    if ((((piVar15 != (int *)0x0) && (piVar15[0x1f] != 0)) &&
                        ((*(ushort *)((int)piVar15 + 6) & 0x13f) == 6)) &&
                       (*(char *)(piVar15[0x1f] + 0x18) == -0x78)) {
                      FUN_101b9824();
                    }
                  }
                  if (*(int *)(iVar7 + 8) == 0) {
                    uVar2 = *(undefined2 *)((int)piVar16 + 0x16);
                    iVar9 = piVar16[5];
                    piVar17[0xb] = 0;
                    piVar17[9] = 0;
                    lv_obj_pos_29(iVar7,(short)iVar9,uVar2);
                    lv_obj_pos_26(iVar7,(int)(short)piVar16[4] - ((int)(short)piVar16[4] >> 0x1f) &
                                        0xfffffffe,(int)*(short *)((int)piVar16 + 0x12));
                    FUN_101c7358(iVar7,(uint)(*(byte *)((int)piVar16 + 0x1a) >> 3) |
                                       (uint)(*(byte *)((int)piVar16 + 0x19) >> 2) << 5 |
                                       (uint)(*(byte *)(piVar16 + 6) >> 3) << 0xb,0);
                    FUN_101c7378(iVar7,(char)piVar16[7],0);
                    iVar9 = 0;
                    while( true ) {
                      sVar3 = *(short *)(iVar9 * 0x18 + DAT_100dde80);
                      if ((short)piVar17[1] == sVar3) break;
                      if (sVar3 == 0xff) goto LAB_100dde0a;
                      iVar9 = iVar9 + 1;
                    }
                    (**(code **)(iVar9 * 0x18 + DAT_100dde80 + 4))(piVar17);
                  }
                  else {
                    FUN_100dc5e4(*(undefined2 *)((int)piVar17 + 6));
                    lv_obj_13(iVar7,1);
                    FUN_100dc38c((short)piVar17[1],piVar17);
                    iVar9 = FUN_101cb89a();
                    piVar17[8] = iVar9;
                  }
LAB_100dde0a:
                  bVar5 = true;
                }
              }
              else if (iVar7 != 0) {
                lv_obj_23(iVar7,1);
                FUN_100dc38c((short)piVar17[1],piVar17);
              }
            }
            bVar1 = *(byte *)((int)piVar16 + 7);
            goto joined_r0x100dd796;
          }
          goto LAB_100dd61a;
        }
        piVar17 = (int *)piVar16[0x1f];
        if (piVar17 == (int *)0x0) {
          piVar17 = (int *)FUN_10188234(1,0xc,uVar6);
          if ((piVar17 == (int *)0x0) || (iVar7 = FUN_1013ba54(uVar13), iVar7 == 0))
          goto LAB_100dd644;
          lv_obj_23(iVar7,0x4000);
          FUN_101c745c(iVar7,*(byte *)((int)piVar16 + 0x39) & 1,0);
          FUN_101c7466(iVar7,(*(byte *)((int)piVar16 + 0x39) & 3) >> 1,0);
          FUN_101c7448(iVar7,*(undefined2 *)((int)piVar16 + 0x35),0);
          lv_obj_pos_29(iVar7,(short)piVar16[5],*(undefined2 *)((int)piVar16 + 0x16));
          lv_obj_pos_26(iVar7,(int)(short)piVar16[4],(int)*(short *)((int)piVar16 + 0x12));
          puVar8 = (uint *)FUN_10188234(1,0xc,uVar6);
          if (puVar8 == (uint *)0x0) goto LAB_100dd64a;
          *puVar8 = 0;
          puVar8[1] = 0;
          puVar8[2] = 0;
          iVar9 = 0;
          if ((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) {
            if (piVar16[9] == 0) {
              FUN_101b926a(iVar7,piVar16 + 6,bVar4,*(undefined4 *)(DAT_100dde78 + 0x50));
            }
            else {
              iVar9 = WFaceRes_1(piVar16[9],puVar19);
              if (iVar9 != 0) {
                *puVar8 = *puVar8 & 0xffe003ff |
                          ((*(byte *)(iVar9 + 0x1a) & 0x1f) << 6 |
                          (uint)(*(byte *)(iVar9 + 0x19) >> 2)) << 10;
                bVar1 = *(byte *)(iVar9 + 0x18);
                *(ushort *)((int)puVar8 + 2) =
                     *(ushort *)((int)puVar8 + 2) & 0x1f |
                     ((ushort)*(byte *)(iVar9 + 0x1b) << 3 | (ushort)(*(byte *)(iVar9 + 0x1a) >> 5))
                     << 5;
                *(byte *)puVar8 = (byte)*puVar8 & 0xe0 | (byte)(((uint)bVar1 << 0x1b) >> 0x1b);
                puVar8[1] = *(uint *)(iVar9 + 0xc);
                puVar8[2] = *(uint *)(iVar9 + 0x10);
              }
              FUN_101c749a(iVar7,puVar8,0);
              uVar18 = (*(byte *)((int)piVar16 + 0x39) & 0x3f) >> 4;
              bVar1 = *(byte *)((int)piVar16 + 0x39) >> 6;
              if (uVar18 == 1) {
                if (bVar1 == 1) {
                  uVar18 = 1;
                }
                else if (bVar1 == 3) {
                  uVar18 = 4;
                }
                else {
                  if (bVar1 != 2) goto LAB_100dde72;
                  uVar18 = 7;
                }
              }
              else if (uVar18 == 2) {
                if (bVar1 != 1) {
                  if (bVar1 != 3) goto LAB_100dde72;
                  uVar18 = 5;
                }
              }
              else if (uVar18 == 3) {
                if (bVar1 != 1) {
                  if (bVar1 == 3) {
                    uVar18 = 6;
                  }
                  else {
                    if (bVar1 != 2) goto LAB_100dde72;
                    uVar18 = 8;
                  }
                }
              }
              else {
LAB_100dde72:
                uVar18 = 0;
              }
              FUN_101c73ac(iVar7,uVar18,0);
              FUN_101b91f6(iVar7,piVar16 + 6,bVar4,*(undefined4 *)(DAT_100dde78 + 0x50));
            }
          }
          *piVar17 = iVar7;
          piVar17[1] = (int)puVar8;
          piVar17[2] = iVar9;
          bVar5 = true;
          piVar16[0x1f] = (int)piVar17;
        }
        else {
          iVar7 = *piVar17;
          if ((*DAT_100dde7c & 0x12) == 2) {
            if (piVar16[9] == 0) {
              FUN_101b926a(iVar7,piVar16 + 6,bVar4,*(undefined4 *)(DAT_100dde78 + 0x50));
            }
            else {
              FUN_101b91f6(iVar7,piVar16 + 6,bVar4);
            }
          }
        }
        if (*(char *)((int)piVar16 + 6) < '\0') {
          if ((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) {
            if (iVar7 != 0) {
              FUN_100dc5e4((uint)*(ushort *)(piVar16 + 2) * 10);
              lv_obj_13(iVar7,1);
              iVar9 = (int)*(short *)((int)piVar16 + 0x31) / 10;
              iVar14 = (int)*(short *)((int)piVar16 + 0x33) / 10;
              uVar18 = -iVar9 & 0xffff;
              uVar12 = -iVar14 & 0xffff;
              if (((int)(uVar12 - uVar18) < 0x168) || (uVar10 = 0, uVar18 != 0)) {
                uVar10 = 0x10eU - iVar9 & 0xffff;
                uVar12 = 0x10eU - iVar14 & 0xffff;
              }
              lv_arc_16(iVar7,uVar10,uVar12);
              goto LAB_100dd8fe;
            }
          }
          else if (iVar7 != 0) {
            lv_obj_23(iVar7,1);
          }
          FUN_100dbbc0(piVar17[2],puVar19);
          piVar17[2] = 0;
        }
      }
LAB_100dd8fe:
      if ((((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) && (iVar7 != 0)) && (bVar5))
      goto LAB_100dd7ac;
      goto LAB_100dd61a;
    }
LAB_100dd644:
    if (bVar5) {
LAB_100dd64a:
      for (uVar18 = 0; uVar18 < local_64; uVar18 = uVar18 + 1) {
        lv_obj_tree_17(local_58[uVar18]);
      }
    }
  }
  return 0;
LAB_100dda54:
  if (iVar7 != 0) {
    lv_obj_23(iVar7,1);
LAB_100dda2a:
    if ((((int)((uint)*(byte *)((int)piVar16 + 7) << 0x1f) < 0) && (iVar7 != 0)) && (local_64 < 10))
    {
      local_58[local_64] = iVar7;
      local_64 = local_64 + 1 & 0xff;
    }
  }
  goto LAB_100dd61a;
}


