#include <stdio.h>
#include <ctype.h>

void decrypt_stored_pass(char *stored_pass, char ctx) {
    char decrypted_pass[17];
    int i;
    int readable = 1;

    // Decrypt each character by XORing with ctx
    for (i = 0; stored_pass[i] != '\0'; i++) {
        decrypted_pass[i] = stored_pass[i] ^ ctx;
        // Check if each character is printable ASCII
        if (!isprint((unsigned char)decrypted_pass[i])) {
            readable = 0;  // Mark as not readable
        }
    }
    decrypted_pass[i] = '\0';

    // Print if the result is a readable ASCII string
    if (readable) {
        printf("Decrypted password with ctx = 0x%02x: %s\n", ctx, decrypted_pass);
    }
}

int main() {
    // The encrypted password array
    char stored_pass[] = {0x51, 0x7d, 0x7c, 0x75, 0x60, 0x73, 0x66, 0x67, 
                          0x7e, 0x73, 0x66, 0x7b, 0x7d, 0x7c, 0x61, 0x33, 0};

    // Test all possible ctx values (0 to 255)
    for (int ctx = 0; ctx < 20; ctx++) {
        decrypt_stored_pass(stored_pass, (char)ctx);
    }

    return 0;
}
