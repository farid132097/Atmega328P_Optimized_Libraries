/* 
 * File:   timebase.c
 * Author: MD. Faridul Islam
 * Atmega328P Timebase Library
 * Created on October 30, 2022, 19:00
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define  UART_ENABLE_TX    
#define  UART_ENABLE_RX    
#define  UART_ENABLE_RX_INT
#define  UART_BUFFER_SIZE       32

typedef struct uart_t{
    volatile uint8_t   Error;
    uint8_t            Digits[8];
    uint8_t            InputNumberDigits;
    volatile uint8_t   Buffer[UART1_BUF_SIZE];
    volatile uint16_t  BufferSize;
    volatile uint16_t  BufferIndex;
    volatile uint16_t  AvailableBytes;
    volatile uint16_t  ReadIndex;
    volatile uint32_t  LastRecivedTimeStamp;
}uart_t;


static uart_t UART;




//////////////////Device specific configurations start//////////////////

void UART_Config_GPIO(void){
	
}

void UART_Config_Clock(void){

}

void UART_Config_BAUD_Rate(uint32_t baud){

}

void UART_Config_Transmitter(void){

}

void UART_Config_Receiver(void){

}

void UART_Config_Receiver_Interrupt(void){

}

void UART_Transmit_Byte(uint8_t val){
  
}

uint8_t UART_Receive_Byte(void){
	uint8_t val=0;
  //need to add
  return val;
}


void UART_Struct_Init(void){
    UART.BufferSize=UART1_BUF_SIZE;
    UART.Error=0x00;
    UART.AvailableBytes=0x00;
    UART.ReadIndex=0x00;
    for(uint8_t i=0;i<UART.BufferSize;i++){
        UART.Buffer[i]=0;
    }
    for(uint8_t i=0;i<8;i++){
        UART.Digits[i]=0;
    }
    UART.InputNumberDigits=0;
    UART.BufferIndex=0;
    UART.LastRecivedTimeStamp=0;
}


void UART_Init(uint32_t baud){
	UART_Config_GPIO();
  UART_Config_Clock();
  UART_Config_BAUD_Rate(baud);
  #ifdef UART_ENABLE_TX
		UART_Config_Transmitter();
	#endif

  #ifdef UART_ENABLE_RX 
		UART_Config_Receiver();
	#endif
  
  #ifdef UART_ENABLE_RX_INT 
    UART_Config_Receiver_Interrupt();
  #endif    
}

void UART1_Transmit_Byte_Hex(uint8_t val){
    uint16_t hex_digit, index=8;
	  UART1_Transmit_Byte('0');
    UART1_Transmit_Byte('x');
	  for(uint8_t i=0;i<2;i++){
			index-=4;
			hex_digit=(uint8_t)((val>>index) & 0x0F);
			if(hex_digit>9){hex_digit+=55;}
      else {hex_digit+=48;}
			UART1_Transmit_Byte((uint8_t)hex_digit);
		}
}

void UART1_Transmit_Half_Word_Hex(uint16_t val){
    uint16_t hex_digit, index=16;
	  UART1_Transmit_Byte('0');
    UART1_Transmit_Byte('x');
	  for(uint8_t i=0;i<4;i++){
			index-=4;
			hex_digit=(uint8_t)((val>>index) & 0x0F);
			if(hex_digit>9){hex_digit+=55;}
      else {hex_digit+=48;}
			UART1_Transmit_Byte((uint8_t)hex_digit);
		}
}

void UART1_Transmit_Word_Hex(uint32_t val){
    uint16_t hex_digit, index=32;
	  UART1_Transmit_Byte('0');
    UART1_Transmit_Byte('x');
	  for(uint8_t i=0;i<8;i++){
			index-=4;
			hex_digit=(uint8_t)((val>>index) & 0x0F);
			if(hex_digit>9){hex_digit+=55;}
      else {hex_digit+=48;}
			UART1_Transmit_Byte((uint8_t)hex_digit);
		}
}

void UART1_Transmit_Byte_Bin(uint8_t val){
    uint8_t bin_digits[8];
    for(uint8_t i=0;i<8;i++){
        bin_digits[i]= (uint8_t)((val & 0x01)+48);
        val>>=1;
    }
    UART1_Transmit_Byte('0');
    UART1_Transmit_Byte('b');
    for(uint8_t i=8;i>0;i--){
        uint8_t index=(uint8_t)(i-1);
        UART1_Transmit_Byte(bin_digits[index]);
    }
}

void UART1_Transmit_Text(char *str){
    uint8_t i=0;
    while(str[i]!='\0'){
        UART1_Transmit_Byte(str[i]);
        i++;
    }
}

void UART1_Determine_Digit_Numbers(uint32_t num){
  uint8_t i=0;
  if(num==0){
    UART.Digits[0]=0;
    UART.InputNumberDigits=1;
  }else{
    while(num!=0){
      UART.Digits[i]=num%10;
      num/=10;
      i++;
    }
	UART.InputNumberDigits=i;
  }
}

void UART1_Transmit_Number_Digits(void){
  for(uint8_t i=UART.InputNumberDigits; i>0; i--){
    uint8_t temp=i;
    temp-=1;
    temp=UART.Digits[temp];
    temp+=48;
    UART1_Transmit_Byte(temp);
  }
}

void UART1_Transmit_Number(int32_t num){
  if(num<0){UART1_Transmit_Byte('-');num=-num;}
  UART1_Determine_Digit_Numbers((uint32_t)num);
  UART1_Transmit_Number_Digits();
}

void UART1_Transmit_Fixed_Digit_Number(int32_t num, uint8_t digits){
  if(num<0){UART1_Transmit_Byte('-');num=-num;}
  UART1_Determine_Digit_Numbers((uint32_t)num);
  for(int i=0;i<(digits-UART.InputNumberDigits);i++){
    UART1_Transmit_Byte(48);
  }
  UART1_Transmit_Number_Digits();
}

void UART1_Print_Debug_Data(int *data_in){
  for(int i=1;i<=data_in[0];i++){
    UART1_Transmit_Fixed_Digit_Number(data_in[i], 4);
    UART1_Transmit_Text("  ");
  }
}


uint8_t UART1_Read_From_Buffer(uint16_t index){
    return UART.Buffer[index];
}

uint16_t UART1_Current_Buffer_Index(void){
    return UART.BufferIndex;
}

uint16_t UART1_Current_Read_Index(void){
    return UART.ReadIndex;
}

uint16_t UART1_Bytes_Available_To_Read(void){
    return UART.AvailableBytes;
}

uint8_t UART1_Read(void){
    uint8_t current_byte=UART.Buffer[UART.ReadIndex];
    UART.ReadIndex++;
    if(UART.ReadIndex>=UART.BufferSize){
      UART.ReadIndex=0;
    }
    if(UART.AvailableBytes>0){
        UART.AvailableBytes--;
    }
  return current_byte;
}

void UART1_Flush_Buffer(void){
    UART.Error=0x00;
    for(uint16_t i=0;i<UART.BufferSize;i++){
        UART.Buffer[i]=0x00;
    }
    UART.ReadIndex=0;
    UART.BufferIndex=0;
    UART.AvailableBytes=0;
}

uint32_t UART1_Last_Byte_Recevied_Time_Stamp(void){
    return UART.LastRecivedTimeStamp;
}

void UART1_Interrupt_Service_Routine(void){
    volatile uint8_t received_byte=0;
	  received_byte=(uint8_t)UART1_Receive_Byte();
    if(UART.Error==0x00){
         UART.Buffer[UART.BufferIndex]=received_byte;
         UART.BufferIndex++;
         UART.AvailableBytes++;
         if(UART.AvailableBytes>UART.BufferSize){
             UART.AvailableBytes=UART.BufferSize;
             UART.ReadIndex++;
             if(UART.ReadIndex>=UART.BufferSize){
                 UART.ReadIndex=0;
             }
         }
         if(UART.BufferIndex>=UART.BufferSize){UART.BufferIndex=0;}
         //UART.LastRecivedTimeStamp=millis();
    }
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
