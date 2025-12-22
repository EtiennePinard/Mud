#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define NC "\033[0m"
#define TICK "✓"
#define CROSS "✗"

#define BUFFER_SIZE (256)
static char progName[BUFFER_SIZE];

#define OUTPUT_BUFFER_SIZE 4096

static void extractNameFromPath(const char* path, char* name, size_t nameBufferSize) {
    size_t lastPathSeparatorIndex = strlen(path);
    while (lastPathSeparatorIndex-- > 0 && path[lastPathSeparatorIndex] != '/');
    strncpy(name, path + lastPathSeparatorIndex + 1, nameBufferSize);
}

static void runTestWithOutput(const char* prog_path, FILE** output) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        close(pipefd[0]);  // Close read end

        // Redirect stdout and stderr to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        execl(prog_path, prog_path, (char*)NULL);

        fprintf(stderr, "Failed to execute %s: %s\n", prog_path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else {
        // Parent process
        close(pipefd[1]);  // Close write end

        // Create a FILE* for reading
        *output = fdopen(pipefd[0], "r");
        if (*output == NULL) {
            perror("fdopen");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        }
    }
}

/*
Little note on output:
If the stdout of the tests is buffered and a signal
like a segfault happens, the output of stdout that was not 
flushed will not be captured by the pipe and propagated
to the parent process. This is quite unfortunate, but
this is something we have to live with.
*/
static void printOutput(FILE* output) {
    char buffer[OUTPUT_BUFFER_SIZE];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), output);

    printf("--- Output ---\n");

    while (bytesRead > 0) {
        fwrite(buffer, sizeof(char), bytesRead, stdout);
        bytesRead = fread(buffer, sizeof(char), sizeof(buffer), output);
    }
    
    printf("--------------\n");
    fclose(output);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "TEST RUNNER ERROR: Not test programs provided\n");
        fprintf(stderr, "Usage: %s <test_program1> [test_program2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Running tests...\n");
    printf("=================\n");

    int total = argc - 1;
    int failed = 0;

    for (int argIndex = 1; argIndex < argc; argIndex++) {
        const char* progPath = argv[argIndex];
        
        extractNameFromPath(progPath, progName, sizeof(progName));
        printf("%-40s", progName);
        fflush(stdout);
        
        FILE* testOutput = NULL;
        runTestWithOutput(progPath, &testOutput);
        
        // Wait for test to finish
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);

            if (exit_code == EXIT_SUCCESS) {
                printf(GREEN TICK " PASS" NC "\n");
                fclose(testOutput);
            }
            else {
                failed++;
                printf(RED CROSS " FAILED" NC "\n");
                printOutput(testOutput);
            }
        }
        else if (WIFSIGNALED(status)) {
            failed++;
            printf(RED CROSS " FAILED (Signal %d)" NC "\n", WTERMSIG(status));
            printOutput(testOutput);
        }
        else {
            failed++;
            printf(RED CROSS " FAILED (Unknown)" NC "\n");
            printOutput(testOutput);
        }
    }

    printf("=================\n");

    if (failed == 0) printf(GREEN "ALL %u tests passed" NC "\n", total);
    else printf(RED "%u/%u tests failed" NC "\n", failed, total);
    return failed;
}
