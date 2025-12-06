#include "syscall.h"

#define NUM_PROCESS 12

int main() {
    int i;
    PutString("Starting test (12 proc x 12 threads ... yes 144 threads)\n");
    
    for (i = 0; i < NUM_PROCESS; i++) {
        ForkExec("../test/dozen_threads");
        PutString("Launcher: Processus start\n");
    }
    
    PutString("Launcher End, Nachos still working...\n");
    Exit(0);
}