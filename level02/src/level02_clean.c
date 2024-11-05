int main() {
    size_t passwordReadLen;
    FILE* passwordFile;
    int usernameLength, passwordLength;

    // Initialize buffers
    memset(local_78, 0, sizeof(local_78));
    memset(local_a8, 0, sizeof(local_a8));
    memset(local_118, 0, sizeof(local_118));

    // Open password file
    passwordFile = fopen("/home/users/level03/.pass", "r");
    if (passwordFile == NULL) {
        fprintf(stderr, "ERROR: failed to open password file\n");
        exit(1);
    }

    // Read password from file
    passwordReadLen = fread(local_a8, 1, 0x29, passwordFile);
    if (passwordReadLen != 0x29) {
        fprintf(stderr, "ERROR: failed to read password file\n");
        exit(1);
    }

    // Remove newline character from password
    local_a8[strcspn((char*)local_a8, "\n")] = 0;

    // Close password file
    fclose(passwordFile);

    // Display login prompt
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    // Get username
    printf("--[ Username: ");
    fgets((char*)local_78, 100, stdin);
    local_78[strcspn((char*)local_78, "\n")] = 0;

    // Get password
    printf("--[ Password: ");
    fgets((char*)local_118, 100, stdin);
    local_118[strcspn((char*)local_118, "\n")] = 0;

    puts("*****************************************");

    // Verify password
    if (strncmp((char*)local_a8, (char*)local_118, 0x29) == 0) {
        printf("Greetings, %s!\n", local_78);
        system("/bin/sh");
        return 0;
    } else {
		printf((char *)local_78);
		puts(" does not have access!");
		exit(1);
    }
}