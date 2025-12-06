#ifdef CHANGED
#ifndef CONSOLEDRIVER_H
#define CONSOLEDRIVER_H
#include "copyright.h"
#include "utility.h"
#include "console.h"
#include "synch.h"

class ConsoleDriver : dontcopythis
{
public:
    // initialize the hardware console device
    ConsoleDriver(const char *readFile, const char *writeFile);
    ~ConsoleDriver(); // clean up

    void PutChar(int ch); // Behaves like putchar(3S)
    int GetChar();        // Behaves like getchar(3S)

    void PutString(const char *s);  // Behaves like fputs(3S)
    void GetString(char *s, int n); // Behaves like fgets(3S)

    void ReleaseLock(Thread *t); // R3_P2_6

private:
    Console *console;
    Semaphore *consoleMutex;
    Thread *lockOwner;
};

unsigned copyStringToMachine(int to, const char *from, unsigned size);
unsigned copyStringFromMachine(int from, char *to, unsigned size);

#endif // CONSOLEDRIVER_H
#endif // CHANGED