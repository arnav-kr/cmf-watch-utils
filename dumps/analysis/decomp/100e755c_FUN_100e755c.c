// 0x10227f24 FUN_100e755c :: _alarmTask_proess #1 update tm is same return!!!\n
// entry 0x100e755c  FUN_100e755c

undefined1 FUN_100e755c(void)

{
  undefined1 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined1 *puVar7;
  undefined4 *puVar8;
  uint uVar9;
  undefined4 local_30;
  undefined4 local_2c;
  undefined2 local_28;
  undefined1 uStack_24;
  char local_23;
  short local_22;
  byte local_20;
  short local_1e;
  
  FUN_100c4068(&uStack_24);
  FUN_100c14b4(&local_30);
  FUN_100c14f4(&local_30);
  uVar9 = ((int)PTR_DAT_100e76dc - (int)PTR_LAB_100e76e0) * 0x20 & 0xff00;
  FUN_1014c818(uVar9 | 0x6b0032,PTR_s__alarmTask_proess__1_tm_tm_hour__100e76e4,local_30._2_1_,
               local_30._1_1_);
  FUN_100e738c();
  puVar2 = DAT_100e76e8;
  if ((((*(short *)((int)DAT_100e76e8 + 6) == local_2c._2_2_) &&
       (*(char *)(DAT_100e76e8 + 1) == (char)local_2c)) &&
      (*(short *)((int)DAT_100e76e8 + 2) == local_30._2_2_)) &&
     (*(char *)((int)DAT_100e76e8 + 1) == local_30._1_1_)) {
    *DAT_100e76e8 = local_30;
    puVar2[1] = local_2c;
    *(undefined2 *)(puVar2 + 2) = local_28;
  }
  else {
    *DAT_100e76e8 = local_30;
    *(undefined2 *)(puVar2 + 2) = local_28;
    puVar2[1] = local_2c;
    FUN_100c407c(0);
    puVar1 = DAT_100e76f0;
    if (((local_2c._2_2_ == local_1e) && ((local_2c & 0xff) == (uint)local_20)) &&
       ((local_30._2_2_ == local_22 && (local_23 == local_30._1_1_)))) {
      FUN_1014c818(uVar9 | 0x7e0031,DAT_100e76ec);
    }
    else {
      if (local_2c._1_1_ == '\0') {
        local_2c._1_1_ = '\a';
      }
      iVar4 = 0;
      puVar7 = DAT_100e76f0;
      do {
        if ((((puVar7[0x5c] == local_30._2_1_) && (puVar7[0x5d] == local_30._1_1_)) &&
            ((puVar7[0x5e] == '\x01' && ((puVar7[0x61] != '\0' && (-1 < (char)puVar7[0x60])))))) &&
           (((byte)puVar7[0x60] == 0 ||
            (((uint)(byte)puVar7[0x60] & 1 << (uint)(byte)(local_2c._1_1_ - 1)) != 0)))) {
          puVar5 = (undefined4 *)(DAT_100e76f0 + iVar4 * 0x2a + 0x80);
          puVar2 = DAT_100e76f4;
          puVar8 = (undefined4 *)(DAT_100e76f0 + iVar4 * 0x2a + 0x58);
          do {
            puVar6 = puVar8 + 1;
            puVar3 = puVar2 + 1;
            *puVar2 = *puVar8;
            puVar2 = puVar3;
            puVar8 = puVar6;
          } while (puVar6 != puVar5);
          *(undefined2 *)puVar3 = *(undefined2 *)puVar6;
          *puVar1 = 0;
          return 1;
        }
        iVar4 = iVar4 + 1;
        puVar7 = puVar7 + 0x2a;
      } while (iVar4 != 5);
      if (((DAT_100e76f0[6] == local_30._2_1_) && (DAT_100e76f0[7] == local_30._1_1_)) &&
         (DAT_100e76f0[8] == '\x01')) {
        if (DAT_100e76f0[0xb] == '\0') {
          return 0;
        }
        puVar5 = DAT_100e76f8 + 10;
        puVar2 = (undefined4 *)((int)DAT_100e76f8 + 0x2a);
        puVar8 = DAT_100e76f8;
        do {
          puVar6 = puVar8 + 1;
          puVar3 = puVar2 + 1;
          *puVar2 = *puVar8;
          puVar2 = puVar3;
          puVar8 = puVar6;
        } while (puVar6 != puVar5);
        *(undefined2 *)puVar3 = *(undefined2 *)puVar6;
        *puVar1 = 1;
        return 1;
      }
    }
  }
  return 0;
}


