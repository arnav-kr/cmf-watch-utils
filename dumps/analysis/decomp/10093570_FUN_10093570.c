// entry 0x10093570  FUN_10093570  (10093570)

void FUN_10093570(undefined4 param_1)

{
  undefined1 *puVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  undefined1 uVar5;
  undefined4 local_204 [125];
  
  uVar3 = FUN_101c4c96();
  iVar4 = FUN_101c4c9a(param_1);
  cVar2 = lv_obj_tree_14(uVar3);
  if (iVar4 == 7) {
    iVar4 = *DAT_100935f0;
    FUN_1014c818(DAT_100935fc | (DAT_100935f8 - DAT_100935f4) * 0x20 & 0xff00U,DAT_10093600,cVar2);
    puVar1 = DAT_10093608;
    if ((byte)(cVar2 - 1U) < 2) {
      cVar2 = *(char *)(iVar4 + 0xb4);
      *DAT_10093604 = 1;
      if (cVar2 == '\0') {
        FUN_100924ec();
        uVar5 = *puVar1;
      }
      else {
        uVar5 = 0x12;
        *puVar1 = 0x12;
      }
      lv_tileview_1(*(undefined4 *)(iVar4 + 4),uVar5,0,0);
      *DAT_1009360c = 0;
      local_204[0] = 0;
      ux2sys_api_6(99,local_204);
    }
  }
  return;
}


