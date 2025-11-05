#include "syscall.h"

void background_thread(void *arg) {
    PutString("Background thread: working...\n");
    volatile int i;
    for (i = 0; i < 20000; i++);
    PutString("Background thread: finished work\n");
    ThreadExit();
}

int main() {
    PutString("=== Test Exit vs ThreadExit ===\n");
    ThreadCreate(background_thread, 0);
    PutString("Main: calling ThreadExit (background continues)\n");
    ThreadExit();
    PutString("Main: ERROR - continued after ThreadExit!\n");
    return 0;
}