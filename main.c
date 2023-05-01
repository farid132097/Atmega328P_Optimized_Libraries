

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"
#include "lpf.h"

int main(void){
  
  Timebase_Init(1000);
  UART_Init(9600);
  LPF_Init();
  
  while(1){
    
  }
}