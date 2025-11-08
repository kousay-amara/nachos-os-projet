#include "syscall.h"

void thread_func(void *arg) {
    int thread_id = (int)arg;
    volatile int i;
    
    PutString("Thread ");
    PutInt(thread_id);
    PutString(": started\n");
    
    char message[6];
    for(i = 0; i < 5; i++) {
        message[i] = 'A' + thread_id;
    }
    message[5] = '\n';
    message[6] = '\0';
    
    PutString(message);
    PutString("Thread ");
    PutInt(thread_id); 
    PutString(": finished\n");
    
    ThreadExit();
}

int main() {
    int i;
    
    PutString("Testing multiple threads creation:\n");
    
    for(i = 0; i < 3; i++) {
        if (ThreadCreate(thread_func, (void*)i) < 0) {
            PutString("Failed to create thread ");
            PutInt(i);
            PutChar('\n');
        }
    }
    
    PutString("All threads created - main calling ThreadExit\n");
    ThreadExit();
    
    return 0;
}