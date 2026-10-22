// entry 0x10058508  FUN_10058508

bool FUN_10058508(undefined4 param_1,uint param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_10;
  uint local_c;
  undefined4 uStack_8;
  
  local_10 = 0;
  local_c = param_2 & 0xffff0000;
  uStack_8 = param_3;
  thunk_FUN_101466e8(DAT_1005853c,&local_10,5);
  uVar1 = FUN_101bf36a(DAT_10058540);
  iVar2 = FUN_101bf39a(&local_10,DAT_10058540,uVar1);
  return iVar2 == 0;
}


