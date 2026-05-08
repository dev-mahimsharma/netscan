#ifndef NETLENS_H
#define NETLENS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
typedef HANDLE             nl_thread_t;
typedef CRITICAL_SECTION   nl_mutex_t;
typedef CONDITION_VARIABLE nl_cond_t;
#else
#include <pthread.h>
typedef pthread_t       nl_thread_t;
typedef pthread_mutex_t nl_mutex_t;
typedef pthread_cond_t  nl_cond_t;
#endif

// Defines an IP address range
typedef struct {
    uint32_t start_ip;
    uint32_t end_ip;
} ip_range_t;

// Port information
typedef struct {
    uint16_t    port;
    const char *service;
} port_info_t;

// Worker queue structures
typedef struct {
    uint32_t ip;
    uint16_t port;
    const char *service;
} scan_task_t;

typedef struct {
    uint32_t ip;
    size_t   port_info_count;
    size_t   open_count;
    size_t   closed_count;
} scan_summary_t;

typedef struct worker_queue_s worker_queue_t;

// Core functions
bool netlens_init(void);
void netlens_cleanup(void);

bool parse_ip(const char *ip_str, uint32_t *ip_out);
void format_ip(uint32_t ip, char *buffer, size_t size);

// Port definitions
extern const port_info_t EDUCATIONAL_PORTS[];
extern const size_t      EDUCATIONAL_PORTS_COUNT;

// Scanner functions
bool check_tcp_port(uint32_t ip, uint16_t port, int timeout_ms);

// Worker queue functions
worker_queue_t *worker_queue_create(int num_workers, scan_summary_t *summaries, uint32_t start_ip);
void            worker_queue_add(worker_queue_t *queue, scan_task_t task);
void            worker_queue_destroy(worker_queue_t *queue);

#endif // NETLENS_H
