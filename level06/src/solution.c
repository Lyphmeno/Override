
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *login = "aaaaaa";
    int serial = (login[3] ^ 0x1337) + 6221293;

    for (int i = 0; i < strlen(login); i++)
        serial += (serial ^ (unsigned int)login[i]) % 0x539;

    printf("Serial: %d\n", serial);
    return 0;
}
