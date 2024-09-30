

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"

int main(void){
  
  
  
  UART_Init(57600);
  Timebase_Init(1000);
  
  //For test, also added inside ISR
  DDRD  |=  (1<<0);
  PORTD &=~ (1<<0);
  _delay_ms(500);
  
  /*
  Timebase_UpCounter_SS_Set_Securely(0, 20000);
  Timebase_UpCounter_Set_Securely(0, 18);
  Timebase_DownCounter_SS_Set_Securely(0, 160000);
  Timebase_DownCounter_Set_Securely(0, 14);
  Timebase_LPDownCounter_Set_Securely(0, 5);*/
  
  while(1){
    
	/*Timebase_Timer_Await_SubSeconds(10);
	
	UART_Transmit_Byte_Hex(Timebase_Get_Update_Req_Sts());
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_LPTimer_Get_Seconds());
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_LPDownCounter_Get_Value(0));
	UART_Transmit_Text(",");
	
	UART_Transmit_Number(Timebase_Timer_Get_Seconds());
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_Timer_Get_SubSeconds());
	UART_Transmit_Text(",");
	
	UART_Transmit_Number(Timebase_UpCounter_SS_Get_Value(0));
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_UpCounter_Get_Value(0));
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_DownCounter_SS_Get_Value(0));
	UART_Transmit_Text(",");
	UART_Transmit_Number(Timebase_DownCounter_Get_Value(0));
	UART_Transmit_Text("\r\n");
    
	if(Timebase_LPDownCounter_Expired_Event(0)){
	  Timebase_LPDownCounter_Set_Securely(0, 5);
	  UART_Transmit_Text("LPTIM Expired\r\n");
	}
	
	
	
	PORTD |= (1<<0);
	Timebase_Main_Loop_Executables();
	PORTD &=~(1<<0);*/
	
  }
}