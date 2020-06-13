
#include "librun.h"

static char cmd[BUFSIZ/8];
static char s[128];

int
main(int argc, char **argv)
{
    FILE *fp;
    int i;
    int cc;

    if (argc == 1) {
        fprintf(stderr, "runme wants a command\n");
        return -1;
    }

    for (i = 1; i < argc; i++)
        sprintf(cmd + strlen(cmd), "%s ", argv[i]);

    if ((cc = system_timeout(cmd, 10)) < 0) {
        perror("system_timeout");
        return -1;
    }

    printf("Now run system2()\n");

    if ((cc = system2(cmd)) < 0) {
        perror("system2()");
        return cc;
    }

    return cc;
}
