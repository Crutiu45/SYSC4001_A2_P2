#include <stdio.h>
#include <unistd.h>



int main()
{

    int counter = 0;
    int cycle = 0;


    while (1)
    {

        cycle++;
        printf("Proccess 2 [PID: %d]  - Cycle: %d - Counter: %d\n", getpid() ,cycle, counter);
        fflush(stdout);
        counter--;
        usleep(500000);


    }


    return 0;


}

