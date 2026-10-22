
#include <stdio.h>
#include <string.h>

char username[256];

int verify_user_name(void)
{
    puts("verifying username....\n");
    return strncmp(username, "dat_wil", 7);
}

int verify_user_pass(const char *password)
{
    return strncmp(password, "admin", 5);
}

int main(void)
{
    char password[64] = {0};

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(username, sizeof(username), stdin);

    if (verify_user_name() != 0)
    {
        puts("nope, incorrect username...\n");
        return 1;
    }

    puts("Enter Password: ");
    fgets(password, 100, stdin); // Buffer overflow

    verify_user_pass(password);

    // The original program always rejects the password.
    puts("nope, incorrect password...\n");
    return 1;
}
