#ifdef CHANGED

#include "userproc.h"
#include "thread.h"
#include "system.h"

int do_ForkExec(int filenameAddr) {
    char *filename = new char[MAX_STRING_SIZE];
    copyStringFromMachine(filenameAddr, filename, MAX_STRING_SIZE);

    OpenFile *executable = fileSystem->Open(filename);
    if (executable == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        delete [] filename;
        return -1;
    }

    AddrSpace *space = new AddrSpace(executable);

    Thread *t = new Thread("Forked Process");
    t->space = space;

    t->Start(StartUserProg, NULL);
    
    delete [] filename;
    return 0;
}

static void StartUserProg(void *arg) {
    (void) arg;
    currentThread->space->InitRegisters(); // set the initial register values
    currentThread->space->RestoreState();  // load page table register
    ASSERT_MSG(FALSE, "Machine->Run returned???\n"); // machine->Run never returns;
    ASSERT(FALSE);
}
#endif // CHANGED