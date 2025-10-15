#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "system.h"

int do_ThreadCreate(int f, int arg)
{
    int farg[2] = {f, arg};
    Thread *newthread = new Thread("newThread");
    newthread->Start(StartUserThread, farg);
}

static void StarUserThread(void *farg)
{
    // Collect the function and its arguments then free the pointer
    int f = farg[0], arg = farg[1];
    free(farg); // free the now useless pointer (here and not in do_ThreadCreate)

    DEBUG('s', "WriteRegister x4 START");
    machine->WriteRegister(PCReg, f); // Set the function in the current program counter
    machine->WriteRegister(4, arg);   // Set the arguments of the function in the right register (see in exception.cc)
    DEBUG('s', "Load f : %d in register %d/nLoad arg : %d in register 4", f, PCReg, arg);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);         // Don't forget to setup the next instruction for the branch delay possibility
    machine->WriteRegister(StackReg, currentThread->space->AllocateUserStack()); // Add the stack that will be used by the current thread
    DEBUG('s', "WriteRegister x4 DONE")

    machine->Run();
}

void do_ThreadExit()
{
    currentThread->Finish();
}

#endif // CHANGED