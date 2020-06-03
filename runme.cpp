
#include "librun.h"

static char cmd[BUFSIZ/8];
static char s[128];

int
main(int argc, char **argv)
{
    FILE *fp;
    int i;

    if (argc == 1) {
        fprintf(stderr, "runme wants a command\n");
        return -1;
    }

    for (i = 1; i < argc; i++)
        sprintf(cmd + strlen(cmd), "%s ", argv[i]);

    fp = popen_read(cmd, 10);
    if (! fp) {
        perror("popen_read()");
        return -1;
    }

    while ((fgets(s, sizeof(s), fp)))
        printf("%s", s);

    popen_close(fp);

    return 0;
}
