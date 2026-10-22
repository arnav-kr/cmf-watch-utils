// 0x1022d830 FUN_100f731c :: cannot found FIRMWARE_VERSION\n
// entry 0x100f731c  FUN_100f731c

int FUN_100f731c(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = param_1;
  local_c = param_2;
  uVar2 = FUN_100f010c();
  iVar1 = DAT_100f7368;
  FUN_101bf3aa(DAT_100f7368,uVar2,0x80);
  local_10 = 0;
  local_c = 0;
  iVar3 = FUN_1010e214(DAT_100f736c,&local_10,8);
  if (iVar3 == 8) {
    *(undefined4 *)(iVar1 + 4) = local_10;
    *(undefined4 *)(iVar1 + 8) = local_c;
  }
  else {
    FUN_101ba194(DAT_100f7370);
    local_10 = *(undefined4 *)(iVar1 + 4);
    local_c = local_10;
    FUN_1010e1b4(DAT_100f736c,&local_10,8);
  }
  return DAT_100f7368;
}


