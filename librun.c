
/* No thread safe
 */

#include "librun.h"

static FILE *fp;
static pid_t pid;
static char debug;

/* This is equivalent to popen() with r parameter but we
 * also have socketpair() control in case the child hangs
 */
FILE *
popen_read(const char *cmd, int timeout)
{
    int s[2];
    int cc;
    char *p;

    debug = 0;
    if ((p = getenv("RUN_DEBUG")))
        debug = 1;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, s) < 0) {
        perror("socketpair()");
        return NULL;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(s[0]);
        close(s[1]);
        return NULL;
    }

    if (pid == 0) {
        if (debug)
            fprintf(stderr, "\
%s: child %d to exec() cmd %s\n", __func__, getpid(), cmd);
        /* child run command
         */
        close(s[0]);
        dup2(s[1], 1);
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        exit(-1);
    }

    fd_set rmask;
    close(s[1]);
    FD_ZERO(&rmask);
    FD_SET(s[0], &rmask);

    if (timeout < 0)
        timeout = 10;

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    if (debug)
        fprintf(stderr, "\
%s: parent %d to select() timeout %d\n", __func__, getpid(), timeout);

    cc = select(s[0] + 1, &rmask, NULL, NULL, &tv);
    if (cc < 0) {
        perror("select()");
        close(s[0]);
        kill(pid, SIGTERM);
        return NULL;
    }

    if (cc == 0) {
        if (debug)
            fprintf(stderr, "%s: select() timed out kill %d\n", __func__, pid);
        kill(pid, SIGTERM);
        close(s[0]);
        errno = ETIME;
        return NULL;
    }

    fp = fdopen(s[0], "r");

    return fp;
}

int
popen_close(FILE *ipf)
{
    if (ipf != fp) {
        errno = EINVAL;
        return -1;
    }

    if (debug)
        fprintf(stderr, "%s: bye pid %d\n", __func__, pid);

    fclose(fp);
    int wstatus;
    if (waitpid(pid, &wstatus, 0) < 0) {
        return -1;
    }

    return wstatus;
}
