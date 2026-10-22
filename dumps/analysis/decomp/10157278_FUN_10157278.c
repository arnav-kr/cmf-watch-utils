// entry 0x10157278  FUN_10157278

void FUN_10157278(undefined4 *param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  FUN_1014c818((DAT_101572b0 - DAT_101572ac) * 0x20 & 0xff00U | 0xc70031,DAT_101572b4,param_2);
  uVar1 = *param_1;
  *param_1 = param_2;
  if ((code *)param_1[3] != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x101572a8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)param_1[3])(param_2,uVar1);
    return;
  }
  return;
}


