// entry 0x100ea50c  FUN_100ea50c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100ea50c(undefined4 param_1,int param_2)

{
  undefined4 local_23c;
  undefined1 auStack_238 [60];
  undefined1 auStack_1fc [63];
  undefined1 auStack_1bd [437];
  
  local_23c = 0;
  FUN_101bf3b2(auStack_238,0,0x3c);
  if (param_2 == 1) {
    ux2sys_api_7(0x11,auStack_1fc);
    FUN_101aa780(&local_23c,0x40,_LAB_100ea554,auStack_1bd);
  }
  else {
    FUN_101bf34a(&local_23c,_LAB_100ea558);
  }
  FUN_100e895c(&local_23c);
  return 0;
}


