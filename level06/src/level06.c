
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *login, int serial)
{
    login[strcspn(login, "\n")] = '\0';
    int length = strnlen(login, 32);

    if (length < 6)
        return 1;

    if (ptrace(PTRACE_TRACEME, 0, (void *)1, 0) == -1)
    {
        puts("TAMPERING DETECTED!");
        return 1;
    }

    int expected = (login[3] ^ 0x1337) + 6221293;

    for (int i = 0; i < length; i++)
    {
        if (login[i] <= 31)
            return 1;

        expected += (expected ^ (unsigned int)login[i]) % 0x539;
    }

    return serial != expected;
}

int main(void)
{
    char login[28];
    int serial;

    puts("***********************************");
    puts("*               level06           *");
    puts("***********************************");

    printf("-> Enter Login: ");
    fgets(login, 32, stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");

    printf("-> Enter Serial: ");
    scanf("%d", &serial);

    if (auth(login, serial))
        return 1;

    puts("Authenticated!");
    system("/bin/sh");

    return 0;
}
