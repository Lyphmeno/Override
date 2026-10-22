#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int decrypt(char key)
{
    char password[] = "Q}|u`sfg~sf{}|a3";

    for (int i = 0; password[i]; i++)
        password[i] ^= key;

    if (strcmp(password, "Congratulations!") == 0)
        return system("/bin/sh");

    return puts("\nInvalid Password");
}

int test(int input, int value)
{
    int key = value - input;

    if ((key >= 1 && key <= 9) ||
        (key >= 16 && key <= 21))
        return decrypt(key);

    return decrypt(rand());
}

int main(void)
{
    int password;

    srand(time(NULL));

    puts("***********************************");
    puts("*               level03         **");
    puts("***********************************");

    printf("Password:");
    scanf("%d", &password);

    test(password, 322424845);

    return 0;
}
