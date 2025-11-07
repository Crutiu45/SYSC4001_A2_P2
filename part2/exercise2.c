#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if (pid == 0) {
        // Child process - execute mult3
        execl("./mult3", "mult3", NULL);
        perror("execl mult3 failed");
        exit(1);
    } else {
        // Parent process - execute decr
        execl("./decr", "decr", NULL);
        perror("execl decr failed");
        exit(1);
    }
    
    return 0;
}

