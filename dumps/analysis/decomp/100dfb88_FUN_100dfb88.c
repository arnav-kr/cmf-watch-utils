// entry 0x100dfb88  FUN_100dfb88

int FUN_100dfb88(void)

{
  int iVar1;
  
  iVar1 = FUN_100dfa3c();
  iVar1 = *(int *)(iVar1 * 0xc + DAT_100dfba4 + 8);
  if (iVar1 == 0) {
    iVar1 = DAT_100dfba8;
  }
  return iVar1;
}


