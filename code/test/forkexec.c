#include "syscall.h"
main()
{
    PutString("Lancement sous_proc 1\n");
    ForkExec("../test/putchar");

    PutString("Lancement sous_proc 2\n");
    ForkExec("../test/putchar");

    PutString("Main_proc terminé\n");
    Exit(0);

    return 0;
}