

#include <avr/io.h>
#include "thdbg.h"

#define  MAX_FNPTR 20

typedef struct thdbg_t{
  void    (*FnPtr[MAX_FNPTR])(void);
  uint8_t TotalThreads;
  uint8_t ExecutionIndex;
}thdbg_t;


thdbg_t ThreadedDbg;

void ThreadedDbg_Struct_Init(void){
  ThreadedDbg.TotalThreads = 0;
  ThreadedDbg.ExecutionIndex = 0;
}

void ThreadedDbg_Add_Function( void (*fptr) (void) ){
  ThreadedDbg.FnPtr[ThreadedDbg.TotalThreads] = fptr;
  ThreadedDbg.TotalThreads++;
}

void ThreadedDbg_Execute_Function(void){
  (*ThreadedDbg.FnPtr[ThreadedDbg.ExecutionIndex])();
  ThreadedDbg.ExecutionIndex++;
  if(ThreadedDbg.ExecutionIndex > ThreadedDbg.TotalThreads){
    ThreadedDbg.ExecutionIndex = 0;
	//Some processors need delay
  }
}






void ThreadedDbg_Init(void){
  ThreadedDbg_Struct_Init();
}



