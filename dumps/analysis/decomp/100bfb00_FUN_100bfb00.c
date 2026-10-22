// entry 0x100bfb00  FUN_100bfb00  (100bfb00)

void FUN_100bfb00(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  iVar1 = FUN_101c4c9a();
  iVar2 = FUN_101c4ca6(param_1);
  uVar4 = ((int)PTR_DAT_100bfb68 - (int)PTR_LAB_100bfb6c) * 0x20 & 0xff00;
  FUN_1014c818(uVar4 | 0x7e0031,PTR_s_Clicked__d_100bfb70,iVar2);
  if (iVar1 != 7) {
    return;
  }
  if (iVar2 == 0) {
    FUN_1014c818(uVar4 | 0x840031,PTR_s_regulation_information_100bfb74);
    uVar3 = 0xa1;
  }
  else {
    if (iVar2 != 1) {
      return;
    }
    FUN_1014c818(uVar4 | 0x8a0031,PTR_s_law_information_100bfb78);
    uVar3 = 0xa2;
  }
  FUN_101898bc(uVar3,0);
  return;
}


