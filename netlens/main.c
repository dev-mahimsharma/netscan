#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "get_ip.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <from-ip> <to-ip> <mode>\n", argv[0]);
        fprintf(stderr, "Example: %s 192.168.1.1 192.168.1.254 --common\n", argv[0]);
        return EXIT_FAILURE;
    }

    T *cmds = malloc(sizeof(T));

    if (cmds == NULL) {
        fprintf(stderr, "memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    snprintf(cmds->program_name, sizeof(cmds->program_name), "%s", argv[0]);
    snprintf(cmds->from, sizeof(cmds->from), "%s", argv[1]);
    snprintf(cmds->to, sizeof(cmds->to), "%s", argv[2]);
    snprintf(cmds->feature, sizeof(cmds->feature), "%s", argv[3]);

    GET_IP(cmds);

    free(cmds);

    return 0;
}