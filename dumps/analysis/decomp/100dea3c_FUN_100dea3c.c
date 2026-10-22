// entry 0x100dea3c  FUN_100dea3c  (100dea3c)

int FUN_100dea3c(int param_1,int param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  
  uVar1 = DAT_100dea90;
  if (param_4 == 0) {
    uVar1 = DAT_100dea8c;
  }
  if (param_1 == -1) {
    FUN_101aa780(param_2,param_3,DAT_100dea9c,DAT_100dea98,DAT_100dea94,0,uVar1);
  }
  else {
    FUN_101aa780(param_2,param_3,DAT_100deaa0,DAT_100dea98,param_1,uVar1);
  }
  *(undefined1 *)(param_3 + param_2 + -1) = 0;
  return param_2;
}


