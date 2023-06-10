#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int err_fd2 (char *str)
{
    while (*str)
        write(2, str++, 1);
    return 1;
}

int ft_cd (char **argv, int i)
{
    if (i != 2)
        return err_fd2("error: cd: bad arguments\n");
    else if (chdir(argv[1]) == -1)
        return err_fd2("error: cd: cannot change directory to "), err_fd2(argv[1]), err_fd2("\n");
    return 0;
}

int ft_exec (char **argv, char **envp, int i)
{
    int fd[2];
    int pid_status;
    int has_pipe;

    has_pipe = argv[i] && !strcmp (argv[i], "|");

    if (has_pipe && pipe(fd) == -1)
        return err_fd2("error: fatal\n");

    int pid = fork();
    if (!pid)
    {
        argv[i] = 0;
        if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close (fd[1]) == -1))
            return err_fd2("error: fatal\n");
        execve(*argv, argv, envp);
        return err_fd2("error: cannot execute "), err_fd2(*argv), err_fd2("\n");
    }

    waitpid (pid, &pid_status, 0);
    if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close (fd[1]) == -1))
        return err_fd2("error: fatal\n");
    return WIFEXITED (pid_status) && WEXITSTATUS (pid_status);
}

int main (int argc, char **argv, char **envp)
{
    int i = 0;
    int status = 0;

    if (argc > 1)
    {
        while (argv[i] && argv[++i])
        {
            argv += i;
            i = 0;
            while (argv[i] && strcmp(argv[i], "|") && strcmp (argv[i], ";"))
                i++;
            if (!strcmp(*argv, "cd"))
                status = ft_cd (argv, i);
            else if (i)
                status = ft_exec(argv, envp, i);
        }
    }
    return status;
}
