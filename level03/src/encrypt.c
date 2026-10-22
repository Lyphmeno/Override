#include <stdio.h>

int main(void)
{
char encrypted[] = "Q}|u`sfg~sf|a3";
char key = 'Q' ^ 'C';

for (int i = 0; encrypted[i]; i++)
        encrypted[i] ^= key;

printf("Key: %d (0x%x)\n", key, key);
    printf("Decrypted: %s\n", encrypted);

return 0;
}