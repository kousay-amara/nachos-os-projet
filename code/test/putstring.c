#include "syscall.h"

void print(char *c)
{
    // #if 0
    PutString(c);
    PutString("c\n");
    // #endif
}

int main()
{
#ifdef CHANGED
    char *test = "Hello Worlffffffffffffffffffffd\n";
    print(test);
    Halt();
#endif // CHANGED
}