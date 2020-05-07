
#if !defined(_LIB_RUN_)
#define _LIB_RUN_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

extern FILE *popen_read(const char *, int);
extern int popen_close(FILE *);

#endif
