#ifndef __lib_h__
#define __lib_h__

#include "types.h"

#define SYS_WRITE 0
#define SYS_FORK 1
#define SYS_EXEC 2
#define SYS_SLEEP 3
#define SYS_EXIT 4

#define STD_OUT 0

#define MAX_BUFFER_SIZE 256

int printf(const char *format,...);

pid_t fork();

int exec(void (*func)(void));

int sleep(uint32_t time);

int exit();

#endif
