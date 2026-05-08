#include "netlens.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_ip_parsing() {
    uint32_t ip;
    assert(parse_ip("192.168.1.1", &ip) == true);
    // 192.168.1.1 = C0 A8 01 01 = 3232235777
    assert(ip == 3232235777);

    assert(parse_ip("invalid_ip", &ip) == false);
    assert(parse_ip("256.256.256.256", &ip) == false);
}

void test_ip_formatting() {
    char buffer[16];
    format_ip(3232235777, buffer, sizeof(buffer));
    assert(strcmp(buffer, "192.168.1.1") == 0);
}

int main() {
    printf("Running unit tests...\n");

    if (!netlens_init()) {
        printf("Failed to initialize network subsystem.\n");
        return 1;
    }

    test_ip_parsing();
    test_ip_formatting();

    netlens_cleanup();

    printf("All tests passed.\n");
    return 0;
}
