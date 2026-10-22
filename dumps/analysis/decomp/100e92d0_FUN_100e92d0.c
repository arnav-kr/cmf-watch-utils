// entry 0x100e92d0  FUN_100e92d0  (100e92d0)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e92d0(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined4 uStack_50;
  undefined1 auStack_4c [64];
  
  uStack_50 = 0;
  FUN_101bf3b2(auStack_4c,0,0x3c);
  uVar3 = *(undefined4 *)(param_3 + 4);
  puVar2 = PTR_s_SETFHTESTSWITCH__s_FAIL_100e9334;
  if (param_2 == 2) {
    iVar1 = FUN_101bf37a(uVar3,_LAB_100e9328);
    if (iVar1 == 0) {
      FUN_101207cc(1);
    }
    else {
      iVar1 = FUN_101bf37a(*(undefined4 *)(param_3 + 4),PTR_s_0_10219df8_0x1b_100e9330);
      if (iVar1 == 0) {
        FUN_10120858();
      }
    }
    uVar3 = *(undefined4 *)(param_3 + 4);
    puVar2 = PTR_s_SETFHTESTSWITCH__s_OK_100e932c;
  }
  FUN_101aa780(&uStack_50,0x40,puVar2,uVar3);
  FUN_100e895c(&uStack_50);
  return 0;
}


