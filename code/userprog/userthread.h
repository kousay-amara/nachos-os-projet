#ifdef CHANGED
#pragma once
/* or #ifndef USERTHREAD_H
      #endif // USERTHREAD_H */

extern int do_ThreadCreate(int f, int arg);
static void StartUserThread(void *schmurtz);
void do_ThreadExit();

#endif // CHANGED