// 0x1021c518 FUN_100b15dc :: wf_idx: %d, %d\n
// entry 0x100b15dc  FUN_100b15dc

void FUN_100b15dc(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  uint *puVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  
  iVar3 = FUN_101c4ca6();
  if ((((iVar3 != 0) && (*(int *)(iVar3 + 4) != 0)) && (*(char *)(iVar3 + 0x158) != '\0')) &&
     (-1 < (int)((uint)*(byte *)(iVar3 + 0x15a) << 0x1e))) {
    iVar4 = FUN_101c4c9a(param_1);
    if (iVar4 == 9) {
      iVar3 = *(int *)(iVar3 + *(char *)(iVar3 + 0x159) * 4 + 0x24);
      if (iVar3 != 0) {
        uVar5 = lv_obj_tree_4(iVar3);
        if (1 < uVar5) {
          uVar6 = lv_obj_tree_3(iVar3,1);
          lv_obj_11(uVar6,1);
        }
        iVar4 = lv_obj_tree_4(iVar3);
        if (iVar4 == 3) {
          uVar6 = lv_obj_tree_3(iVar3,2);
          lv_obj_11(uVar6,1);
          return;
        }
      }
    }
    else if (iVar4 == 10) {
      if (*(int *)(param_1 + 0x10) == 0) {
        iVar4 = FUN_101c650a(*(undefined4 *)(iVar3 + 4));
        cVar1 = (char)(iVar4 / 0x118);
        FUN_1014c818(((int)PTR_DAT_100b1788 - (int)PTR_LAB_100b1784) * 0x20 & 0xff00U | 0x1180032,
                     PTR_s_wf_idx___d___d_100b178c,iVar4 / 0x118,iVar4);
        *(char *)(iVar3 + 0x159) = cVar1;
        iVar3 = *(int *)(iVar3 + cVar1 * 4 + 0x24);
        if (iVar3 != 0) {
          uVar5 = lv_obj_tree_4(iVar3);
          if (1 < uVar5) {
            uVar6 = lv_obj_tree_3(iVar3,1);
            lv_obj_1(uVar6,1);
          }
          iVar4 = lv_obj_tree_4(iVar3);
          if (iVar4 == 3) {
            uVar6 = lv_obj_tree_3(iVar3,2);
            lv_obj_1(uVar6,1);
            return;
          }
        }
      }
    }
    else if ((iVar4 == 0xb) &&
            (iVar7 = FUN_101c650a(*(undefined4 *)(iVar3 + 4)), iVar2 = DAT_100b1794,
            iVar4 = DAT_100b1790, -1 < iVar7)) {
      uVar5 = iVar7 / 0x118;
      uVar11 = uVar5 & 0xffff;
      if (uVar11 < *(byte *)(iVar3 + 0x158)) {
        if (uVar11 == 0) {
          uVar12 = 0;
        }
        else {
          uVar12 = uVar5 - 1 & 0xffff;
        }
        if ((int)(uVar5 & 0xffff) < (int)(*(byte *)(iVar3 + 0x158) - 1)) {
          uVar11 = uVar5 + 1 & 0xffff;
        }
        for (; uVar12 <= uVar11; uVar12 = uVar12 + 1) {
          iVar13 = *(int *)(iVar3 + 0x24 + uVar12 * 4);
          if ((iVar13 != 0) && (iVar8 = lv_obj_tree_4(iVar13), iVar8 != 0)) {
            uVar6 = lv_obj_tree_3(iVar13,0);
            puVar9 = (uint *)simple_img_4();
            if (puVar9 != (uint *)0x0) {
              uVar5 = (*puVar9 & 0x1fffff) >> 10;
              uVar10 = 0x10e00 / uVar5;
              iVar13 = uVar12 * 0x118 - iVar7;
              if (iVar13 < 0) {
                iVar13 = -iVar13;
              }
              if (0x117 < iVar13) {
                iVar13 = 0x118;
              }
              simple_img_3(uVar6,(int)((iVar4 / (int)uVar5 + uVar10) * iVar13) / iVar2 + uVar10 &
                                 0xffff);
            }
          }
        }
      }
    }
  }
  return;
}


