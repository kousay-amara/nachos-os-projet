#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "system.h"

int do_ThreadCreate(int f, int arg)
{
    int farg[2] = {f, arg};
    Thread *newthread = new Thread("newThread");
    newthread->Start(StartUserThread, farg);
    free(farg);
}
static void StarUserThread(void *farg)
{
    // TODO
}

void do_ThreadExit()
{
    currentThread->Finish();
}


#endif // CHANGED