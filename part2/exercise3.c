#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if (pid == 0) {
        // Child process - counts down to -500
        int counter = 0;
        int cycle = 0;
        
        while (counter > -500) {
            printf("Child [PID: %d] - Cycle: %d, Counter: %d\n", 
                   getpid(), cycle, counter);
            counter--;
            cycle++;
            usleep(5000);  // 0.005 second for faster execution
        }
        
        printf("Child [PID: %d] reached -500. Exiting...\n", getpid());
        exit(0);
    } else {
        // Parent process - waits for child
        printf("Parent [PID: %d] waiting for child [PID: %d]...\n", getpid(), pid);
        wait(NULL);
        printf("Parent [PID: %d]: Child has finished. Parent exiting.\n", getpid());
    }
    
    return 0;
}

