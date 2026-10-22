
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int get_unum(void); // Existing binary helper

int store_number(unsigned int *data)
{
    printf(" Number: ");
    unsigned int number = get_unum();

    printf(" Index: ");
    unsigned int index = get_unum();

    if (index % 3 == 0 || ((number >> 8) & 0xff) == 0xb7)
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }

    data[index] = number; // No bounds checking!
    return 0;
}

int read_number(unsigned int *data)
{
    printf(" Index: ");
    unsigned int index = get_unum();

    printf(" Number at data[%u] is %u\n",
           index, data[index]); // No bounds checking!

    return 0;
}

int main(int argc, char **argv, char **envp)
{
    unsigned int data[100] = {0};
    char command[20] = {0};
    int status;

    // Clear arguments and environment variables
    for (int i = 0; argv[i]; i++)
        memset(argv[i], 0, strlen(argv[i]));

    for (int i = 0; envp[i]; i++)
        memset(envp[i], 0, strlen(envp[i]));

    puts("----------------------------------------------------");
    puts("  Welcome to wil's crappy number storage service!");
    puts("----------------------------------------------------");
    puts(" Commands: store, read, quit");
    puts("----------------------------------------------------");

    while (1)
    {
        printf("Input command: ");

        if (!fgets(command, sizeof(command), stdin))
            return 1;

        command[strcspn(command, "\n")] = '\0';
        status = 1;

        if (strncmp(command, "store", 5) == 0)
            status = store_number(data);
        else if (strncmp(command, "read", 4) == 0)
            status = read_number(data);
        else if (strncmp(command, "quit", 4) == 0)
            return 0;

        if (status)
            printf(" Failed to do %s command\n", command);
        else
            printf(" Completed %s command successfully\n", command);
    }
}
