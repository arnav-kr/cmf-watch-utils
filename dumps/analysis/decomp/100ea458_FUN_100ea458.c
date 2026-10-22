// entry 0x100ea458  FUN_100ea458  (100ea458)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100ea458(undefined4 param_1,int param_2)

{
  undefined1 auStack_4c [4];
  undefined4 uStack_48;
  undefined1 auStack_44 [60];
  
  uStack_48 = 0;
  FUN_101bf3b2(auStack_44,0,0x3c);
  if (param_2 == 1) {
    ux2sys_api_9(10,auStack_4c,3);
    FUN_101aa780(&uStack_48,0x40,_LAB_100ea49c,auStack_4c[0]);
  }
  else {
    FUN_101bf34a(&uStack_48,_LAB_100ea4a0);
  }
  FUN_100e895c(&uStack_48);
  return 0;
}


