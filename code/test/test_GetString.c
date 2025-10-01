#include "syscall.h"

int main() {
    char buf[64];
    int i = 0;
    GetString(buf, 64);
    while (buf[i] != '\0') {
        PutChar(buf[i++]);
    }
    Halt();
}
