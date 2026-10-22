
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>

int main(void)
{
    char buffer[128] = {0}; // [esp+20h] [ebp-98h]
    int status = 0;
    long syscall_number = 0;
    pid_t pid = fork();

    if (pid != 0)
    {
        // Parent process
        do
        {
            wait(&status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                puts("child is exiting...");
                return 0;
            }

            syscall_number = ptrace(PTRACE_PEEKUSER, pid, 44, 0);

        } while (syscall_number != 11);

        puts("no exec() for you");
        kill(pid, SIGKILL);
    }
    else
    {
        // Child process
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        ptrace(PTRACE_TRACEME, 0, 0, 0);

        puts("Give me some shellcode, k");
        gets(buffer); // Buffer overflow
    }

    return 0;
}
