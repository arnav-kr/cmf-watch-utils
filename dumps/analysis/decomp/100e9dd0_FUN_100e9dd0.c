// entry 0x100e9dd0  FUN_100e9dd0  (100e9dd0)

undefined4 FUN_100e9dd0(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uStack_74;
  undefined1 auStack_70 [32];
  undefined4 uStack_50;
  undefined1 auStack_4c [64];
  
  uStack_50 = 0;
  FUN_101bf3b2(auStack_4c,0,0x3c);
  if (param_2 == 1) {
    uStack_74 = 0;
    FUN_101bf3b2(auStack_70,0,0x1d);
    iVar1 = FUN_1010e214(PTR_s_APP_SECRET_100e9e24,&uStack_74,0x20);
    if (0 < iVar1) {
      FUN_101aa780(&uStack_50,0x40,PTR_s_GETSECRET__s_OK_100e9e2c,&uStack_74);
      goto LAB_100e9e18;
    }
  }
  FUN_101bf34a(&uStack_50,PTR_s_GETSECRET_FAIL_100e9e28);
LAB_100e9e18:
  FUN_100e895c(&uStack_50);
  return 0;
}


