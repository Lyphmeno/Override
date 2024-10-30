int verifyPass(byte* pw) {
    int len = 5;
    byte* admin = (byte*)"admin";

    while (len > 0) {
        len--;
        bool pwLT = *pw < *admin;
        bool pwEQ = *pw == *admin;
        pw++;
        admin++;
        
        if (!pwEQ) {
            return pwLT ? -1 : 1;
        }
    }

    return 0;
}

int verifyUser() {
    puts("verifying username....");

    int len = 7;
    byte* user = &a_user_name;
    byte* expected = (byte*)"dat_wil";
    bool userLT, userEQ;

    while (len > 0) {
        len--;
        userLT = *user < *expected;
        userEQ = *user == *expected;
        user += (userLT || userEQ) ? 1 : -1;
        expected += (userLT || userEQ) ? 1 : -1;
        
        if (!userEQ) {
            return userLT ? -1 : 1;
        }
    }

    return 0;
}

int main() {
    memset(local_54, 0, sizeof(local_54));
    int status = 0;

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(&a_user_name, 0x100, stdin);
    status = verifyUser();

    if (status == 0) {
        puts("Enter Password: ");
        fgets((char*)local_54, 100, stdin);
        status = verifyPass(local_54);

        if (status == 0) {
            puts("Success! Logged in as admin.");
            return 0;
        } else {
            puts("nope, incorrect password...");
            return 1;
        }
    } else {
        puts("nope, incorrect username...");
        return 1;
    }
}