#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg)
{
    (void)arg;
    readAvail->V();
}

static void WriteDoneHandler(void *arg)
{
    (void)arg;
    writeDone->V();
}

ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}

ConsoleDriver::~ConsoleDriver()
{
    delete console;
    delete writeDone;
    delete readAvail;
}

void ConsoleDriver::PutChar(int ch)
{
    console->TX(ch);
    writeDone->P(); // wait for write to finish
}

int ConsoleDriver::GetChar()
{
    readAvail->P();       // wait for character to arrive
    return console->RX(); // return our char as int
}

void ConsoleDriver::PutString(const char *s)
{
    while (*(s) != '\0')
    {
        console->TX(*(s++));
        writeDone->P();
    }
}

void ConsoleDriver::GetString(char *s, int n)
{
    // ...
}

#endif // CHANGED