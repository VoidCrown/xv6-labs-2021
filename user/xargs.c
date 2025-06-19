#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void main(int argc, char *argv[])
{
    int i;
    char buf[512], *args[MAXARG];
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [parameter...]");
    }
    for (i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    while(gets(buf, sizeof(buf)) > 0) {
        buf[strlen(buf) - 1] = '\0';

        args[argc - 1] = buf;
        args[argc] = 0;

        if (fork() == 0) {
            exec(args[0], args);
        } else {
            wait(0);
        }
    }

    

    exit(0);
}