// entry 0x101071e0  FUN_101071e0  (101071e0)

int FUN_101071e0(int param_1)

{
  int iVar1;
  code *pcVar2;
  
  iVar1 = 0;
  if (*(int *)(param_1 + 4) != 0) {
    pcVar2 = *(code **)(*(int *)(*(int *)(param_1 + 4) + 0x1c) + 0x1c);
    if (pcVar2 == (code *)0x0) {
      iVar1 = -0x86;
    }
    else {
      iVar1 = (*pcVar2)();
      if (iVar1 < 0) {
        FUN_1014c818((DAT_10107230 - DAT_1010722c) * 0x20 & 0xff00U | 0xe80012,DAT_10107234,iVar1,
                     param_1 + 9);
        FUN_101bbdaa();
      }
      else {
        *(undefined1 *)(param_1 + 9) = 0;
        *(undefined4 *)(param_1 + 4) = 0;
      }
    }
  }
  return iVar1;
}


