

#include <avr/io.h>
#include "timebase.h"
#include "uart.h"
#include "pid.h"
#include "app.h"




void App_Setup(void){

  UART_Init(57600);
  
  
  Timebase_DownCounter_SS_Set_Securely(0, 50);
  
}


void App_Main_Loop(void){
  
  
  if(UART_Data_Available()){
    UART_Tx_Parameter_Hex_SP("CalcCRC", UART_Data_Calculated_CRC_Get() );
    UART_Tx_Parameter_Hex_SP("ErrorCode", UART_Error_Code_Get() );
    UART_Data_Print_Buf();
	UART_Buf_Flush();
	UART_Data_Clear_Available_Flag();
	UART_Data_Clear_Read_Complete_Flag();
  }
  
  /*if(Timebase_DownCounter_SS_Continuous_Expired_Event(0)){
    UART_Tx_Parameter_NL("Dcnt0", Timebase_DownCounter_SS_Get_Value(0));
  }*/
  
}

