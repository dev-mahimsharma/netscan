#include "netlens.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_WORKERS 10

void print_usage(const char *prog_name) {
    printf("NetLens - Educational Local TCP Checker\n");
    printf("Usage: %s <start_ip> <end_ip>\n", prog_name);
    printf("Example: %s 192.168.1.1 192.168.1.254\n\n", prog_name);
    printf("WARNING: NetLens is strictly for local private networks.\n");
    printf("Do not use on public IPs or networks you do not own.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (!netlens_init()) {
        fprintf(stderr, "Error: Failed to initialize network subsystem.\n");
        return EXIT_FAILURE;
    }

    uint32_t start_ip, end_ip;
    if (!parse_ip(argv[1], &start_ip) || !parse_ip(argv[2], &end_ip)) {
        fprintf(stderr, "Error: Invalid IP address format.\n");
        netlens_cleanup();
        return EXIT_FAILURE;
    }

    if (start_ip > end_ip) {
        fprintf(stderr, "Error: start_ip must be less than or equal to end_ip.\n");
        netlens_cleanup();
        return EXIT_FAILURE;
    }

    // Basic check to discourage scanning large public ranges
    // (In a real app, we'd do strict RFC 1918 checking here)
    if (end_ip - start_ip > 1024) {
        fprintf(stderr, "Error: Range too large. Please limit to a small local subnet.\n");
        netlens_cleanup();
        return EXIT_FAILURE;
    }

    printf("Starting NetLens educational scan...\n");
    uint32_t total_ips = end_ip - start_ip + 1;
    char     start_ip_str[16];
    char     end_ip_str[16];
    format_ip(start_ip, start_ip_str, sizeof(start_ip_str));
    format_ip(end_ip, end_ip_str, sizeof(end_ip_str));
    printf("Checking %s to %s (total %u IPs)\n", start_ip_str, end_ip_str, total_ips);
    printf("Using %zu port_info_t entries from src/ports.c\n", EDUCATIONAL_PORTS_COUNT);

    scan_summary_t *summaries = calloc(total_ips, sizeof(scan_summary_t));
    if (!summaries) {
        fprintf(stderr, "Error: Failed to allocate scan summaries.\n");
        netlens_cleanup();
        return EXIT_FAILURE;
    }

    for (uint32_t i = 0; i < total_ips; i++) {
        summaries[i].ip              = start_ip + i;
        summaries[i].port_info_count = EDUCATIONAL_PORTS_COUNT;
    }

    worker_queue_t *queue = worker_queue_create(MAX_WORKERS, summaries, start_ip);
    if (!queue) {
        fprintf(stderr, "Error: Failed to create worker queue.\n");
        free(summaries);
        netlens_cleanup();
        return EXIT_FAILURE;
    }

    for (uint32_t i = 0; i < total_ips; i++) {
        uint32_t ip = start_ip + i;
        for (size_t p = 0; p < EDUCATIONAL_PORTS_COUNT; p++) {
            scan_task_t task;
            task.ip   = ip;
            task.port = EDUCATIONAL_PORTS[p].port;
            worker_queue_add(queue, task);
        }
    }

    worker_queue_destroy(queue);

    printf("\nScan summary by address:\n");
    for (uint32_t i = 0; i < total_ips; i++) {
        char ip_str[16];
        format_ip(summaries[i].ip, ip_str, sizeof(ip_str));
        printf("%s: open=%zu closed=%zu port_info_t=%zu\n", ip_str, summaries[i].open_count,
               summaries[i].closed_count, summaries[i].port_info_count);
    }

    free(summaries);
    printf("Scan completed.\n");

    netlens_cleanup();
    return EXIT_SUCCESS;
}
