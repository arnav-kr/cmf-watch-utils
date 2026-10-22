// entry 0x100e9c30  FUN_100e9c30  (100e9c30)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e9c30(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uStack_48;
  undefined1 auStack_44 [60];
  
  uStack_48 = 0;
  FUN_101bf3b2(auStack_44,0,0x3c);
  if ((param_2 == 1) && (iVar1 = FUN_1011fc30(), iVar1 - 1U < 0xfffffffe)) {
    FUN_101aa780(&uStack_48,0x40,_LAB_100e9c78,iVar1);
  }
  else {
    FUN_101bf34a(&uStack_48,_LAB_100e9c74);
  }
  FUN_100e895c(&uStack_48);
  return 0;
}


