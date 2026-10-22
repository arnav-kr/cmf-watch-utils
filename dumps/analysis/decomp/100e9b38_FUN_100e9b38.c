// entry 0x100e9b38  FUN_100e9b38

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e9b38(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 local_48;
  undefined1 auStack_44 [60];
  
  local_48 = 0;
  FUN_101bf3b2(auStack_44,0,0x3c);
  if ((param_2 == 1) &&
     (iVar1 = FUN_1010e1e4(PTR_s_BT_BQB_MODE_100e9b8c,_DAT_100e9b88,1), iVar1 == 0)) {
    FUN_101bf34a(&local_48,PTR_s_ENTERFCCMODE_OK_100e9b90);
    FUN_100e895c(&local_48);
    FUN_100eeff4(0);
  }
  else {
    FUN_101bf34a(&local_48,PTR_s_ENTERFCCMODE_FAIL_100e9b94);
    FUN_100e895c(&local_48);
  }
  return 0;
}


