// 0x102382f7 FUN_101161b8 ::  cst820 update success \n
// entry 0x101161b8  FUN_101161b8

int FUN_101161b8(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  uint uVar6;
  undefined2 local_b8 [2];
  undefined4 local_b4;
  undefined1 auStack_b0 [140];
  
  iVar4 = 0;
  local_b4 = 0;
  FUN_101bf3b2(auStack_b0,0,0x88);
  local_b8[0] = 0;
  iVar2 = FUN_10115ff0();
  iVar1 = DAT_10116300;
  if (iVar2 != -1) {
    iVar3 = DAT_101162f4 - DAT_101162f8;
    iVar2 = DAT_10116304;
    do {
      local_b4 = CONCAT13((char)((uint)iVar4 >> 8),0x14a0);
      FUN_101c07a4(*(undefined4 *)(iVar1 + 4),&local_b4,4,0x6a);
      FUN_101c0772(2);
      uVar6 = 0xa018;
      do {
        local_b4 = CONCAT22(local_b4._2_2_,
                            (ushort)((uVar6 & 0xff) << 8) | (ushort)(uVar6 >> 8) & 0xff);
        FUN_101bf3aa((int)&local_b4 + 2,iVar2 + -0xa018 + uVar6,0x80);
        FUN_101c07a4(*(undefined4 *)(iVar1 + 4),&local_b4,0x82,0x6a);
        FUN_101c0772(2);
        uVar6 = uVar6 + 0x80;
      } while (uVar6 != 0xa218);
      FUN_101c0772(4);
      local_b4 = CONCAT13(local_b4._3_1_,0xee04a0);
      FUN_101c07a4(*(undefined4 *)(iVar1 + 4),&local_b4,3,0x6a);
      FUN_101c0772(param_1);
      cVar5 = '3';
      while( true ) {
        cVar5 = cVar5 + -1;
        if (cVar5 == '\0') break;
        local_b4 = CONCAT22(local_b4._2_2_,0x5a0);
        FUN_101c0738(*(undefined4 *)(iVar1 + 4),0x6a,&local_b4,2,local_b8,1);
        if ((char)local_b8[0] == 'U') {
          FUN_1014c818(iVar3 * 0x20 & 0xff00U | 0x1300031,DAT_101162fc);
          break;
        }
        FUN_101c0772(10);
      }
      iVar4 = iVar4 + 0x200;
      iVar2 = iVar2 + 0x200;
    } while (iVar4 != 0x3c00);
    local_b4 = CONCAT13(local_b4._3_1_,0x3a0);
    FUN_101c07a4(*(undefined4 *)(iVar1 + 4),&local_b4,3,0x6a);
    iVar2 = 0;
  }
  return iVar2;
}


