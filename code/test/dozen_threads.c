#include "syscall.h"

// many_threads.c simplified

#define NUM_THREADS 12

void worker(void *arg) {
    int id = (int)arg;
    PutString("Thread "); PutInt(id); PutString(" running\n");
    ThreadExit();
}

int main() {
    int i;
    for(i = 1; i <= NUM_THREADS; i++) {
        if (ThreadCreate(worker, (void*)i) == -1) {
            PutString("Failed to create thread "); PutInt(i); PutString("\n");
        }
    }
    ThreadExit();
    return 0;
}