

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"

int main(void){
  
  Timebase_Init(1000);
  UART_Init(9600);
  
  while(1){
    
  }
}