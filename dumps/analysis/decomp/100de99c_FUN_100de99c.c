// entry 0x100de99c  FUN_100de99c  (100de99c)

int FUN_100de99c(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined4 uVar5;
  
  uVar2 = DAT_100dea18;
  if (param_4 == 0) {
    uVar2 = DAT_100dea14;
  }
  iVar1 = 0;
  piVar4 = DAT_100dea1c;
  uVar3 = DAT_100dea24;
  if (param_1 == -1) {
    param_1 = 0;
    uVar5 = DAT_100dea2c;
  }
  else {
    do {
      if (param_1 == *piVar4) {
        uVar3 = DAT_100dea34;
        uVar5 = DAT_100dea30;
        if (param_4 != 0) {
          FUN_101aa780(param_2,param_3,DAT_100dea28,DAT_100dea24,param_1,uVar2);
          goto LAB_100de9de;
        }
        break;
      }
      iVar1 = iVar1 + 1;
      piVar4 = piVar4 + 1;
      uVar5 = DAT_100dea20;
    } while (iVar1 != 6);
  }
  FUN_101aa780(param_2,param_3,DAT_100dea38,uVar3,uVar5,param_1,uVar2);
LAB_100de9de:
  *(undefined1 *)(param_3 + param_2 + -1) = 0;
  return param_2;
}


