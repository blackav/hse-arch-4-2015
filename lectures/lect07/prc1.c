#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int startval = 10;
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(1);
    }
    if (pid > 0) {
        // parent
        for (int i = startval; i < 100; i += 2) {
            printf("%d\n", i); fflush(stdout);
            sched_yield();
        }
        int stat = 0;
        wait(&stat);
        if (WIFEXITED(stat)) {
            printf("Son finished with status %d\n", WEXITSTATUS(stat));
        } else if (WIFSIGNALED(stat)) {
            printf("Son terminated with signal %d\n", WTERMSIG(stat));
        }
    } else {
        // son
        nice(5);
        int i;
        setbuf(stdout, NULL);
        for (i = startval + 1; i < 100; i += 2) {
            printf("%d\n", i);
            sched_yield();
        }
        return(25);
    }
    return 0;
}
