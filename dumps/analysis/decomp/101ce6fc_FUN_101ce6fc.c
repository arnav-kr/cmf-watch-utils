// entry 0x101ce6fc  FUN_101ce6fc

undefined4 FUN_101ce6fc(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uStack_c;
  undefined4 uStack_8;
  
  uStack_c = param_2;
  uStack_8 = param_3;
  if (((int *)*DAT_10149f9c == (int *)0x0) || (iVar1 = *(int *)*DAT_10149f9c, iVar1 == 0)) {
    FUN_1014c818((DAT_10149fa0 - DAT_10149fa4) * 0x20 & 0xff00U | 0xa50011,DAT_10149fa8);
  }
  else {
    iVar1 = (*(code *)**(undefined4 **)(iVar1 + 8))
                      (iVar1,3,&uStack_c,(code *)**(undefined4 **)(iVar1 + 8),3);
    if (-1 < iVar1) {
      return uStack_c;
    }
    FUN_1014c818((DAT_10149fa0 - DAT_10149fa4) * 0x20 & 0xff00U | 0xab0011,DAT_10149fac);
  }
  return 0xffffffed;
}


