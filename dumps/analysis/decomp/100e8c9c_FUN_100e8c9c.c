// entry 0x100e8c9c  FUN_100e8c9c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e8c9c(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 local_48;
  undefined1 auStack_44 [60];
  
  local_48 = 0;
  FUN_101bf3b2(auStack_44,0,0x3c);
  if (param_2 == 1) {
    FUN_10117f98();
    uVar1 = FUN_101c083e();
    FUN_101aa780(&local_48,0x40,_LAB_100e8cdc,uVar1);
  }
  else {
    FUN_101bf34a(&local_48,_LAB_100e8ce0);
  }
  FUN_100e895c(&local_48);
  return 0;
}


