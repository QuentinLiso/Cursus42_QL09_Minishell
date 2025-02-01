#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void execute_command(char *cmd, char **args, char **env) {
    pid_t pid = fork();
    
    if (pid == 0) {  // Child process
        execve(cmd, args, env);
        perror("execve failed"); // If execve fails, print an error
        exit(127);  // Exit with error status
    }
    
    int status;
    waitpid(pid, &status, 0);  // Parent waits for child

    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 127) {
            printf("Error: Command execution failed (execve failed)\n");
        }
    }
}

int main(int argc, char *argv[], char *envp[]) {
    char *cmd = "/bin/ls";  // Try changing to an invalid command like "/bin/doesnotexist"
    char *args[] = { "ls", "-l", NULL };

    execute_command(cmd, args, envp);
	(void)argc; (void)argv;

    return 0;
}
