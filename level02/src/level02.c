undefined8 main(void)

{
  int iVar1;
  size_t sVar2;
  long lVar3;
  undefined8 *puVar4;
  undefined8 local_118 [14];
  undefined8 local_a8 [6];
  undefined8 local_78 [12];
  int local_14;
  FILE *local_10;
  
  puVar4 = local_78;
  for (lVar3 = 0xc; lVar3 != 0; lVar3 = lVar3 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  *(undefined4 *)puVar4 = 0;
  puVar4 = local_a8;
  for (lVar3 = 5; lVar3 != 0; lVar3 = lVar3 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  *(undefined *)puVar4 = 0;
  puVar4 = local_118;
  for (lVar3 = 0xc; lVar3 != 0; lVar3 = lVar3 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  *(undefined4 *)puVar4 = 0;
  local_10 = (FILE *)0x0;
  local_14 = 0;
  local_10 = fopen("/home/users/level03/.pass","r");
  if (local_10 == (FILE *)0x0) {
    fwrite("ERROR: failed to open password file\n",1,0x24,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  sVar2 = fread(local_a8,1,0x29,local_10);
  local_14 = (int)sVar2;
  sVar2 = strcspn((char *)local_a8,"\n");
  *(undefined *)((long)local_a8 + sVar2) = 0;
  if (local_14 != 0x29) {
    fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
    fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  fclose(local_10);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets((char *)local_78,100,stdin);
  sVar2 = strcspn((char *)local_78,"\n");
  *(undefined *)((long)local_78 + sVar2) = 0;
  printf("--[ Password: ");
  fgets((char *)local_118,100,stdin);
  sVar2 = strcspn((char *)local_118,"\n");
  *(undefined *)((long)local_118 + sVar2) = 0;
  puts("*****************************************");
  iVar1 = strncmp((char *)local_a8,(char *)local_118,0x29);
  if (iVar1 == 0) {
    printf("Greetings, %s!\n",local_78);
    system("/bin/sh");
    return 0;
  }
  printf((char *)local_78);
  puts(" does not have access!");
                    /* WARNING: Subroutine does not return */
  exit(1);
}