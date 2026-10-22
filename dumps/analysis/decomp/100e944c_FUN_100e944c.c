// entry 0x100e944c  FUN_100e944c  (100e944c)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100e944c(undefined4 param_1,int param_2,int param_3)

{
  byte bVar1;
  undefined2 uVar2;
  undefined4 uStack_244;
  undefined1 auStack_240 [60];
  byte abStack_204 [2];
  undefined2 uStack_202;
  
  uStack_244 = 0;
  FUN_101bf3b2(auStack_240,0,0x3c);
  if (param_2 == 3) {
    bVar1 = FUN_101aaaf8(*(undefined4 *)(param_3 + 4),0,0);
    uVar2 = FUN_101aaaf8(*(undefined4 *)(param_3 + 8),0,0);
    abStack_204[0] = bVar1;
    uStack_202 = uVar2;
    ux2sys_api_6(0x5e,abStack_204);
    FUN_101aa780(&uStack_244,0x40,_LAB_100e94b8,bVar1,uVar2);
  }
  else {
    FUN_101bf34a(&uStack_244,_LAB_100e94bc);
  }
  FUN_100e895c(&uStack_244);
  return 0;
}


