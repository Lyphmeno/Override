
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char username[100] = {0};
    char password[100] = {0};
    char stored_password[48] = {0};

    FILE *file = fopen("/home/users/level03/.pass", "r");

    if (file == NULL)
    {
        fprintf(stderr, "ERROR: failed to open password file\n");
        exit(1);
    }

    size_t bytes_read = fread(stored_password, 1, 41, file);
    stored_password[strcspn(stored_password, "\n")] = '\0';

    if (bytes_read != 41)
    {
        fprintf(stderr, "ERROR: failed to read password file\n");
        exit(1);
    }

    fclose(file);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\***************************************/");

    printf("--[ Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("--[ Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    puts("*****************************************");

    if (strncmp(stored_password, password, 41) == 0)
    {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");
        return 0;
    }

    printf(username); // Format string vulnerability
    puts(" does not have access!");

    return 1;
}
