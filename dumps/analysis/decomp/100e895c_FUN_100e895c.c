// entry 0x100e895c  FUN_100e895c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e895c(undefined4 param_1)

{
  undefined4 uVar1;
  
  if (*_LAB_100e8988 == '\0') {
    FUN_101ba194(_LAB_100e898c,param_1);
  }
  else if (*_LAB_100e8988 == '\x01') {
    uVar1 = FUN_101bf372(param_1,0x40);
    FUN_100cb9e0(param_1,uVar1);
  }
  return 0;
}


