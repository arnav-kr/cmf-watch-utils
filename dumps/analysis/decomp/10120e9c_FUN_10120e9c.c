// 0x1023c039 FUN_10120e9c :: S4 sleep interval: %ds update, bk_rc32k=%d, rc32k_now=%d\n
// entry 0x10120e9c  FUN_10120e9c

undefined4 FUN_10120e9c(int param_1)

{
  char cVar1;
  bool bVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  undefined *puVar8;
  undefined4 uVar9;
  uint uVar10;
  uint *puVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  uint uVar14;
  uint uVar15;
  undefined4 local_4c;
  undefined4 uStack_48;
  short local_44;
  int local_40;
  int local_3c;
  uint local_38;
  uint local_34;
  int local_30;
  char local_2c;
  char local_2b;
  char local_29;
  
  puVar12 = *(undefined4 **)(param_1 + 0x10);
  puVar13 = *(undefined4 **)(param_1 + 4);
  puVar11 = (uint *)*puVar13;
  *puVar12 = puVar11;
  *(byte *)((int)puVar12 + 0x19) = *(byte *)((int)puVar12 + 0x19) & 0xfe;
  iVar5 = FUN_101baa48();
  if (iVar5 != 0) {
    *(byte *)((int)puVar12 + 0x19) = *(byte *)((int)puVar12 + 0x19) | 1;
  }
  if ((int)(*puVar11 << 0x1e) < 0) {
    *puVar11 = *puVar11 | 2;
  }
  puVar4 = DAT_10121154;
  puVar3 = DAT_1012113c;
  puVar11 = (uint *)*puVar12;
  cVar1 = *(char *)(*(int *)(param_1 + 4) + 4);
  if (cVar1 == '\0') {
    *puVar11 = *puVar11 & 0xfffffff3;
    uVar10 = *puVar11 | 8;
  }
  else {
    if (cVar1 == '\x02') {
      *puVar11 = 1;
      *puVar4 = 0;
      FUN_1019b814(500);
      uVar9 = 0xcc1;
    }
    else {
      *DAT_1012113c = 0x10;
      puVar3[1] = 0x10;
      *DAT_10121140 = DAT_10121144;
      *puVar11 = 5;
      *puVar4 = 0;
      FUN_1019b814(500);
      uVar9 = 0xc8c;
    }
    *puVar4 = uVar9;
    uVar10 = *puVar11 | 0x20000;
  }
  *puVar11 = uVar10;
  FUN_101c2a00(param_1);
  FUN_101bf3b2(&local_40,0);
  local_4c = 0;
  uStack_48 = 0;
  local_44 = 0;
  iVar5 = FUN_100efc80(200);
  FUN_101ba194(PTR_s_cur_rc32k_freq__d_10121108,iVar5);
  iVar6 = FUN_1010e25c(PTR_s_PM_BAK_TIME_1012110c,&local_40);
  if ((iVar6 == 0x18) && (local_2c != '\0')) {
    uVar10 = FUN_100efae4(1);
    if ((int)uVar10 < 1) {
      iVar5 = (int)PTR_DAT_1012111c - (int)PTR_LAB_10121120;
      uVar10 = DAT_10121150;
      puVar8 = DAT_1012114c;
    }
    else {
      FUN_101ba194(PTR_s_power_on_current_8hz___d_10121110,uVar10);
      if (uVar10 < local_38) {
        iVar6 = (uVar10 - local_38) + 0x3ffffff;
      }
      else {
        iVar6 = uVar10 - local_38;
      }
      uVar7 = FUN_101baaf4(iVar6,local_30 + iVar5);
      uVar14 = local_3c + 9 + uVar7;
      uVar9 = 0;
      bVar2 = (bool)isCurrentModePrivileged();
      if (bVar2) {
        uVar9 = getBasePriority();
      }
      bVar2 = (bool)isCurrentModePrivileged();
      if ((bVar2) && (uVar15 = getBasePriority(), uVar15 == 0 || 0x20 < uVar15)) {
        setBasePriority(0x20);
      }
      InstructionSynchronizationBarrier(0xf);
      uVar15 = uVar14 / 1000;
      FUN_10121190(uVar15 + local_40,&local_4c,uVar9,local_40,uVar9);
      local_44 = (short)uVar14 + (short)uVar15 * -1000;
      FUN_10120d48(puVar12,&local_4c);
      *(byte *)((int)puVar12 + 0x19) = *(byte *)((int)puVar12 + 0x19) | 1;
      bVar2 = (bool)isCurrentModePrivileged();
      if (bVar2) {
        setBasePriority(uVar9);
      }
      InstructionSynchronizationBarrier(0xf);
      FUN_101ba194(PTR_s_S4_sleep_interval___ds_update__b_10121114,uVar7 / 1000,local_30,iVar5);
      if (local_2b == '\0') {
        if (local_29 != '\0') {
          FUN_101ba194(DAT_10121148);
          FUN_101bab08();
        }
      }
      else {
        if (uVar10 < local_34) {
          uVar10 = local_34 - uVar10;
        }
        else {
          uVar10 = (local_34 + 0x3ffffff) - uVar10;
        }
        uVar9 = FUN_1004639c((int)((ulonglong)uVar10 * 0x3e8000),
                             (int)((ulonglong)uVar10 * 0x3e8000 >> 0x20),iVar5,0);
        FUN_101ba194(PTR_s_recovery_alarm__after__d_ms__int_10121118,uVar9,uVar10,local_34);
        FUN_100efb80(uVar9);
      }
      FUN_101212c8(&local_4c);
      FUN_101bf3b2(&local_40,0);
      iVar5 = FUN_1010e1e4(PTR_s_PM_BAK_TIME_1012110c,&local_40);
      if (iVar5 == 0) goto LAB_10121044;
      iVar5 = (int)PTR_DAT_1012111c - (int)PTR_LAB_10121120;
      uVar10 = DAT_10121128;
      puVar8 = PTR_s_failed_to_save_pm_backup_time_to_10121124;
    }
    FUN_1014c818(uVar10 | iVar5 * 0x20 & 0xff00U,puVar8);
  }
LAB_10121044:
  iVar5 = FUN_10120898(puVar12,&local_40);
  if ((iVar5 == 0) && (iVar5 = FUN_101c2b04(&local_40), iVar5 < 0)) {
    FUN_1014c818(DAT_1012112c | ((int)PTR_DAT_1012111c - (int)PTR_LAB_10121120) * 0x20 & 0xff00U,
                 DAT_10121130);
    FUN_10121190(DAT_10121134,&local_40);
    FUN_10120d48(puVar12,&local_40);
  }
  (*(code *)puVar13[2])();
  FUN_101ba194(DAT_10121138);
  return 0;
}


