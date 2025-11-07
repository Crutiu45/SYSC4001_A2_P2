#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    // Create shared memory segment
    int shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }
    
    // Attach shared memory
    int *shared = (int *)shmat(shmid, NULL, 0);
    if (shared == (int *)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    // Initialize shared variables
    shared[0] = 3;   // multiple value
    shared[1] = 0;   // counter
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if (pid == 0) {
        // Child process
        // Wait until counter > 100
        while (shared[1] <= 100) {
            usleep(10000);
        }
        
        printf("Child [PID: %d] started. Shared counter: %d\n", getpid(), shared[1]);
        
        while (shared[1] <= 500) {
            if (shared[1] % shared[0] == 0) {
                printf("Child [PID: %d]: %d is a multiple of %d\n", 
                       getpid(), shared[1], shared[0]);
            }
            usleep(50000);
        }
        
        printf("Child [PID: %d] finished (counter > 500)\n", getpid());
        shmdt(shared);
        exit(0);
    } else {
        // Parent process
        printf("Parent [PID: %d] started. Incrementing counter...\n", getpid());
        
        while (shared[1] <= 500) {
            shared[1]++;
            if (shared[1] % 50 == 0) {
                printf("Parent [PID: %d]: Counter = %d\n", getpid(), shared[1]);
            }
            usleep(10000);
        }
        
        printf("Parent [PID: %d] finished (counter > 500)\n", getpid());
        wait(NULL);
        
        // Cleanup
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
    }
    
    return 0;
}
