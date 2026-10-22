// entry 0x100eae74  FUN_100eae74  (100eae74)

void FUN_100eae74(int *param_1)

{
  int iVar1;
  code *UNRECOVERED_JUMPTABLE;
  
  FUN_1014c818((DAT_100eaea8 - DAT_100eaeac) * 0x20 & 0xff00U | 0xdf0031,DAT_100eaeb0);
  if (((param_1 != (int *)0x0) && (iVar1 = *param_1, iVar1 != 0)) &&
     (UNRECOVERED_JUMPTABLE = *(code **)(*(int *)(iVar1 + 8) + 0x18),
     UNRECOVERED_JUMPTABLE != (code *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x100eaea4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(iVar1,0);
    return;
  }
  return;
}


