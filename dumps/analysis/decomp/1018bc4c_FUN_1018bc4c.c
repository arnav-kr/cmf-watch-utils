// 0x101fd9f0 FUN_1018bc4c :: view %d update
// entry 0x1018bc4c  FUN_1018bc4c

undefined4 FUN_1018bc4c(uint param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  byte bVar3;
  int *piVar4;
  
  piVar1 = DAT_1018bddc;
  piVar4 = (int *)*DAT_1018bddc;
  while( true ) {
    if (piVar4 == (int *)0x0) {
      return 0xfffffffd;
    }
    if (*(ushort *)(piVar4[1] + 0x10) == param_1) break;
    piVar4 = (int *)*piVar4;
  }
  if ((code *)DAT_1018bddc[0x14] != (code *)0x0) {
    (*(code *)DAT_1018bddc[0x14])(param_1,param_2,param_3);
  }
  if (param_2 != 8) {
    if (param_2 == 0xb) {
      FUN_1014c818((DAT_1018bde4 - DAT_1018bde0) * 0x20 & 0xff00U | 0x1220031,DAT_1018bdf4,param_1);
      FUN_1018e784(piVar4,0);
    }
    else if (param_2 == 7) {
      FUN_1014c818(DAT_1018bdec | (DAT_1018bde4 - DAT_1018bde0) * 0x20 & 0xff00U,DAT_1018bdf0,
                   param_1,*(undefined1 *)((int)piVar1 + 0x5d),(*(byte *)(piVar4 + 4) & 7) >> 2);
      FUN_10188c5c(piVar4[8],*(undefined1 *)((int)piVar1 + 0x5d));
      FUN_1018e700(piVar4);
      bVar3 = *(byte *)(piVar4 + 4);
      if ((int)((uint)bVar3 << 0x1c) < 0) {
        FUN_1018e784(piVar4,0);
        bVar3 = *(byte *)(piVar4 + 4);
      }
      *(byte *)(piVar4 + 4) = bVar3 | 2;
    }
    *(undefined2 *)((int)piVar1 + 0x62) = *(undefined2 *)(piVar4[1] + 0x10);
    FUN_10146a30();
    FUN_1018e6e4(piVar4);
    iVar2 = piVar4[1];
    if (*(code **)(iVar2 + 8) == (code *)0x0) {
      if (param_3 == 0) {
        param_3 = (int)(piVar4 + 7);
      }
      (**(code **)(iVar2 + 4))(*(undefined2 *)(iVar2 + 0x10),param_2,param_3);
    }
    else {
      (**(code **)(iVar2 + 8))(*(undefined2 *)(iVar2 + 0x10),piVar4 + 7,param_2,param_3);
    }
    *(undefined2 *)((int)piVar1 + 0x62) = 0;
    FUN_10146a30(0);
    return 0;
  }
  FUN_1014c818((DAT_1018bde4 - DAT_1018bde0) * 0x20 & 0xff00U | 0x1180031,DAT_1018bde8,param_1);
  *(undefined2 *)((int)piVar1 + 0x62) = *(undefined2 *)(piVar4[1] + 0x10);
  FUN_10146a30();
  FUN_1018e6e4(piVar4);
  iVar2 = piVar4[1];
  if (*(code **)(iVar2 + 8) == (code *)0x0) {
    if (param_3 == 0) {
      param_3 = (int)(piVar4 + 7);
    }
    (**(code **)(iVar2 + 4))(*(undefined2 *)(iVar2 + 0x10),8,param_3);
  }
  else {
    (**(code **)(iVar2 + 8))(*(undefined2 *)(iVar2 + 0x10),piVar4 + 7,8,param_3);
  }
  *(undefined2 *)((int)piVar1 + 0x62) = 0;
  FUN_10146a30(0);
  FUN_1018e738(piVar4);
  FUN_101d7a08(piVar4[8],1);
  *(byte *)(piVar4 + 4) = *(byte *)(piVar4 + 4) & 0xdd;
  return 0;
}


