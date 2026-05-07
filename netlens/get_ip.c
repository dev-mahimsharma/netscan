#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

void get_local_ips(T *cmds) {
    char from_ip[16];
    char to_ip[16];
    char loop_ip[16];
    char prefix[16];

    strncpy(from_ip, cmds->from, sizeof(from_ip) - 1);
    from_ip[sizeof(from_ip) - 1] = '\0';

    strncpy(to_ip, cmds->to, sizeof(to_ip) - 1);
    to_ip[sizeof(to_ip) - 1] = '\0';

    char *last_dot_from_ip = strrchr(from_ip, '.');
    char *last_dot_to_ip = strrchr(to_ip, '.');

    if (last_dot_from_ip == NULL || last_dot_to_ip == NULL) {
        printf("Invalid IP\n");
        return;
    }

    int last_from_ip_part = atoi(last_dot_from_ip + 1);
    int last_to_ip_part = atoi(last_dot_to_ip + 1);

    if (last_from_ip_part < 1 || last_from_ip_part > 254 ||
        last_to_ip_part < 1 || last_to_ip_part > 254 ||
        last_from_ip_part > last_to_ip_part) {
        printf("Invalid IP range\n");
        return;
    }

    size_t prefix_len = (size_t)(last_dot_from_ip - from_ip);

    if (prefix_len >= sizeof(prefix)) {
        printf("IP prefix too long\n");
        return;
    }

    strncpy(prefix, from_ip, prefix_len);
    prefix[prefix_len] = '\0';

    for (int i = last_from_ip_part; i <= last_to_ip_part; i++) {
        snprintf(loop_ip, sizeof(loop_ip), "%s.%d", prefix, i);
        printf("%s\n", loop_ip);
    }
}