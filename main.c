

#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

int main(void){
  
  Debug_Init();
  
  while(1){
    
	Debug_Tx_Parameter_NL("ADC_VAL",125);
	_delay_ms(100);
	
  }
}