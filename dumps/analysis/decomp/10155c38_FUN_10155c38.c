// entry 0x10155c38  FUN_10155c38

undefined4 FUN_10155c38(void)

{
  int iVar1;
  undefined2 local_34;
  undefined1 local_32;
  undefined1 auStack_30 [3];
  undefined1 auStack_2d [37];
  
  local_34 = *DAT_10155ca0;
  local_32 = *(undefined1 *)(DAT_10155ca0 + 1);
  iVar1 = FUN_1015c06c(auStack_30,1);
  if (iVar1 == 0) {
    FUN_1014c818((DAT_10155ca8 - DAT_10155ca4) * 0x20 & 0xff00U | 0x3300031,DAT_10155cb0);
  }
  else {
    iVar1 = FUN_101bf39a(auStack_2d,&local_34,3);
    if (iVar1 == 0) {
      FUN_1014c818((DAT_10155ca8 - DAT_10155ca4) * 0x20 & 0xff00U | 0x32c0031,DAT_10155cac);
      return 1;
    }
  }
  return 0;
}


