int verify_user_pass(byte *param_1)
{
  int iVar1;
  byte *pbVar2;
  undefined in_CF;
  undefined in_ZF;
  
  iVar1 = 5;
  pbVar2 = (byte *)"admin";
  do {
    if (iVar1 == 0) break;
    iVar1 = iVar1 + -1;
    in_CF = *param_1 < *pbVar2;
    in_ZF = *param_1 == *pbVar2;
    param_1 = param_1 + 1;
    pbVar2 = pbVar2 + 1;
  } while ((bool)in_ZF);
  return (int)(char)((!(bool)in_CF && !(bool)in_ZF) - in_CF);
}

int verify_user_name(void)
{
  int iVar1;
  byte *pbVar2;
  byte *pbVar3;
  undefined uVar4;
  undefined uVar5;
  byte bVar6;
  
  bVar6 = 0;
  uVar4 = &stack0xfffffff4 < (undefined *)0x10;
  uVar5 = &stack0x00000000 == (undefined *)0x1c;
  puts("verifying username....\n");
  iVar1 = 7;
  pbVar2 = &a_user_name;
  pbVar3 = (byte *)"dat_wil";
  do {
    if (iVar1 == 0) break;
    iVar1 = iVar1 + -1;
    uVar4 = *pbVar2 < *pbVar3;
    uVar5 = *pbVar2 == *pbVar3;
    pbVar2 = pbVar2 + (uint)bVar6 * -2 + 1;
    pbVar3 = pbVar3 + (uint)bVar6 * -2 + 1;
  } while ((bool)uVar5);
  return (int)(char)((!(bool)uVar4 && !(bool)uVar5) - uVar4);
}


undefined4 main(void)
{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 local_54 [16];
  int local_14;
  
  puVar3 = local_54;
  for (iVar2 = 0x10; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  local_14 = 0;
  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(&a_user_name,0x100,stdin);
  local_14 = verify_user_name();
  if (local_14 == 0) {
    puts("Enter Password: ");
    fgets((char *)local_54,100,stdin);
    local_14 = verify_user_pass(local_54);
    if ((local_14 == 0) || (local_14 != 0)) {
      puts("nope, incorrect password...\n");
      uVar1 = 1;
    }
    else {
      uVar1 = 0;
    }
  }
  else {
    puts("nope, incorrect username...\n");
    uVar1 = 1;
  }
  return uVar1;
}
