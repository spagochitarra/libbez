
/* system() with timeout
 */

#include "librun.h"

static char debug;

int
system_timeout(const char *cmd, int timeout)
{
    FILE *fp;
    int s[2];
    int cc;
    char *p;
    pid_t pid;
    char buf[BUFSIZ/8];

    if (cmd == NULL
        || timeout < 0) {
        errno = EINVAL;
        return -1;
    }

    debug = 0;
    if ((p = getenv("RUN_DEBUG")))
        debug = 1;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, s) < 0) {
        perror("socketpair()");
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(s[0]);
        close(s[1]);
        return -1;
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
        return -1;
    }

    if (cc == 0) {
        if (debug)
            fprintf(stderr, "%s: select() timed out kill %d\n", __func__, pid);
        kill(pid, SIGTERM);
        close(s[0]);
        errno = ETIME;
        return -1;
    }

    fp = fdopen(s[0], "r");

    while ((fgets(buf, sizeof(s), fp)))
        printf("%s", buf);

    fclose(fp);

    int wstatus;
    if (waitpid(pid, &wstatus, 0) < 0) {
        perror("wait");
        return -1;
    }

    return wstatus;
}

/* wrap the above
 */
int
system2(const char *cmd)
{
    char *p;
    int t;

    if (cmd == NULL) {
        errno = EINVAL;
        return -1;
    }

    t = _SYSTEM_TIMEOUT;
    if ((p = getenv("_SYSTEM_TIMEOUT"))) {
        t = atoi(p);
        if (t <= 0)
            t = _SYSTEM_TIMEOUT;
    }

    return system_timeout(cmd, t);
}
