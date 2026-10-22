// 0x10234f39 FUN_1010b8d8 :: get efuse power version ret:%d value:%x default %x\n
// entry 0x1010b8d8  FUN_1010b8d8

uint FUN_1010b8d8(int param_1,undefined4 param_2,uint param_3)

{
  byte bVar1;
  undefined4 *puVar2;
  byte *pbVar3;
  undefined *puVar4;
  undefined1 uVar5;
  uint uVar6;
  undefined4 uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  char cVar11;
  undefined4 local_1c [2];
  
  iVar10 = DAT_1010ba5c;
  if (*(uint *)(*(int *)(param_1 + 4) + 4) < param_3) {
    return 0xffffffea;
  }
  *DAT_1010ba58 = *DAT_1010ba58 & 0xfffeffff;
  uVar6 = FUN_1010b7b8(PTR_s_bttbl_bin_1010ba60,iVar10);
  local_1c[0] = 0;
  uVar7 = FUN_101de8b0();
  uVar8 = (uint)*(byte *)(iVar10 + 0x44);
  FUN_101ba194(PTR_s_opt__d_Befor_set_0x42___0x_x_0x4_1010ba64,uVar7,*(undefined1 *)(iVar10 + 0x42),
               *(undefined1 *)(iVar10 + 0x43),uVar8);
  iVar9 = FUN_101bf5fa(2,local_1c);
  FUN_101ba194(PTR_s_get_efuse_avdd_ret__d_value__x_d_1010ba68,iVar9,local_1c[0],
               *(undefined1 *)(iVar10 + 0x50));
  cVar11 = iVar9 == 0;
  if ((bool)cVar11) {
    *DAT_1010ba6c = local_1c[0];
  }
  iVar10 = FUN_101bf5fa(0,local_1c);
  FUN_101ba194(DAT_1010ba70,iVar10,local_1c[0],*(undefined1 *)(DAT_1010ba5c + 0x54),uVar8);
  if (iVar10 == 0) {
    cVar11 = cVar11 + '\x01';
    *DAT_1010ba74 = local_1c[0];
  }
  iVar9 = FUN_101bf5fa(1,local_1c);
  iVar10 = DAT_1010ba5c;
  FUN_101ba194(DAT_1010ba78,iVar9,local_1c[0],*(undefined1 *)(DAT_1010ba5c + 0x58));
  if (iVar9 == 0) {
    *DAT_1010ba7c = local_1c[0];
  }
  else if (cVar11 == '\0') goto LAB_1010b994;
  *(byte *)(iVar10 + 0x43) = *(byte *)(iVar10 + 0x43) | 0x10;
LAB_1010b994:
  *(byte *)(iVar10 + 0x42) = *(byte *)(iVar10 + 0x42) & 0xde | 0x82;
  pbVar3 = DAT_1010ba88;
  puVar2 = DAT_1010ba84;
  uVar7 = DAT_1010ba80;
  *(byte *)(iVar10 + 0x44) = *(byte *)(iVar10 + 0x44) & 0xfb;
  puVar4 = PTR_s_Bt_uart_rate__d_1010ba8c;
  *puVar2 = uVar7;
  FUN_101ba194(puVar4);
  puVar4 = PTR_s_max_rf_power__d_1010ba90;
  if ((int)((uint)*pbVar3 << 0x1e) < 0) {
    bVar1 = pbVar3[2];
    *(byte *)(iVar10 + 0x22) = bVar1;
    *(byte *)(iVar10 + 0x25) = bVar1;
    FUN_101ba194(puVar4);
  }
  puVar4 = PTR_s_ble_rf_tx_power__d_1010ba94;
  if ((int)((uint)*pbVar3 << 0x1d) < 0) {
    *(byte *)(DAT_1010ba5c + 0x29) = pbVar3[3];
    FUN_101ba194(puVar4);
  }
  iVar9 = FUN_1010d4b4();
  iVar10 = DAT_1010ba5c;
  if (iVar9 != 0) {
    *(byte *)(DAT_1010ba5c + 0x41) = *(byte *)(DAT_1010ba5c + 0x41) & 0xfd;
    *(byte *)(iVar10 + 0x44) = *(byte *)(iVar10 + 0x44) | 0x10;
    *(undefined1 *)(iVar10 + 0x2b) = 1;
    *(undefined1 *)(iVar10 + 0xb5) = 0xe;
  }
  iVar9 = FUN_101de8b0();
  iVar10 = DAT_1010ba5c;
  if (iVar9 != 0) {
    *(byte *)(DAT_1010ba5c + 0x43) = *(byte *)(DAT_1010ba5c + 0x43) | 4;
  }
  uVar5 = FUN_100eecf8();
  *(undefined1 *)(iVar10 + 0xb6) = uVar5;
  FUN_101ba194(PTR_s_After_set_0x42___0x_x_0x43___0x__1010ba98,*(undefined1 *)(iVar10 + 0x42),
               *(undefined1 *)(iVar10 + 0x43),*(undefined1 *)(iVar10 + 0x44));
  uVar8 = FUN_1010b7b8(DAT_1010baa0,DAT_1010ba9c);
  return uVar8 | uVar6;
}


