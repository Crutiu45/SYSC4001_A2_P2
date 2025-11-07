#include <stdio.h>
#include <unistd.h>

int main() {
    int counter = 0;
    int cycle = 0;
    
    while (1) {
        cycle++;
        if (counter % 3 == 0) {
            printf("Process 1 [PID: %d] - Cycle: %d - %d is a multiple of 3\n", 
                   getpid(), cycle, counter);
        } else {
            printf("Process 1 [PID: %d] - Cycle: %d - Counter: %d\n", 
                   getpid(), cycle, counter);
        }
        fflush(stdout);
        counter++;
        usleep(500000);
    }
    
    return 0;
}
