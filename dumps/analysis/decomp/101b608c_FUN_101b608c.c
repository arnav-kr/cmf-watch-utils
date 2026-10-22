// entry 0x101b608c  FUN_101b608c

undefined4
FUN_101b608c(undefined2 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined2 local_12 [3];
  
  iVar1 = FUN_100ccab4(param_4);
  if (iVar1 == 0) {
    *param_1 = (short)param_3;
  }
  else {
    param_2 = FUN_100ccc0c(local_12,param_2,param_3,iVar1);
    *param_1 = local_12[0];
  }
  return param_2;
}


