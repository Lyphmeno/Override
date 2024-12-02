#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int i; // [esp+14h] [ebp-14h]
  int pass; // [esp+18h] [ebp-10h]
  int len; // [esp+1Ch] [ebp-Ch]
  char *login = "aaaaaa";

  len = strnlen(login, 32);
  if ( len <= 5 )
    return 1;
  else
  {
    pass = (login[3] ^ 0x1337) + 6221293;
    for ( i = 0; i < len; ++i )
    {
      if ( login[i] <= 31 )
        return 1;
      pass += (pass ^ (unsigned int)login[i]) % 0x539;
    }
    printf("pass = %d\n", pass);
	return (0);
  }
}