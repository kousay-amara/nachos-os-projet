#include "syscall.h"

#define NUM_THREADS 6

void worker(void *arg) {
    int id = (int)arg;
    volatile int i;
    PutString("Thread "); PutInt(id); PutString(" started\n");
    
    for(i = 0; i < 3; i++) {
        PutString("Thread "); PutInt(id); PutString(": loop "); PutInt(i); PutString("\n");
    }
    
    PutString("Thread "); PutInt(id); PutString(" finished\n");
    
    ThreadExit();
}

int main() {
    int i;
    
    PutString("=== Testing many threads with bitmap allocation ===\n");
    
    for(i = 1; i <= NUM_THREADS; i++) {
        if (ThreadCreate(worker, (void*)i) == -1) {
            PutString("Failed to create thread "); PutInt(i); PutString("\n");
            break;
        }
        PutString("Created thread "); PutInt(i); PutString("\n");
    }
    
    PutString("Main thread finished\n");
    ThreadExit();
    return 0;
}