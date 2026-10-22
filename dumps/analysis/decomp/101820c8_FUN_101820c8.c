// entry 0x101820c8  FUN_101820c8  (101820c8)

undefined4
FUN_101820c8(undefined2 param_1,undefined1 param_2,undefined4 param_3,undefined2 param_4,
            undefined4 param_5)

{
  int iVar1;
  undefined4 uVar2;
  undefined2 local_38;
  undefined1 local_36;
  undefined1 local_35;
  undefined2 local_34;
  undefined2 local_32;
  undefined4 local_30;
  undefined4 local_2c;
  undefined1 *local_28;
  undefined1 auStack_24 [24];
  undefined4 local_c;
  
  local_36 = 9;
  local_32 = 0;
  local_2c = DAT_10182120;
  local_38 = param_1;
  local_35 = param_2;
  local_34 = param_4;
  local_30 = param_3;
  local_28 = auStack_24;
  FUN_101dae76(auStack_24,0,1);
  local_c = param_5;
  iVar1 = FUN_101464b4(DAT_10182124,&local_38);
  if (iVar1 == 0) {
    uVar2 = 0xffffff97;
  }
  else {
    iVar1 = FUN_101be202(auStack_24,0xffffffff);
    uVar2 = 0;
    if (iVar1 != 0) {
      uVar2 = 0xffffffc2;
    }
  }
  return uVar2;
}


