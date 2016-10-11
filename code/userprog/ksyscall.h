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

#define MAX_BUF_SIZE 256

#include "kernel.h"

#include "synchconsole.h"

//EVA
#include "openfile.h"

//EVA

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

//EVA
void CopyStrFromMachine(int addr, char *string, int size)
{
	int val;
	char c;
	int i = 0;
	
	do{
		//readmem returns false in case reading failed,
		//handle failed reading pending...
		kernel->machine->ReadMem(addr + i,1,&val);
		c = (char) val;
		string[i] = c;
		i++;
	} while (i < size - 1 && c!= '\0');
	
	while i<size) {
		string[i] = '\0';
		i++;
	}
	
	return;
}

void CopyStrToMachine(int addr, char *string, int size){
	
	int i=0;
	
	//copy each charachter to virtual memory, stop if end of string or newline
	while(i<size && string[i] != '\0' && string[i]!='\n'){
		//writemem returns false in case writing failed,
		//handle failed writing pending...
		kernel->machine->WriteMem(addr+i,1,(int)string[i]);
		i++;
	}
	
	//end string with '\0'
	kernel->machine->WriteMem(addr +i,1,(int)'\0');
	
	return;
}

OpenFileId SysOpenFileId(char *filename){
	
	int val = fileSystem->Open(filename);
	char name[MAX_BUF_SIZE + 1];
	
	CopyStringFromMachine(machine->ReadRegister(4), name, MAX_BUF_SIZE+1);
	//add to table of open files
	fileSystem->fileDescriptorTable[fileIndex] = file_id;
	kernel->currentThread->AddFile(val);
	kernel->machine->WriteRegister(2,(int)file_id);
	return file_id;
}

void SysClose()
{
	int file_id = kernel->machine->ReadRegister(4);
	
	//file not-existent error
	if (fileSystem->fileDescriptorTable[file_id] == NULL){
		DEBUG("File is not opened\n");
		return;
	}
	
	//close file
	delete fileSystem->fileDescriptorTable[file_id];
	fileSystem->fileDescriptorTable[file_id]==NULL;
	
	kernel->currentThread->RemoveFile(val);
	return;
}

int SysRead(){
	int file_id = kernel->machine->ReadRegister(4);
	char buffer[MAX_BUF_SIZE];
	int addr = kernel->machine->ReadRegister(5);
	int n = kernel->machine->ReadRegister(6);
	int val;
	
	if(kernel->currentThread->FileIsOpened(file_id)==TRUE){
		val = fileSystem->Read(file_id,buffer,n);
		CopyStringToMachine(addr,buffer,n);
	}
	//if failure to open file
	else{
		val = -1;
	}
	kernel->machine->WriteRegister(2,val);
	return;
}

void SysWrite(){
		
	int file_id = kernel->machine->ReadRegister(4);
	char buffer[MAX_BUF_SIZE];
	int addr = kernel->machine->ReadRegister(5);
	int n = kernel->machine->ReadRegister(6);
	int val;
	
	if(kernel->currentThread->FileIsOpened(file_id) == TRUE){
		CopyStrFromMachine(addr,buffer,n);
		val = fileSystem->Write(file_id,buffer,n);
	}
	else{
		val = -1;
	}
	kernel->machine->WriteRegister(2,val);
	return;
}
//EVA

#endif /* ! __USERPROG_KSYSCALL_H__ */
