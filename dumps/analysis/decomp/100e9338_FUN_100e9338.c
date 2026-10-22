// entry 0x100e9338  FUN_100e9338  (100e9338)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e9338(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 local_48;
  undefined1 auStack_44 [60];
  
  local_48 = 0;
  FUN_101bf3b2(auStack_44,0,0x3c);
  uVar1 = _LAB_100e9370;
  if (param_2 == 1) {
    FUN_100cc278();
    uVar1 = _LAB_100e936c;
  }
  FUN_101bf34a(&local_48,uVar1);
  FUN_100e895c(&local_48);
  return 0;
}


