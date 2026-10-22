// entry 0x100e9184  FUN_100e9184  (100e9184)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e9184(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  undefined *puVar2;
  undefined4 local_50;
  undefined1 auStack_4c [64];
  
  local_50 = 0;
  FUN_101bf3b2(auStack_4c,0,0x3c);
  puVar2 = PTR_s_OPENCWALGO_FAIL_100e91f4;
  if (param_2 == 2) {
    iVar1 = FUN_101bf37a(*(undefined4 *)(param_3 + 4),_LAB_100e91e4);
    if (iVar1 == 0) {
      FUN_100c7998(1);
      FUN_101970c8(1);
      puVar2 = PTR_s_OPENCWALGO_OPEN_SUCCESS_100e91e8;
    }
    else {
      iVar1 = FUN_101bf37a(*(undefined4 *)(param_3 + 4),PTR_s_0_10219df8_0x1b_100e91ec);
      puVar2 = PTR_s_OPENCWALGO_FAIL_100e91f4;
      if (iVar1 == 0) {
        FUN_100c7998();
        FUN_101970c8(0);
        puVar2 = PTR_s_OPENCWALGO_CLOSE_SUCCESS_100e91f0;
      }
    }
  }
  FUN_101bf34a(&local_50,puVar2);
  FUN_100e895c(&local_50);
  return 0;
}


