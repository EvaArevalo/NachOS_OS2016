/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"

#include "synchconsole.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

void SysPrintInt(int n)
{
    const char zero = '0';
    if(n == 0){
        kernel->synchConsoleOut->PutChar(zero);
    }
    int digits = 0;
    char number[20];
    while(n>0){
        *(number+digits) = zero+(n%10);
        n /= 10;
        digits++;
    }
    while(digits){
        digits--;
        kernel->synchConsoleOut->PutChar(*(number+digits));
    }
}

int SysCreate(char *filename)
{
	// return value
	// 1: success
	// 0: failed
	return kernel->interrupt->CreateFile(filename);
}


#endif /* ! __USERPROG_KSYSCALL_H__ */
