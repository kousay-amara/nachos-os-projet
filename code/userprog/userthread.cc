#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "system.h"

static void StartUserThread(void *schmurtz);

int do_ThreadCreate(int f, int arg) {
    int stackAddr = currentThread->space->AllocateUserStack();
    if (stackAddr == -1) {
        DEBUG('t', "Thread creation failed: no stack space\n");
        return -1;
    }
    
    int *farg = new int[3];
    farg[0] = f;
    farg[1] = arg;
    farg[2] = stackAddr;
    
    Thread *newThread = new Thread("newThread");
    if (newThread == NULL) {
        currentThread->space->FreeUserStack(stackAddr);
        DEBUG('t', "Thread creation failed: kernel thread allocation\n");
        delete[] farg;
        return -1;
    }
    newThread->space = currentThread->space;

#ifdef CHANGED
    newThread->space->AddThread(newThread);
#endif // CHANGED

    currentThread->space->IncrementThreadCount();
    newThread->Start(StartUserThread, farg);
    return 0;
}

static void StartUserThread(void *_farg) {
    int *farg = (int *)_farg;
    int f = farg[0], arg = farg[1], stackAddr = farg[2];
    delete[] farg;

    for (int i = 0; i < NumTotalRegs; i++) {
        machine->WriteRegister(i, 0);
    }
    machine->WriteRegister(PCReg, f);
    machine->WriteRegister(4, arg);
    machine->WriteRegister(PrevPCReg, f - 4); // l'adresse de l'instruction précédente
    machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(StackReg, stackAddr - 16);
    
    DEBUG('t', "Starting user thread at function %x with stack %x\n", f, stackAddr - 16);
    
    machine->Run();
}

void do_ThreadExit() {
    DEBUG('t', "ThreadExit called from thread %s\n", currentThread->getName());
    AddrSpace* space = currentThread->space;
    if (space != NULL) {
        int stackReg = machine->ReadRegister(StackReg);
        space->FreeUserStack(stackReg);
        
        space->DecrementThreadCount();
        int count = space->GetThreadCount();
        if (count == 0) {
            DEBUG('t', "Last thread - calling Powerdown\n");
            interrupt->Powerdown();
            return;
        }
    }
    currentThread->space->RemoveThread(currentThread);
    currentThread->Finish();
}

#endif // CHANGED