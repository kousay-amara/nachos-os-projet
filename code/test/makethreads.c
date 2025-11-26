#ifdef CHANGED
#include "syscall.h"

void aux(void *arg){
    PutChar('A');
    ThreadExit();
}

int main(){
    ThreadCreate(aux, 0);
    ThreadExit();
}
#endif // CHANGED
