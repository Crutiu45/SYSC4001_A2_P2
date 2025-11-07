#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

// Semaphore operations
void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    // Create shared memory
    int shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }
    
    int *shared = (int *)shmat(shmid, NULL, 0);
    if (shared == (int *)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    // Create semaphore
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget failed");
        exit(1);
    }
    
    // Initialize semaphore to 1
    semctl(semid, 0, SETVAL, 1);
    
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
        int local_counter;
        
        // Wait until counter > 100
        do {
            sem_wait(semid);
            local_counter = shared[1];
            sem_signal(semid);
            usleep(10000);
        } while (local_counter <= 100);
        
        printf("Child [PID: %d] started. Shared counter: %d\n", getpid(), local_counter);
        
        while (1) {
            sem_wait(semid);
            local_counter = shared[1];
            int multiple = shared[0];
            sem_signal(semid);
            
            if (local_counter > 500) break;
            
            if (local_counter % multiple == 0) {
                printf("Child [PID: %d]: %d is a multiple of %d\n", 
                       getpid(), local_counter, multiple);
            }
            usleep(50000);
        }
        
        printf("Child [PID: %d] finished (counter > 500)\n", getpid());
        shmdt(shared);
        exit(0);
    } else {
        // Parent process
        printf("Parent [PID: %d] started. Incrementing counter...\n", getpid());
        
        while (1) {
            sem_wait(semid);
            shared[1]++;
            int current = shared[1];
            sem_signal(semid);
            
            if (current > 500) break;
            
            if (current % 50 == 0) {
                printf("Parent [PID: %d]: Counter = %d\n", getpid(), current);
            }
            usleep(10000);
        }
        
        printf("Parent [PID: %d] finished (counter > 500)\n", getpid());
        wait(NULL);
        
        // Cleanup
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, 0);
    }
    
    return 0;
}
