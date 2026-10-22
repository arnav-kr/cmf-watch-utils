// entry 0x1006bd18  FUN_1006bd18

int FUN_1006bd18(int param_1,undefined4 param_2,int *param_3,int param_4)

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
  if (param_1 != 0x11a) {
    return 0;
  }
  piVar5 = *(int **)(param_4 + 0x10);
  uVar6 = (DAT_1006bf70 - DAT_1006bf74) * 0x20 & 0xff00;
  FUN_1014c818(uVar6 | 0xe30034,DAT_1006bf78,0x11a,param_2,piVar5,param_3,param_3);
  switch(param_2) {
  case 0x19:
    uVar1 = FUN_100dfb88();
    piVar7 = piVar5 + 3;
    iVar2 = FUN_10184804(DAT_1006bf84,piVar7,DAT_1006bf80,DAT_1006bf7c,uVar1);
    if (iVar2 < 0) {
      FUN_1014c818(uVar6 | 0xa30011,DAT_1006bf88,DAT_1006bf84);
      iVar2 = -2;
      goto LAB_1006bd90;
    }
    iVar2 = FUN_101848d4(piVar7,DAT_1006bf90,piVar5 + 6,0,1);
    if (iVar2 < 0) {
      uVar3 = uVar6 | 0xac0000;
      uVar1 = DAT_1006bf94;
    }
    else {
      iVar2 = FUN_10184a5c(piVar7,DAT_1006bf98,piVar5 + 9,1);
      if (-1 < iVar2) {
        if ((char)piVar5[0x16] == '\0') {
          iVar4 = FUN_100df9a0(piVar5 + 0xe,DAT_1006bfb0);
          if (-1 < iVar4) {
            *(undefined1 *)(piVar5 + 0x16) = 1;
            goto LAB_1006bdf2;
          }
          uVar3 = uVar6 | 0xbc0011;
          uVar1 = DAT_1006bfb4;
        }
        else {
LAB_1006bdf2:
          uVar3 = uVar6 | 0xc20031;
          uVar1 = DAT_1006bfa0;
        }
        FUN_1014c818(uVar3,uVar1);
        if (iVar2 == 0) {
          piVar7 = piVar5 + 0xb;
          lv_style_6(piVar7);
          FUN_101cc792(piVar7,2);
          FUN_101cc770(piVar7,0xffffffff);
          FUN_101cc782(piVar7,0xff);
          if ((char)piVar5[0x16] == '\0') {
            FUN_1014c818(uVar6 | 0x620011,DAT_1006bfb8);
          }
          else {
            FUN_101cc78a(piVar7,piVar5 + 0xe);
          }
          *piVar5 = param_4;
          iVar2 = FUN_1013ed38(param_4);
          piVar5[1] = iVar2;
          lv_img_2(iVar2,piVar5 + 6);
          FUN_101c5de4(piVar5[1],2,0,0x80);
          uVar1 = FUN_10127cfc(*piVar5);
          FUN_101c5de4(uVar1,2,0,200);
          lv_obj_pos_29(uVar1,0x188,0xb4);
          iVar2 = FUN_1005b578(uVar1);
          uVar1 = DAT_1006bfa4;
          piVar5[2] = iVar2;
          lv_obj_pos_29(iVar2,0x188,uVar1);
          FUN_101c5de4(piVar5[2],9,0);
          FUN_1012bb14(piVar5[2],piVar7,0);
          FUN_101acba4(piVar5[2],0);
          FUN_1005b70c(piVar5[2],piVar5 + 9);
          FUN_1014c818(uVar6 | 0x7c0031,DAT_1006bfa8);
          return 0;
        }
        goto LAB_1006bd90;
      }
      uVar3 = uVar6 | 0xb40000;
      uVar1 = DAT_1006bf9c;
    }
    FUN_1014c818(uVar3 | 0x11,uVar1);
LAB_1006bd90:
    FUN_1014c818(uVar6 | 0x1000013,DAT_1006bf8c,0x11a,0x19,iVar2);
    return iVar2;
  case 0x1a:
  case 0x1b:
    if (piVar5 == (int *)0x0) {
      FUN_101d779c(DAT_1006bf84);
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
    }
    FUN_101d770e(DAT_1006bf84);
    break;
  case 0x1c:
    FUN_100df934(*piVar5);
    break;
  case 0x1d:
    if (*param_3 == DAT_1006bfac) {
      *(undefined1 *)(param_3 + 1) = 1;
      FUN_100d6460(0x11a,0);
    }
  }
  return 0;
}


