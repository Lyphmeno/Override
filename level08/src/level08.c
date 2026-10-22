#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void log_wrapper(FILE *log_file, const char *message, const char *filename)
{
    char buffer[264];

    strcpy(buffer, message);

    // Format string vulnerability
    snprintf(buffer + strlen(buffer),
             254 - strlen(buffer),
             filename);

    buffer[strcspn(buffer, "\n")] = '\0';

    fprintf(log_file, "LOG: %s\n", buffer);
}

int main(int argc, char **argv)
{
    char backup_path[104];
    char byte;

    if (argc != 2)
        printf("Usage: %s filename\n", argv[0]);

    FILE *log_file = fopen("./backups/.log", "w");
    if (log_file == NULL)
    {
        printf("ERROR: Failed to open ./backups/.log\n");
        exit(1);
    }

    log_wrapper(log_file, "Starting back up: ", argv[1]);

    FILE *source = fopen(argv[1], "r");
    if (source == NULL)
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    strcpy(backup_path, "./backups/");
    strncat(backup_path, argv[1], 99 - strlen(backup_path));

    int backup_fd = open(backup_path, O_WRONLY | O_CREAT | O_EXCL, 0660);
    if (backup_fd < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }

    while (1)
    {
        byte = (char)fgetc(source);

        if (byte == -1)
            break;

        write(backup_fd, &byte, 1);
    }

    log_wrapper(log_file, "Finished back up ", argv[1]);

    fclose(source);
    close(backup_fd);

    return 0;
}
