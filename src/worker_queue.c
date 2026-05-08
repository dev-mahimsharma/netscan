#include "netlens.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct task_node {
    scan_task_t       task;
    struct task_node *next;
} task_node_t;

struct worker_queue_s {
    task_node_t *head;
    task_node_t *tail;
    nl_mutex_t   mutex;
    nl_cond_t    cond;
    nl_thread_t *workers;
    scan_summary_t *summaries;
    uint32_t     start_ip;
    int          num_workers;
    bool         stop;
};

#ifdef _WIN32
static DWORD WINAPI worker_thread(LPVOID arg) {
#else
static void *worker_thread(void *arg) {
#endif
    worker_queue_t *queue = (worker_queue_t *)arg;

    while (1) {
#ifdef _WIN32
        EnterCriticalSection(&queue->mutex);
        while (queue->head == NULL && !queue->stop) {
            SleepConditionVariableCS(&queue->cond, &queue->mutex, INFINITE);
        }
#else
        pthread_mutex_lock(&queue->mutex);
        while (queue->head == NULL && !queue->stop) {
            pthread_cond_wait(&queue->cond, &queue->mutex);
        }
#endif

        if (queue->stop && queue->head == NULL) {
#ifdef _WIN32
            LeaveCriticalSection(&queue->mutex);
#else
            pthread_mutex_unlock(&queue->mutex);
#endif
            break;
        }

        task_node_t *node = queue->head;
        queue->head       = node->next;
        if (queue->head == NULL) {
            queue->tail = NULL;
        }

#ifdef _WIN32
        LeaveCriticalSection(&queue->mutex);
#else
        pthread_mutex_unlock(&queue->mutex);
#endif

        scan_task_t task = node->task;
        free(node);

        // Perform the task
        bool is_open = check_tcp_port(task.ip, task.port, 500); // 500ms timeout

#ifdef _WIN32
        EnterCriticalSection(&queue->mutex);
#else
        pthread_mutex_lock(&queue->mutex);
#endif
        scan_summary_t *summary = &queue->summaries[task.ip - queue->start_ip];
        if (is_open) {
            summary->open_count++;
        } else {
            summary->closed_count++;
        }

        char ip_str[16];
        format_ip(task.ip, ip_str, sizeof(ip_str));
        printf("%s:%u is %s\n", ip_str, task.port, is_open ? "OPEN" : "CLOSED");
        fflush(stdout);
#ifdef _WIN32
        LeaveCriticalSection(&queue->mutex);
#else
        pthread_mutex_unlock(&queue->mutex);
#endif
    }

#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

worker_queue_t *worker_queue_create(int num_workers, scan_summary_t *summaries, uint32_t start_ip) {
    worker_queue_t *queue = malloc(sizeof(worker_queue_t));
    if (!queue)
        return NULL;

    queue->head        = NULL;
    queue->tail        = NULL;
    queue->stop        = false;
    queue->summaries   = summaries;
    queue->start_ip    = start_ip;
    queue->num_workers = num_workers;

#ifdef _WIN32
    InitializeCriticalSection(&queue->mutex);
    InitializeConditionVariable(&queue->cond);
    queue->workers = malloc(sizeof(nl_thread_t) * num_workers);
    for (int i = 0; i < num_workers; i++) {
        queue->workers[i] = CreateThread(NULL, 0, worker_thread, queue, 0, NULL);
    }
#else
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
    queue->workers = malloc(sizeof(nl_thread_t) * num_workers);
    for (int i = 0; i < num_workers; i++) {
        pthread_create(&queue->workers[i], NULL, worker_thread, queue);
    }
#endif

    return queue;
}

void worker_queue_add(worker_queue_t *queue, scan_task_t task) {
    task_node_t *node = malloc(sizeof(task_node_t));
    node->task        = task;
    node->next        = NULL;

#ifdef _WIN32
    EnterCriticalSection(&queue->mutex);
    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail       = node;
    }
    WakeConditionVariable(&queue->cond);
    LeaveCriticalSection(&queue->mutex);
#else
    pthread_mutex_lock(&queue->mutex);
    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail       = node;
    }
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
#endif
}

void worker_queue_destroy(worker_queue_t *queue) {
#ifdef _WIN32
    EnterCriticalSection(&queue->mutex);
    queue->stop = true;
    WakeAllConditionVariable(&queue->cond);
    LeaveCriticalSection(&queue->mutex);

    WaitForMultipleObjects(queue->num_workers, queue->workers, TRUE, INFINITE);

    for (int i = 0; i < queue->num_workers; i++) {
        CloseHandle(queue->workers[i]);
    }

    DeleteCriticalSection(&queue->mutex);
#else
    pthread_mutex_lock(&queue->mutex);
    queue->stop = true;
    pthread_cond_broadcast(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);

    for (int i = 0; i < queue->num_workers; i++) {
        pthread_join(queue->workers[i], NULL);
    }

    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
#endif

    free(queue->workers);
    free(queue);
}
