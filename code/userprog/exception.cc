// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#ifdef CHANGED
#include "userthread.h"
#include "userproc.h"
#endif // CHANGED
static const unsigned Buf_size = 32;

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC()
{
  int pc = machine->ReadRegister(PCReg);
  machine->WriteRegister(PrevPCReg, pc);
  pc = machine->ReadRegister(NextPCReg);
  machine->WriteRegister(PCReg, pc);
  pc += 4;
  machine->WriteRegister(NextPCReg, pc);
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
  int type = machine->ReadRegister(2);
  int address = machine->ReadRegister(BadVAddrReg);

  switch (which)
  {
  case SyscallException:
  {
    switch (type)
    {
    case SC_Halt:
    {
      DEBUG('s', "Shutdown, initiated by user program.\n");
      interrupt->Powerdown();
      break;
    }
#ifdef CHANGED
    case SC_Exit:
    {
      DEBUG('s', "Exit\n");
      int status = machine->ReadRegister(4);
      DEBUG('s', "Process %s shutdown with status %d\n", currentThread->getName(), status);
      processCountMutex->P();
      processCount--;
      bool lastProcess = (processCount == 0);
      processCountMutex->V();
      if (lastProcess) {
        DEBUG('s', "Last process exiting. Shutdown machine.\n");
        interrupt->Powerdown();
      } else {
        DEBUG('s', "Process exiting but others are still running.\n");
        currentThread->Finish(); 
      }
      break;
    }
    case SC_PutChar:
    {
      DEBUG('s', "PutChar\n");
      consoledriver->PutChar(machine->ReadRegister(4));
      break;
    }
    case SC_GetChar:
    {
      DEBUG('s', "GetChar\n");
      int c = consoledriver->GetChar();
      if (c == EOF)
      {
        machine->WriteRegister(2, -1);
      }
      else
      {
        machine->WriteRegister(2, c);
      }
      break;
    }
    case SC_PutString:
    {
      DEBUG('s', "SC_PutString\n");
      char *s = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
      if (s == NULL)
      {
        DEBUG('s', "MALLOC ERROR\n"); // if it happens good luck
        break;
      }
      unsigned length = 0, buf;
      DEBUG('s', "Start LOOP\n");
      do
      {
        buf = copyStringFromMachine(machine->ReadRegister(4) + length, s, MAX_STRING_LENGTH);
        length += buf; // collect the length the string if needed later and help to navigate the data in register 4
        consoledriver->PutString(s);
      } while (buf == MAX_STRING_LENGTH);
      DEBUG('s', "LOOP OK\n");
      DEBUG('s', "END OF SC_PutString\n");
      free(s); // Don't forget to free s !
      break;
    }
    case SC_GetString:
    {
      int n = machine->ReadRegister(5);
      if (n <= 0)
        break;
      char *buf = new char[n];
      consoledriver->GetString(buf, n);
      copyStringToMachine(machine->ReadRegister(4), buf, (unsigned)n);
      delete[] buf;
      break;
    }
    case SC_PutInt:
    {
      char buf[Buf_size];
      int x = snprintf(buf, Buf_size, "%d", machine->ReadRegister(4));
      if (x < 0)
        buf[0] = '\0';
      consoledriver->PutString(buf);
      break;
    }
    case SC_GetInt:
    {
      char buf[Buf_size];
      consoledriver->GetString(buf, Buf_size);
      int x = 0;
      int n = sscanf(buf, "%d", &x);
      if (n != 1)
        x = 0;
      machine->WriteMem(machine->ReadRegister(4), 4, x);
      break;
    }
    case SC_ThreadCreate:
    {
      machine->WriteRegister(2, do_ThreadCreate(machine->ReadRegister(4), machine->ReadRegister(5)));
      // UpdatePC();
      break;
    }
    case SC_ThreadExit:
    {
      do_ThreadExit();
      break;
    }
    case SC_ForkExec: 
    {
      DEBUG('s', "ForkExec\n");
      int filenameAddr = machine->ReadRegister(4);
      int res = do_ForkExec(filenameAddr);
      machine->WriteRegister(2, res);
      break;
    }
#endif // CHANGED

    default:
    {
      ASSERT_MSG(FALSE, "Unimplemented system call %d\n", type);
    }
    }

    // Do not forget to increment the pc before returning!
    // This skips over the syscall instruction, to continue execution
    // with the rest of the program
    UpdatePC();
    break;
  }

  case PageFaultException:
    if (!address)
    {
      ASSERT_MSG(FALSE, "NULL dereference at PC %x!\n", machine->registers[PCReg]);
    }
    else
    {
      // For now
      ASSERT_MSG(FALSE, "Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
    }
    break;

  case ReadOnlyException:
    // For now
    ASSERT_MSG(FALSE, "Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
    break;

  case BusErrorException:
    // For now
    ASSERT_MSG(FALSE, "Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
    break;

  case AddressErrorException:
    // For now
    ASSERT_MSG(FALSE, "Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
    break;

  case OverflowException:
    // For now
    ASSERT_MSG(FALSE, "Overflow at PC %x\n", machine->registers[PCReg]);
    break;

  case IllegalInstrException:
    // For now
    ASSERT_MSG(FALSE, "Illegal instruction at PC %x\n", machine->registers[PCReg]);
    break;

  default:
    ASSERT_MSG(FALSE, "Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
    break;
  }
}
