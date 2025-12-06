#include "syscall.h"
int main() {
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages0");
    Exit(0);
    return 0;
}