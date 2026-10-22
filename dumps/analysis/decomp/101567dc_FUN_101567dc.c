// entry 0x101567dc  FUN_101567dc

void FUN_101567dc(undefined4 param_1)

{
  if (*DAT_101567fc == 0) {
    FUN_1014c818(DAT_10156808 | (DAT_10156800 - DAT_10156804) * 0x20 & 0xff00U,DAT_1015680c);
    return;
  }
  *(undefined4 *)(*DAT_101567fc + 0x40) = param_1;
  return;
}


