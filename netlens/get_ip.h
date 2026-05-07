#ifndef GET_IP_H
#define GET_IP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct T T;

#define GET_IP(c) (get_local_ips((c)))

extern void get_local_ips(T *cmds);

#ifdef __cplusplus
}
#endif

#endif // NETLENS_GET_IP_H
