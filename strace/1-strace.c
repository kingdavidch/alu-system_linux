#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscalls.h"
/*#include <sys/syscall.h>*/


/**
 * main - traces a process and prints system call numbers as they're called
 * @argc: argument count
 * @argv: argument array
 * @envp: environment parameters
 * Return: 0 on success | 1 on failure (not enough arguments)
 * author: Frank Onyema Orji
 **/
int main(int argc, char *argv[], char *envp[])
{
	int i, status;
	struct user_regs_struct regs;
	pid_t pid;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <full_path> [path_args]\n", argv[0]);
		return (1);
	}
	setbuf(stdout, NULL);
	pid = fork();

	if (pid == 0)
	{
		printf("execve");
		ptrace(PTRACE_TRACEME, pid, NULL, NULL);
		execve(argv[1], argv + 1, envp);
	}
	else
	{
		for (status = 1, i = 1; !WIFEXITED(status); i ^= 1)
		{
			ptrace(PT_SYSCALL, pid, NULL, NULL);
			wait(&status);
			ptrace(PT_GETREGS, pid, NULL, &regs);
			if (i)
				printf("\n");
			else
				printf("%s", syscalls_64_g[regs.orig_rax].name);
		}
	}

	return (0);
}