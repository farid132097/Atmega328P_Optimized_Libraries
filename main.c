

#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

int main(void){
  
  Debug_Init();
  
  while(1){
    
	Debug_Tx_Byte(0xAA);
	_delay_ms(1000);
	
  }
}