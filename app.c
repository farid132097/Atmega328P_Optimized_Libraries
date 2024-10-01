

#include <avr/io.h>
#include "timebase.h"
#include "uart.h"
#include "app.h"


void App_Setup(void){

  UART_Init(57600);
  
  DDRD  |=  (1<<0);
  PORTD &=~ (1<<0);
  
  Timebase_DownCounter_SS_Set_Securely(0, 50);
  Timebase_LPDownCounter_Set_Securely(0, 5);
  
}


void App_Main_Loop(void){

  if(Timebase_DownCounter_SS_Continuous_Expired_Event(0)){
    UART_Transmit_Number(Timebase_LPTimer_Get_Seconds());
	UART_Transmit_Text("\r\n");
  }
  
}

