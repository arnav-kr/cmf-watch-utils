// entry 0x100e8bd0  FUN_100e8bd0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e8bd0(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_48;
  undefined1 auStack_44 [60];
  
  local_48 = 0;
  FUN_101bf3b2(auStack_44,0,0x3c);
  if ((param_2 == 1) && (iVar1 = thunk_FUN_101978c0(), iVar1 != 0)) {
    uVar2 = thunk_FUN_101978c0();
    FUN_101aa780(&local_48,0x40,_LAB_100e8c18,uVar2);
  }
  else {
    FUN_101bf34a(&local_48,_LAB_100e8c14);
  }
  FUN_100e895c(&local_48);
  return 0;
}


