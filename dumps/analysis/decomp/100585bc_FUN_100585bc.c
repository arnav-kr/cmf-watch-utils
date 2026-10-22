// entry 0x100585bc  FUN_100585bc

bool FUN_100585bc(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_10;
  undefined4 uStack_c;
  
  local_10 = 0;
  uStack_c = 0;
  thunk_FUN_101466e8(DAT_1005860c,&local_10,8);
  FUN_1014c818(DAT_10058618 | (DAT_10058614 - DAT_10058610) * 0x20 & 0xff00U,DAT_1005861c,&local_10)
  ;
  uVar1 = FUN_101bf36a(DAT_10058620);
  iVar2 = FUN_101bf39a(&local_10,DAT_10058620,uVar1);
  if (iVar2 == 0) {
    thunk_FUN_10146730(DAT_1005860c,DAT_10058624,8);
  }
  return iVar2 == 0;
}


