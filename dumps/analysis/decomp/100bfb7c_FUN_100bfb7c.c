// entry 0x100bfb7c  FUN_100bfb7c  (100bfb7c)

void FUN_100bfb7c(int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  
  piVar2 = *(int **)(param_1 + 0x14);
  if (piVar2 != (int *)0x0) {
    uVar1 = FUN_101c6514(piVar2[1]);
    *DAT_100bfbf0 = uVar1;
    lv_style_7(piVar2 + 0x2c);
    lv_style_7(piVar2 + 0x2f);
    lv_style_7(piVar2 + 0x32);
    lv_style_7(piVar2 + 0x35);
    if (*piVar2 != 0) {
      lv_obj_tree_16();
      *piVar2 = 0;
    }
    if ((char)piVar2[0x48] != '\0') {
      thunk_FUN_10185d3c(piVar2 + 0x38);
      *(undefined1 *)(piVar2 + 0x48) = 0;
    }
    if (*(char *)((int)piVar2 + 0x121) != '\0') {
      thunk_FUN_10185d3c(piVar2 + 0x40);
      *(undefined1 *)((int)piVar2 + 0x121) = 0;
    }
    FUN_101d777a(piVar2 + 0x12,0xd);
    return;
  }
  return;
}


