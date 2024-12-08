

#include <avr/io.h>
#include "timebase.h"
#include "thdbg.h"
#include "uart.h"
#include "pid.h"
#include "app.h"

void Print0(void){
  UART_Tx_Parameter_Hex_NL("Print0",0x25);
}

void Print1(void){
  UART_Tx_Parameter_Hex_NL("Print1",0x0700);
}

void Print2(void){
  UART_Tx_Parameter_Hex_NL("Print2",0x96);
}

void Print3(void){
  UART_Tx_Parameter_Hex_NL("Print3",0x26);
}

void App_Setup(void){

  UART_Init(57600);
  ThreadedDbg_Init();
  ThreadedDbg_Add_Function(Print0);
  ThreadedDbg_Add_Function(Print1);
  ThreadedDbg_Add_Function(Print2);
  ThreadedDbg_Add_Function(Print3);
  Timebase_DownCounter_Set_Forcefully(0,5);
  
}


void App_Main_Loop(void){
  
  Timebase_Window_Timer_Start();
  if(Timebase_DownCounter_Continuous_Expired_Event(0)){
    ThreadedDbg_Execution_Enable();
  }
  
  
  ThreadedDbg_Execute_Function();
  uint16_t val = Timebase_Window_Timer_Get_Interval_Reset();
  
  if(UART_Data_Available()){
    UART_Tx_Parameter_Hex_CM("CalCRC", UART_Data_Calculated_CRC_Get());
    UART_Tx_Parameter_Hex_CM("RecCRC", UART_Data_Received_CRC_Get());
    UART_Data_Print_Buf();
	UART_Tx_NL();
	UART_RX_Packet_Read_Complete();
  }
  
  //UART_Tx_Parameter_NL("LoopTime", val);
  //UART_Tx_NL();
  
}

