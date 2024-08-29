

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"

int main(void){
  
  
  
  UART_Init(19200);
  Timebase_Init(1000);
  
  //For test, also added inside ISR
  DDRD  |=  (1<<5)|(1<<6);
  PORTD &=~ ((1<<5)|(1<<6));
  
  while(1){
    
    UART_Transmit_Text("Timebase ");
	UART_Transmit_Number(Timebase_Timer_Get_Seconds());
	UART_Transmit_Text(" ");
	UART_Transmit_Number(Timebase_Timer_Get_SubSeconds());
	UART_Transmit_New_Line();
	
	_delay_ms(100);
	
	//Add update req flag
	PORTD |= (1<<6);
	Timebase_Main_Loop_Executables();
	PORTD &=~(1<<6);
	
  }
}