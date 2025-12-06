#include "syscall.h"

#define NUM_ITERATIONS 5
#define CHARS_PER_ITERATION 10

void aggressive_writer(void *arg) {
    int thread_id = (int)(long)arg;
    char id_char = 'A' + thread_id;
    int i, j;
    volatile int k;

    for (i = 0; i < NUM_ITERATIONS; i++) {
        for (j = 0; j < CHARS_PER_ITERATION; j++) {
            PutChar(id_char);
        }
        PutChar('0' + i);
        for (k = 0; k < 1000; k++);
    }
    PutChar('\n');
}

int main() {
    volatile int i;
    PutString("=== TEST CONFLIT CONSOLE ===\n");
    ThreadCreate(aggressive_writer, (void*)0);  // Thread A
    ThreadCreate(aggressive_writer, (void*)1);  // Thread B  
    ThreadCreate(aggressive_writer, (void*)2);  // Thread C
    
    for (i = 0; i < 100000; i++);
    
    PutString("\n=== FIN TEST ===\n");
    
    return 0;
}