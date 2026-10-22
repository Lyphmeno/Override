#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char input[100];

    fgets(input, sizeof(input), stdin);

    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
            input[i] ^= 0x20;
    }

    printf(input); // Format string vulnerability
    exit(0);
}
