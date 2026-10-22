// entry 0x100d587c  ui_view_creater_1  (100d587c)

uint ui_view_creater_1(byte *param_1,int param_2,int *param_3,uint param_4,int *param_5)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *puVar3;
  undefined1 uVar4;
  undefined4 uVar5;
  byte bVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  int *piVar10;
  int *piVar11;
  int *piVar12;
  
  if (param_3 == (int *)0x0) {
    return 0;
  }
  if ((param_2 == 0x10) && (param_4 == 5)) {
switchD_100d58de_caseD_5:
    uVar9 = FUN_101b617e(param_1,param_3 + 3);
    goto LAB_100d5922;
  }
  iVar8 = DAT_100d5b3c - DAT_100d5b38;
  uVar5 = FUN_100d5710(param_4);
  iVar7 = param_3[3];
  uVar9 = iVar8 * 0x20 & 0xff00;
  piVar11 = param_3;
  piVar12 = param_5;
  FUN_1014c818(DAT_100d5b40 | uVar9,DAT_100d5b44,param_2,param_1,uVar5,param_3,iVar7,param_5);
  puVar3 = DAT_100d5b64;
  if (0xd < param_4) {
    if (param_4 == 0x81) {
      piVar11 = (int *)param_3[3];
      if (piVar11 == (int *)0x0) {
        return 0;
      }
      if (piVar11[5] == 0) {
        return 0;
      }
      iVar7 = (**(code **)(param_1 + 0x24))(piVar11,1);
      if (-1 < iVar7) {
        if (*piVar11 == 0) {
LAB_100d5b00:
          if ((int)((uint)*param_1 << 0x1c) < 0) {
            iVar7 = FUN_1018c020(*(undefined2 *)(param_1 + 2));
            if (iVar7 != 0) {
              (**(code **)(param_1 + 0x24))(piVar11,0);
              return 0;
            }
            *(byte *)((int)piVar11 + 0x1a) = *(byte *)((int)piVar11 + 0x1a) | 1;
            return 0;
          }
          iVar7 = FUN_1018c020(*(undefined2 *)(param_1 + 2));
          if (iVar7 == 0) {
            FUN_1018bef8(*(undefined2 *)(param_1 + 2),1);
          }
          (**(code **)(param_1 + 0x24))(piVar11,0);
          if (iVar7 != 0) {
            return 0;
          }
          FUN_101c76a6(*param_3);
          FUN_1018bef8(*(undefined2 *)(param_1 + 2),0);
          return 0;
        }
        FUN_101d7714(piVar11 + 1);
        uVar5 = FUN_100dfb88();
        iVar7 = FUN_10184804(*piVar11,piVar11 + 1,DAT_100d5d28,DAT_100d5d24,uVar5);
        if (-1 < iVar7) goto LAB_100d5b00;
      }
LAB_100d5970:
      uVar9 = 0xfffffffe;
      goto LAB_100d5974;
    }
switchD_100d58de_caseD_6:
    iVar7 = param_3[3];
    if (iVar7 == 0) {
      return 0;
    }
    if (*(int *)(iVar7 + 0x14) == 0) {
      return 0;
    }
    if (*(code **)(param_1 + 0x2c) == (code *)0x0) {
      return 0;
    }
    uVar9 = (**(code **)(param_1 + 0x2c))(iVar7,param_4);
    goto LAB_100d5922;
  }
  if (param_4 < 2) goto switchD_100d58de_caseD_6;
  switch(param_4) {
  case 2:
    uVar9 = FUN_100d5600(param_1,param_3);
    goto LAB_100d5922;
  case 3:
    piVar11 = (int *)param_3[3];
    if (piVar11 == (int *)0x0) {
      if (-1 < (int)((uint)param_1[1] << 0x1f)) {
        return 0;
      }
      iVar7 = FUN_100d5600(param_1,param_3);
      if (iVar7 != 0) {
        uVar9 = 0xffffffff;
        goto LAB_100d5974;
      }
      piVar11 = (int *)param_3[3];
    }
    FUN_10127e14(*param_3);
    iVar7 = thunk_FUN_10146a3c(*(undefined2 *)(param_1 + 4),DAT_100d5b48);
    if (iVar7 == 0) {
      uVar9 = 0xfffffff4;
      goto LAB_100d5974;
    }
    uVar1 = *(undefined2 *)(param_1 + 4);
    piVar11[5] = iVar7;
    FUN_101bf3b2(iVar7,0,uVar1);
    if (*piVar11 != 0) {
      uVar5 = FUN_100dfb88();
      iVar7 = FUN_10184804(*piVar11,piVar11 + 1,DAT_100d5b58,DAT_100d5b54,uVar5);
      if (iVar7 < 0) goto LAB_100d5970;
    }
    iVar7 = FUN_100d56a4(param_1,piVar11);
    if (iVar7 == 0) {
      (**(code **)(param_1 + 0xc))(piVar11);
      (**(code **)(param_1 + 0x10))(piVar11);
      if (-1 < (int)((uint)*(byte *)((int)piVar11 + 0x1a) << 0x1e)) {
        return 0;
      }
      (**(code **)(param_1 + 0x14))(piVar11,1);
      return 0;
    }
    goto LAB_100d5970;
  case 4:
    piVar10 = (int *)param_3[3];
    if (piVar10 != (int *)0x0) {
      iVar8 = *piVar10;
      if (piVar10[5] != 0) {
        FUN_1014c818(uVar9 | 0x4e0033,DAT_100d5d2c,*(undefined2 *)(param_1 + 2),
                     (*(byte *)((int)piVar10 + 0x1a) & 7) >> 2,1,piVar11,iVar7,piVar12);
        (**(code **)(param_1 + 0x20))(piVar10);
        *(byte *)((int)piVar10 + 0x1a) = *(byte *)((int)piVar10 + 0x1a) & 0xfb;
        FUN_101ce04a(piVar10[5]);
        piVar10[5] = 0;
      }
      if ((*param_3 != 0) && (iVar7 = lv_disp_7(), iVar7 == 0)) {
        FUN_101ba194(DAT_100d5d38,DAT_100d5d30,DAT_100d5d34,0xea);
        FUN_101ba194(DAT_100d5d3c);
        FUN_100eeb30(DAT_100d5d34,0xea);
      }
      FUN_101d7714(piVar10 + 1);
      FUN_101ce04a(param_3[3]);
      param_3[3] = 0;
      if (iVar8 != 0) {
        FUN_101d779c(iVar8);
        FUN_101d770e(iVar8);
      }
      if ((*DAT_100d5d40 != 0) && (*(short *)(*DAT_100d5d40 + 2) == *(short *)(param_1 + 2))) {
        *DAT_100d5d40 = 0;
        *DAT_100d5d44 = 0;
        return 0;
      }
    }
    break;
  case 5:
    goto switchD_100d58de_caseD_5;
  default:
    goto switchD_100d58de_caseD_6;
  case 7:
    piVar10 = (int *)param_3[3];
    FUN_1014c818(uVar9 | 0x9d0033,DAT_100d5b5c,1,*(undefined2 *)(param_1 + 2),*DAT_100d5b64,piVar11,
                 iVar7,piVar12);
    if (piVar10 != (int *)0x0) {
      bVar6 = *(byte *)((int)piVar10 + 0x1a);
      *puVar3 = piVar10;
      *(byte *)((int)piVar10 + 0x1a) = bVar6 | 2;
      *DAT_100d5b60 = param_1;
      if ((*(short *)(param_1 + 2) == 0x10) && (iVar7 = FUN_100d67cc(), iVar7 != 0)) {
        lv_disp_7(*param_3);
        lv_obj_pos_42();
      }
      bVar6 = *param_1;
      if ((bVar6 & 8) == 0) {
        if (piVar10[5] != 0) {
          FUN_101b6136(param_1,param_3[3]);
          return bVar6 & 8;
        }
      }
      else if ((*piVar10 != 0) && (iVar7 = thunk_FUN_10186b10(), iVar7 == 0)) {
        FUN_10185810(*piVar10,*(undefined2 *)(param_1 + 2),1,0);
        return 0;
      }
    }
    break;
  case 8:
    piVar10 = (int *)param_3[3];
    uVar5 = *DAT_100d5b64;
    FUN_1014c818(uVar9 | 0x9d0033,DAT_100d5b5c,0,*(undefined2 *)(param_1 + 2),uVar5,piVar11,iVar7,
                 piVar12);
    if (piVar10 != (int *)0x0) {
      *(byte *)((int)piVar10 + 0x1a) = *(byte *)((int)piVar10 + 0x1a) & 0xfd;
      FUN_100e01b8(0);
      if (piVar10[5] != 0) {
        (**(code **)(param_1 + 0x14))(piVar10,0);
        if (-1 < (int)((uint)*param_1 << 0x1c)) {
          return 0;
        }
        bVar6 = *(byte *)((int)piVar10 + 0x1a) & 0xfb;
        *(byte *)((int)piVar10 + 0x1a) = bVar6;
        FUN_1014c818(uVar9 | 0xcb0031,DAT_100d5b68,*(undefined2 *)(param_1 + 2),bVar6,uVar5,piVar11,
                     iVar7,piVar12);
      }
      if (((int)((uint)*param_1 << 0x1c) < 0) && (*piVar10 != 0)) {
        FUN_101d779c();
        FUN_101d770e(*piVar10);
      }
    }
    break;
  case 0xb:
    uVar9 = FUN_101b6136(param_1,param_3[3]);
LAB_100d5922:
    if (uVar9 != 0) {
LAB_100d5974:
      FUN_1014c818(DAT_100d5b4c | (DAT_100d5b3c - DAT_100d5b38) * 0x20 & 0xff00U,DAT_100d5b50,uVar9)
      ;
      return uVar9;
    }
    break;
  case 0xc:
    if ((param_3[3] != 0) && (*(int *)(param_3[3] + 0x14) != 0)) {
      FUN_101b617e(param_1,param_3 + 3);
      FUN_101c76a6(*param_3);
      FUN_10101b2c(1000);
    }
    break;
  case 0xd:
    if (((param_3[3] != 0) && (*(int *)(param_3[3] + 0x14) != 0)) && (param_5 != (int *)0x0)) {
      if (*param_5 == DAT_100d5d48) {
        FUN_1018e3e0();
      }
      FUN_1014c818(DAT_100d5d50 | uVar9,DAT_100d5d54,*(undefined2 *)(param_1 + 2),*param_5,
                   *DAT_100d5d58,*DAT_100d5d4c,iVar7,piVar12);
      uVar4 = (**(code **)(param_1 + 0x28))(param_3[3],*param_5);
      sVar2 = *(short *)(param_1 + 2);
      *(undefined1 *)(param_5 + 1) = uVar4;
      if (((sVar2 != 0x3e) && (sVar2 != 0xe7)) &&
         ((iVar7 = FUN_100d6770(), iVar7 == 0 && (iVar7 = FUN_1018ef08(), 0 < iVar7)))) {
        FUN_1014c818(DAT_100d5d5c | uVar9,DAT_100d5d60);
        uVar5 = DAT_100d5d60;
        do {
          FUN_10182298();
          iVar7 = FUN_1018ef08();
          FUN_1014c818(uVar9 | 0x1700031,uVar5,iVar7);
        } while (0 < iVar7);
      }
    }
  }
  return 0;
}


