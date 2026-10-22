// entry 0x100bf90c  FUN_100bf90c  (100bf90c)

undefined4 FUN_100bf90c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined *puVar1;
  int iVar2;
  undefined *puVar3;
  uint uVar4;
  int iVar5;
  
  puVar1 = PTR_LAB_100bf9a0;
  puVar3 = PTR_DAT_100bf99c;
  iVar5 = *(int *)(param_1 + 0x14);
  if (*(char *)(iVar5 + 0x120) == '\0') {
    iVar2 = FUN_100df9a0(iVar5 + 0xe0,PTR_s__NAND_A_font28M_font_100bf9a4,param_3,0,param_4);
    if (-1 < iVar2) {
      *(undefined1 *)(iVar5 + 0x120) = 1;
      goto LAB_100bf930;
    }
LAB_100bf96c:
    uVar4 = ((int)puVar3 - (int)puVar1) * 0x20 & 0xff00U | 0xdc0000;
    puVar3 = PTR_s_font_not_found_10216b0e_3_100bf9b4;
  }
  else {
LAB_100bf930:
    if (*(char *)(iVar5 + 0x121) == '\0') {
      iVar2 = FUN_100df9a0(iVar5 + 0x100,PTR_s__NAND_A_font32M_font_100bf9a8);
      if (iVar2 < 0) goto LAB_100bf96c;
      *(undefined1 *)(iVar5 + 0x121) = 1;
    }
    iVar5 = FUN_10184a5c(param_1 + 4,PTR_DAT_100bf9ac,iVar5 + 0x48,0xd);
    uVar4 = ((int)puVar3 - (int)puVar1) * 0x20 & 0xff00;
    if (-1 < iVar5) {
      FUN_1014c818(uVar4 | 0xe90031,PTR_s_load_resource_succeed_10217f59_0x1c_100bf9b8);
      return 0;
    }
    uVar4 = uVar4 | 0xe50000;
    puVar3 = PTR_s_str_not_found_100bf9b0;
  }
  FUN_1014c818(uVar4 | 0x11,puVar3);
  return 0xfffffffe;
}


