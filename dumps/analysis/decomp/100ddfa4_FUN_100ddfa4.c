// entry 0x100ddfa4  FUN_100ddfa4  (100ddfa4)

void FUN_100ddfa4(undefined4 param_1,int param_2)

{
  byte bVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined1 auStack_70 [69];
  undefined1 local_2b;
  
  uVar2 = DAT_100de01c;
  uVar5 = 0;
  bVar1 = *(byte *)(param_2 + 4);
  for (iVar4 = param_2; iVar4 != param_2 + (uint)bVar1 * 0x10; iVar4 = iVar4 + 0x10) {
    iVar3 = FUN_101aa780(auStack_70 + uVar5,0x45 - uVar5,uVar2,*(undefined4 *)(iVar4 + 0xc));
    uVar5 = iVar3 + uVar5 & 0xffff;
  }
  local_2b = 0;
  FUN_1014c818(DAT_100de014 | (DAT_100de00c - DAT_100de010) * 0x20 & 0xff00U,DAT_100de018,param_1,
               *(undefined1 *)(param_2 + 5),(uint)bVar1,auStack_70);
  return;
}


