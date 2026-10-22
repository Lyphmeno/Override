#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char message[140];
    char username[40];
    int max_length;
} Message;

void secret_backdoor(void)
{
    char command[128];

    fgets(command, sizeof(command), stdin);
    system(command);
}

void set_username(Message *data)
{
    char input[128] = {0};

    puts(">: Enter your username");
    printf(">>: ");
    fgets(input, sizeof(input), stdin);

    for (int i = 0; i <= 40 && input[i]; i++)
    {
        // Copies up to 41 bytes into a 40-byte field
        ((char *)data)[140 + i] = input[i];
    }

    printf(">: Welcome, %s", data->username);
}

void set_msg(Message *data)
{
    char input[1024] = {0};

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(input, sizeof(input), stdin);

    // Length can be corrupted by set_username()
    strncpy(data->message, input, data->max_length);
}

void handle_msg(void)
{
    Message data = {0};
    data.max_length = 140;

    set_username(&data);
    set_msg(&data);

    puts(">: Msg sent!");
}

int main(void)
{
    puts("--------------------------------------------\n"
         "|   ~Welcome to l33t-m$n ~    v1337        |\n"
         "--------------------------------------------");

    handle_msg();
    return 0;
}
