

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"

int main(void){
  
  
  
  UART_Init(57600);
  Timebase_Init(1000);
  
  //For test, also added inside ISR
  DDRD  |=  (1<<5)|(1<<6);
  PORTD &=~ ((1<<5)|(1<<6));
  _delay_ms(500);
  Timebase_UpCounter_SS_Set_Securely(0, 20000);
  Timebase_UpCounter_Set_Securely(0, 18);
  Timebase_DownCounter_SS_Set_Securely(0, 16000);
  Timebase_DownCounter_Set_Securely(0, 14);
  
  while(1){
    
	Timebase_Timer_Await_SubSeconds(50);
	
	UART_Transmit_Number(Timebase_UpCounter_SS_Get_Value(0));
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_UpCounter_Get_Value(0));
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_DownCounter_SS_Get_Value(0));
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_DownCounter_Get_Value(0));
	UART_Transmit_Text("\r\n");
    /*UART_Transmit_Text("Timebase ");
	UART_Transmit_Number(Timebase_Timer_Get_Seconds());
	UART_Transmit_Text(" ");
	UART_Transmit_Number(Timebase_Timer_Get_SubSeconds());
	UART_Transmit_New_Line();*/
	
	/*UART_Transmit_Number(Timebase_DownCounter_SS_Get_EndValueSec(0));
	UART_Transmit_Space();
	UART_Transmit_Number(Timebase_DownCounter_SS_Get_EndValueSubSec(0));
	UART_Transmit_Space();*/
	//UART_Transmit_Number(Timebase_DownCounter_SS_Get_Value(0));
	//UART_Transmit_New_Line();
	//Timebase_Timer_Delay_SubSeconds(100);
	
	//Add update req flag
	PORTD |= (1<<6);
	Timebase_Main_Loop_Executables();
	PORTD &=~(1<<6);
	
  }
}