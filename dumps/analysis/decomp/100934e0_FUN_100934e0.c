// entry 0x100934e0  FUN_100934e0  (100934e0)

void FUN_100934e0(uint param_1,undefined4 param_2,undefined4 param_3,uint param_4,undefined4 param_5
                 ,uint param_6)

{
  char *pcVar1;
  undefined1 *puVar2;
  undefined1 uVar3;
  int unaff_r4;
  int unaff_r5;
  
  FUN_1014c818(param_1 | param_4 & 0xff00,DAT_10093560);
  pcVar1 = DAT_10093564;
  if (unaff_r4 - 1U < 2) {
    *(bool *)(DAT_10093568 + 0xf) = unaff_r4 == 1;
    if (*pcVar1 != '\0') {
      *pcVar1 = '\0';
      param_6 = 0;
      ux2sys_api_9(0x1a,&param_6);
      param_6 = param_6 & 0xfffffeff;
      ux2sys_api_8(0x1a,&param_6);
    }
    puVar2 = DAT_1009356c;
    if (*(char *)(unaff_r5 + 0xb4) == '\0') {
      FUN_100924ec();
      uVar3 = *puVar2;
    }
    else {
      uVar3 = 0x12;
      *DAT_1009356c = 0x12;
    }
    lv_tileview_1(*(undefined4 *)(unaff_r5 + 4),uVar3,0,0);
  }
  return;
}


