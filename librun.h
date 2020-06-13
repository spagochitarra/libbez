
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

#define _SYSTEM_TIMEOUT 10

extern int system_timeout(const char *, int);
extern int system2(const char *);

#endif
