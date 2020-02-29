#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    pid_t c1pid, c2pid, cpid;
    int wstatus, fds[2];
    if(pipe(fds) < 0)
    {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }
    c1pid = fork();
    if (c1pid < 0)
    {
        perror("fork()1");
        exit(EXIT_FAILURE);
    }

    if (c1pid == 0)
    {
        close(fds[0]);
        if (dup2(fds[1], STDOUT_FILENO) < 0)
        {
            perror("can't dup");
            exit(1);
        }
        close(fds[1]);
        execl(argv[1], argv[1], NULL);
        perror("execlp\n");
        exit(0);
    }
    else 
    {
        c2pid = fork();
        if (c2pid < 0)
        {
            perror("fork()2");
        }
        if (c2pid == 0)
        {
            close(fds[1]);
            if (dup2(fds[0], STDIN_FILENO) < 0)
            {
                perror("can't dup");
                exit(1);
            }
            close(fds[0]);
            execl(argv[2], argv[2], NULL);
            perror("execlp\n");
            exit(0);
        }
        else
        {
            close(fds[0]);
            close(fds[1]);
        }
    }

    while(1) {
        if ((cpid = wait(NULL)) == -1){
            if (errno == ECHILD) {
                exit(EXIT_SUCCESS);
            } else {
                perror("wait error");
                exit(EXIT_FAILURE);
            }
        }
    }
    return EXIT_SUCCESS;
}
