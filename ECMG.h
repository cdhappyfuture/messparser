#ifndef _ECMG_H_
#define _ECMG_H_
#include "myheaders.h"

#define MAX_SOCKETS 10
#define LISTEN_PORT 6667

void work_with_client(int);
void* thread_fun(void*);

#endif
