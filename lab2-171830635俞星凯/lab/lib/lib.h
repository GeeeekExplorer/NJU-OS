#ifndef __lib_h__
#define __lib_h__

#define SYS_WRITE 0
#define SYS_READ 1

#define STD_OUT 0
#define STD_IN 1

#define MAX_BUFFER_SIZE 256
//#define MAX_BUFFER_SIZE 1

int printf(const char *format,...);

int scanf(const char *format,...);

#endif
