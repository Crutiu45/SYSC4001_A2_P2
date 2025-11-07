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
    
    int counter = 0;
    
    if (pid == 0) {
        // Child process
        while (1) {
            printf("Child [PID: %d] - Cycle: %d, Counter: %d\n", 
                   getpid(), counter, counter);
            fflush(stdout);
            counter++;
            usleep(500000);  // 0.5 second delay
        }
    } else {
        // Parent process
        while (1) {
            printf("Parent [PID: %d] - Cycle: %d, Counter: %d\n", 
                   getpid(), counter, counter);
            fflush(stdout);
            counter++;
            usleep(500000);  // 0.5 second delay
        }
    }
    
    return 0;
}
