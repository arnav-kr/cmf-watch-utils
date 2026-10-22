// 0x10202209 FUN_10056724 :: mic_update_buf %p \n
// entry 0x10056724  FUN_10056724

int FUN_10056724(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  undefined1 *puVar6;
  uint uVar7;
  undefined4 *puVar8;
  int *piVar9;
  int iVar10;
  
  uVar1 = DAT_10056738;
  if (param_1 != 1) {
    return param_1;
  }
  iVar3 = thunk_FUN_10146a3c(0x4c,DAT_10056a9c,param_2,param_4,DAT_10056738,16000);
  piVar2 = DAT_10056aa0;
  *DAT_10056aa0 = iVar3;
  if (iVar3 != 0) {
    FUN_101bf3b2(iVar3,0,0x4c);
    puVar8 = (undefined4 *)*piVar2;
    puVar8[4] = 16000;
    puVar8[5] = param_2;
    uVar4 = thunk_FUN_10146a3c(0x3e0,DAT_10056a9c);
    *puVar8 = uVar4;
    piVar9 = (int *)*piVar2;
    if (*piVar9 != 0) {
      iVar3 = thunk_FUN_10146a3c(0x3e0,DAT_10056a9c);
      piVar9[1] = iVar3;
      iVar3 = *piVar2;
      if (*(int *)(iVar3 + 4) != 0) {
        uVar4 = thunk_FUN_10146a3c(0x200,DAT_10056a9c);
        iVar5 = *piVar2;
        *(undefined4 *)(iVar3 + 8) = uVar4;
        puVar6 = *(undefined1 **)(iVar5 + 8);
        if (puVar6 != (undefined1 *)0x0) {
          *(undefined1 **)(iVar5 + 0x3c) = puVar6;
          *puVar6 = 0x16;
          *(undefined2 *)(puVar6 + 10) = 16000;
          *(undefined2 *)(puVar6 + 2) = 0x1f8;
          *(undefined4 *)(puVar6 + 0xc) = 0;
          uVar4 = FUN_10188518(0xf0000);
          puVar8 = DAT_10056aa4;
          *DAT_10056aa4 = uVar4;
          FUN_101ba194(DAT_10056aa8,uVar4);
          iVar3 = *piVar2;
          uVar4 = FUN_10147408(*puVar8,0xf0000);
          *(undefined4 *)(iVar3 + 0x44) = uVar4;
          iVar3 = FUN_1014df50(5,0x10,0x10,1,1,*(undefined4 *)(*piVar2 + 0x44));
          uVar7 = (DAT_10056aac - DAT_10056ab0) * 0x20 & 0xff00;
          if (iVar3 != 0) {
            iVar10 = *piVar2;
            *(int *)(iVar10 + 0x40) = iVar3;
            uVar4 = FUN_101cecde();
            iVar5 = *piVar2;
            *(undefined4 *)(iVar10 + 0x48) = uVar4;
            uVar4 = FUN_101ce410(*(undefined4 *)(iVar5 + 0x48));
            FUN_1014c818(uVar7 | 0x7b0031,DAT_10056ab8,uVar4);
            FUN_101cec56(iVar3);
            thread_timer(*piVar2 + 0x20,uVar1,0);
            thread_timer_1(*piVar2 + 0x20,5);
            return 1;
          }
          FUN_1014c818(uVar7 | 0x730011,DAT_10056ab4);
        }
      }
    }
  }
  return 0;
}


