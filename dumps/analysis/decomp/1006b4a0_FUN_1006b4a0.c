// entry 0x1006b4a0  FUN_1006b4a0  (1006b4a0)

int FUN_1006b4a0(int param_1,undefined4 param_2,int *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  int *piVar7;
  
  if (param_4 == 0) {
    return 0;
  }
  if (param_1 != 0x10c) {
    return 0;
  }
  piVar5 = *(int **)(param_4 + 0x10);
  uVar6 = (DAT_1006b728 - DAT_1006b72c) * 0x20 & 0xff00;
  FUN_1014c818(uVar6 | 0xef0034,DAT_1006b730,0x10c,param_2,piVar5,param_3,param_3);
  switch(param_2) {
  case 0x19:
    uVar1 = FUN_100dfb88();
    piVar7 = piVar5 + 3;
    iVar2 = FUN_10184804(DAT_1006b73c,piVar7,DAT_1006b738,DAT_1006b734,uVar1);
    if (iVar2 < 0) {
      FUN_1014c818(uVar6 | 0xaf0011,DAT_1006b740,DAT_1006b73c);
      iVar2 = -2;
      goto LAB_1006b518;
    }
    iVar2 = FUN_101848d4(piVar7,DAT_1006b748,piVar5 + 6,0,1);
    if (iVar2 < 0) {
      uVar3 = uVar6 | 0xb80000;
      uVar1 = DAT_1006b74c;
    }
    else {
      iVar2 = FUN_10184a5c(piVar7,DAT_1006b750,piVar5 + 9,1);
      if (-1 < iVar2) {
        if ((char)piVar5[0x16] == '\0') {
          iVar4 = FUN_100df9a0(piVar5 + 0xe,DAT_1006b764);
          if (-1 < iVar4) {
            *(undefined1 *)(piVar5 + 0x16) = 1;
            goto LAB_1006b578;
          }
          uVar3 = uVar6 | 0xc80011;
          uVar1 = DAT_1006b768;
        }
        else {
LAB_1006b578:
          uVar3 = uVar6 | 0xce0031;
          uVar1 = DAT_1006b758;
        }
        FUN_1014c818(uVar3,uVar1);
        if (iVar2 == 0) {
          piVar7 = piVar5 + 0xb;
          FUN_101b5982();
          lv_style_6(piVar7);
          FUN_101cc792(piVar7,2);
          FUN_101cc770(piVar7,0xffffffff);
          if ((char)piVar5[0x16] == '\0') {
            FUN_1014c818(uVar6 | 0x600011,DAT_1006b75c);
          }
          else {
            FUN_101cc78a(piVar7,piVar5 + 0xe);
          }
          *piVar5 = param_4;
          lv_obj_pos_29(param_4,0x1d2);
          FUN_101c7358(*piVar5,0);
          FUN_101c7378(*piVar5,0xff,0);
          FUN_101c5de4(*piVar5,9,0);
          lv_obj_13(*piVar5,0x8000);
          iVar2 = FUN_1013ed38(*piVar5);
          piVar5[1] = iVar2;
          lv_img_2(iVar2,piVar5 + 6);
          FUN_101c5de4(piVar5[1],2,0,0x80);
          uVar1 = FUN_10127cfc(*piVar5);
          FUN_101c5de4(uVar1,2,0,0xdc);
          lv_obj_pos_29(uVar1,0x188,0xb4);
          iVar2 = FUN_1005b578(uVar1);
          uVar1 = DAT_1006b76c;
          piVar5[2] = iVar2;
          lv_obj_pos_29(iVar2,0x188,uVar1);
          FUN_101c5de4(piVar5[2],9,0);
          FUN_1012bb14(piVar5[2],piVar7,0);
          FUN_101acba4(piVar5[2],0);
          FUN_1005b70c(piVar5[2],piVar5 + 9);
          FUN_1014c818(uVar6 | 0x830031,DAT_1006b770);
          return 0;
        }
        goto LAB_1006b518;
      }
      uVar3 = uVar6 | 0xc00000;
      uVar1 = DAT_1006b754;
    }
    FUN_1014c818(uVar3 | 0x11,uVar1);
LAB_1006b518:
    FUN_1014c818(uVar6 | 0x10c0013,DAT_1006b744,0x10c,0x19,iVar2);
    return iVar2;
  case 0x1a:
  case 0x1b:
    if (piVar5 == (int *)0x0) {
      FUN_101d779c(DAT_1006b73c);
    }
    else {
      if (*piVar5 != 0) {
        lv_obj_tree_16();
        *piVar5 = 0;
      }
      lv_style_7(piVar5 + 0xb);
      if ((char)piVar5[0x16] != '\0') {
        thunk_FUN_10185d3c(piVar5 + 0xe);
        *(undefined1 *)(piVar5 + 0x16) = 0;
      }
      FUN_101d775a(piVar5 + 6,1);
      FUN_101d777a(piVar5 + 9,1);
      FUN_101d7714(piVar5 + 3);
    }
    FUN_101d770e(DAT_1006b73c);
    break;
  case 0x1c:
    FUN_100df934(*piVar5);
    break;
  case 0x1d:
    if (*param_3 == DAT_1006b760) {
      *(undefined1 *)(param_3 + 1) = 1;
      FUN_100d6460(0x10c,0);
    }
  }
  return 0;
}


