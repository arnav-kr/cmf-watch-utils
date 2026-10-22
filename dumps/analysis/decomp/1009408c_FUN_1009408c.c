// entry 0x1009408c  FUN_1009408c  (1009408c)

void FUN_1009408c(int param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  char *pcVar2;
  undefined4 uVar3;
  byte bVar4;
  char *pcVar5;
  int iVar6;
  int local_18;
  undefined4 local_14;
  undefined4 uStack_10;
  
  pcVar2 = DAT_100941ac;
  cVar1 = *DAT_100941ac;
  local_18 = param_1;
  local_14 = param_2;
  uStack_10 = param_3;
  if (cVar1 == '\x14') {
    if (param_1 == DAT_100941b0) {
      FUN_1008dc40();
      return;
    }
LAB_1009412a:
    if (param_1 == DAT_100941cc) {
      return;
    }
  }
  else {
    if (cVar1 != '\x0e') {
      if (cVar1 == '\x03') {
        pcVar5 = DAT_100941bc;
        if (param_1 == DAT_100941b0) goto LAB_100940d6;
      }
      else {
        pcVar5 = DAT_100941c0;
        if ((cVar1 == '\x04') || (pcVar5 = DAT_100941c4, cVar1 == '\x05')) {
          if (param_1 == DAT_100941b0) goto LAB_100940d6;
          goto LAB_1009412a;
        }
      }
      iVar6 = *DAT_100941c8;
      if (cVar1 == '\x1a') {
        if (param_1 == DAT_100941b0) {
          FUN_10091244();
LAB_1009410a:
          uVar3 = *(undefined4 *)(iVar6 + 4);
          *pcVar2 = '\x01';
          lv_tileview_1(uVar3,1,0);
          return;
        }
      }
      else if (((cVar1 == '\x1c') || (cVar1 == '\x1d')) && (param_1 == DAT_100941b0))
      goto LAB_1009410a;
      goto LAB_1009412a;
    }
    if (param_1 == DAT_100941b0) {
      bVar4 = *DAT_100941b4 | 1;
LAB_100940b6:
      *DAT_100941b4 = bVar4;
      return;
    }
    if (param_1 == DAT_100941b8) {
      bVar4 = *DAT_100941b4 | 2;
      goto LAB_100940b6;
    }
    if (param_1 == DAT_100941cc) {
      FUN_1014c818(((int)PTR_DAT_100941f0 - (int)PTR_LAB_100941ec) * 0x20 & 0xff00U | 0x640031,
                   PTR_s_factory_test_view_key_event_hand_100941f4);
      local_18 = 1;
      ux2sys_api_8(0x25,&local_18,4);
      local_14 = 0;
      ux2sys_api_8(0x5d,&local_14);
      return;
    }
  }
  if (param_1 != DAT_100941d0) {
    return;
  }
  pcVar5 = DAT_100941d4;
  if ((((cVar1 != '\x06') && (pcVar5 = DAT_100941d8, cVar1 != '\a')) &&
      ((pcVar5 = DAT_100941dc, cVar1 != '\b' &&
       ((pcVar5 = DAT_100941e0, cVar1 != '\t' && (pcVar5 = DAT_100941e4, cVar1 != '\v')))))) &&
     (pcVar5 = DAT_100941e8, cVar1 != '\x0e')) {
    return;
  }
  if (*pcVar5 == '\0') {
    return;
  }
LAB_100940d6:
  *pcVar5 = '\0';
  return;
}


