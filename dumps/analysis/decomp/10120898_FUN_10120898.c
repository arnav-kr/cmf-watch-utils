// 0x1023bec7 FUN_10120898 ::  REGUPDATE: 0x%x
// entry 0x10120898  FUN_10120898

undefined4 FUN_10120898(undefined4 *param_1,byte *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  uint uVar5;
  undefined4 *puVar6;
  byte *local_1c;
  
  puVar4 = (undefined4 *)*param_1;
  local_1c = param_2;
  if ((int)((uint)*(byte *)((int)param_1 + 0x19) << 0x1e) < 0) {
    *(byte *)((int)param_1 + 0x19) = *(byte *)((int)param_1 + 0x19) & 0xfd;
    puVar6 = param_1;
    iVar1 = FUN_100f96ac();
    uVar5 = (iVar1 - param_1[7]) + 16000;
    if ((uint)(iVar1 - param_1[7]) < 0xffffc180) {
      uVar5 = uVar5 / 32000;
      param_1 = puVar6;
    }
    else {
      uVar5 = FUN_1004639c(uVar5,1,32000,0);
      param_1 = puVar6;
    }
    if (uVar5 < 0xb) {
      iVar1 = FUN_101dabba();
      if (iVar1 == 0) {
        sched_13(0xb - uVar5,0);
      }
      else {
        FUN_1019b814((0xb - uVar5) * 1000);
      }
    }
  }
  uVar5 = puVar4[7];
  iVar1 = puVar4[6];
  *(short *)(param_2 + 8) = ((short)(puVar4[5] & 0x7f) + (short)((puVar4[5] & 0x7f) << 2)) * 2;
  *param_2 = (byte)iVar1 & 0x3f;
  param_2[1] = (byte)((uint)(iVar1 << 0x12) >> 0x1a);
  param_2[3] = (byte)uVar5 & 0x1f;
  param_2[5] = 0;
  iVar3 = ((uVar5 & 0xfff) >> 8) - 1;
  iVar2 = ((uVar5 & 0x7fffff) >> 0x10) + 100;
  param_2[2] = (byte)((uint)(iVar1 << 0xb) >> 0x1b);
  param_2[4] = (byte)iVar3;
  *(short *)(param_2 + 6) = (short)iVar2;
  FUN_101c2b4c(param_2,&local_1c,iVar2,iVar3,param_1);
  param_2[5] = (byte)(((uint)local_1c / DAT_10120a40 + 4) % 7);
  iVar1 = FUN_101c2b04(param_2);
  if (iVar1 < 0) {
    uVar5 = (DAT_10120a48 - DAT_10120a44) * 0x20 & 0xff00;
    FUN_1014c818(DAT_10120a4c | uVar5,DAT_10120a50);
    FUN_1014c818(uVar5 | 0x7e0031,DAT_10120a54);
    FUN_1014c818(uVar5 | 0x7f0031,DAT_10120a58,puVar4);
    FUN_1014c818(uVar5 | 0x800031,DAT_10120a5c,*puVar4);
    FUN_1014c818(uVar5 | 0x810031,DAT_10120a60,puVar4[1]);
    FUN_1014c818(uVar5 | 0x820031,DAT_10120a64,puVar4[2]);
    FUN_1014c818(uVar5 | 0x830031,DAT_10120a68,puVar4[3]);
    FUN_1014c818(uVar5 | 0x840031,DAT_10120a6c,puVar4[4]);
    FUN_1014c818(uVar5 | 0x850031,DAT_10120a70,puVar4[5]);
    FUN_1014c818(uVar5 | 0x860031,DAT_10120a74,puVar4[6]);
    FUN_1014c818(uVar5 | 0x870031,DAT_10120a78,puVar4[7]);
  }
  return 0;
}


