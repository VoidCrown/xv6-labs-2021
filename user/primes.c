#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_proc(int *p);

int
main(int argc, char *argv[])
{
    int p[2], i;

    pipe(p);
    if (fork() == 0) {
        new_proc(p);
    } else {
        // main process
        close(p[0]);
        for (i = 2; i <= 35; i++) {
           if (write(p[1], &i, sizeof(int)) != sizeof(int)) {
                fprintf(2, "first process failed to write %d into the pipe\n", i);
				exit(1);
           }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}

void new_proc(int *p) {
    // get a number from left neighbor
    int prime, n, p2[2];
    close(p[1]);
    if (read(p[0], &prime, sizeof(int)) == 0) {
        exit(0);
    }

    // print prime
    printf("prime %d\n", prime);

    pipe(p2);
    if (fork() == 0) {
        new_proc(p2);
    } else {
        close(p2[0]);
        while(read(p[0], &n, sizeof(int))) {
            // get a number from left neighbor
            // if not p does not divide n, send n to right neighbor
            if (n % prime) {
                write(p2[1], &n, sizeof(prime));
            }
        }
        close(p[0]);
        close(p2[1]);
        wait(0);
    }
    exit(0);
}