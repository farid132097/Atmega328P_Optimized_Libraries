/* 
 * File:   uart.c
 * Author: MD. Faridul Islam
 * Atmega328P UART Library
 * Created on October 30, 2022, 19:00
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define  UART_DOUBLE_SPEED
#define  UART_ENABLE_TX    
#define  UART_ENABLE_RX    
#define  UART_ENABLE_RX_INT
#define  UART_CIRCULAR_BUFFER_READ
#define  UART_LAST_RECEIVED_TIMESTAMP
#define  UART_AUXILIARY_PRINT_FUNCTIONS
#define  UART_DEBUG_DATA_PRINT_FUNCTIONS
#define  UART_BUFFER_SIZE       32

typedef struct uart_t{
  volatile uint8_t   Error;
  uint8_t            Digits[8];
  uint8_t            InputNumberDigits;

  #ifdef UART_ENABLE_RX_INT
  volatile uint8_t   Buffer[UART_BUFFER_SIZE];
  volatile uint16_t  BufferSize;
  volatile uint16_t  BufferIndex;
  #endif
    
  #ifdef UART_CIRCULAR_BUFFER_READ        
  volatile uint16_t  AvailableBytes;
  volatile uint16_t  ReadIndex;
  #endif

  #ifdef UART_LAST_RECEIVED_TIMESTAMP
  volatile uint32_t  LastRecivedTimeStamp;
  #endif    
}uart_t;


#ifdef UART_LAST_RECEIVED_TIMESTAMP
#include "timebase.h"
#endif

static uart_t UART;



void UART_Config_GPIO(void){
	
}

void UART_Config_Clock(void){
  
}

void UART_Config_BAUD_Rate(uint32_t baud_rate){
  #ifdef UART_DOUBLE_SPEED
  uint32_t UBRR_VAL = ((F_CPU/8)/baud_rate)-1 ;
  UCSR0A|=(1<<U2X0);  
  #else
  UCSR0A&=~(1<<U2X0);  
  uint32_t UBRR_VAL = ((F_CPU/16)/baud_rate)-1 ;
  #endif
  UBRR0H = (UBRR_VAL >> 8) & 0xFF;
  UBRR0L = UBRR_VAL & 0xFF ;
}

void UART_Config_Transmitter(void){
  #ifdef UART_ENABLE_TX   
  UCSR0B|=(1<<TXEN0);
  UCSR0C=(1<<UCSZ00)| (1<<UCSZ01);
  #endif
}

void UART_Config_Receiver(void){
  #ifdef UART_ENABLE_RX
  UCSR0B|=(1<<RXEN0);
  UCSR0C=(1<<UCSZ00)| (1<<UCSZ01);
  #endif
}

void UART_Config_Receiver_Interrupt(void){
  #ifdef UART_ENABLE_RX_INT
  UCSR0B|=(1<<RXCIE0);
  sei();
  #endif
}

void UART_Transmit_Byte(uint8_t val){
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0=val;
}

uint8_t UART_Receive_Byte(void){
  volatile uint8_t val=0;
  if( UCSR0A & (1<<FE0) ){
    val=UDR0;
    UART.Error=0x01;
  }else if(UCSR0A & (1<<DOR0) ){
    val=UDR0;
    UART.Error=0x02;
  }else{
    val=UDR0;
    UART.Error=0x00;
  }
  return val;
}

#ifdef UART_LAST_RECEIVED_TIMESTAMP
uint32_t UART_Reference_Time(void){
  return Timebase_Timer_Get_SubSeconds();
}
#endif

void UART_Struct_Init(void){
  UART.Error=0;
  for(uint8_t i=0;i<8;i++){
    UART.Digits[i]=0;
  }
  UART.InputNumberDigits=0;
    
  #ifdef UART_ENABLE_RX_INT
  UART.BufferSize=UART_BUFFER_SIZE;
  UART.BufferIndex=0;
  for(uint8_t i=0;i<UART.BufferSize;i++){
    UART.Buffer[i]=0;
  }
  #endif
    
  #ifdef UART_CIRCULAR_BUFFER_READ  
  UART.AvailableBytes=0;
  UART.ReadIndex=0;
  #endif

  #ifdef UART_LAST_RECEIVED_TIMESTAMP  
  UART.LastRecivedTimeStamp=0;
  #endif
}


void UART_Init(uint32_t baud){
  UART_Config_GPIO();
  UART_Config_Clock();
  UART_Config_BAUD_Rate(baud);
  UART_Config_Transmitter();
  UART_Config_Receiver();
  UART_Config_Receiver_Interrupt();
}

#ifdef UART_AUXILIARY_PRINT_FUNCTIONS
void UART_Transmit_Byte_Hex(uint32_t val){

  uint16_t hex_digit, index=0, loop_counter=0;
  if(val <= 0xFF){
    index=8;
    loop_counter=2;
  }else if(val <= 0xFFFF){
    index=16;
    loop_counter=4;     
  }else{
    index=32;
    loop_counter=8;
  }
  UART_Transmit_Byte('0');
  UART_Transmit_Byte('x');
	for(uint8_t i=0;i<loop_counter;i++){
	  index-=4;
	  hex_digit=(uint8_t)((val>>index) & 0x0F);
	  if(hex_digit>9){
	    hex_digit+=55;
	  }else {
	    hex_digit+=48;
	  }
	  UART_Transmit_Byte((uint8_t)hex_digit);
	}
}
#endif


#ifdef UART_AUXILIARY_PRINT_FUNCTIONS
void UART_Transmit_Byte_Bin(uint32_t val){
  uint8_t loop_counter=0;
  if(val <= 0xFF){
    loop_counter=7;
  }else if(val <= 0xFFFF){
    loop_counter=15;     
  }else{
    loop_counter=31;
  }
  
  UART_Transmit_Byte('0');
  UART_Transmit_Byte('b');
  for(int i=loop_counter; i>=0; i--){
    if( (val>>i) & 1){
      UART_Transmit_Byte( 49 );   
    }else{
      UART_Transmit_Byte( 48 );         
    }
  }
}
#endif


void UART_Transmit_Text(char *str){
    uint8_t i=0;
    while(str[i]!='\0'){
        UART_Transmit_Byte(str[i]);
        i++;
    }
}

void UART_Determine_Digit_Numbers(uint32_t num){
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

void UART_Transmit_Number_Digits(void){
  for(uint8_t i=UART.InputNumberDigits; i>0; i--){
    uint8_t temp=i;
    temp-=1;
    temp=UART.Digits[temp];
    temp+=48;
    UART_Transmit_Byte(temp);
  }
}

void UART_Transmit_Number(int32_t num){
  if(num<0){UART_Transmit_Byte('-');num=-num;}
  UART_Determine_Digit_Numbers((uint32_t)num);
  UART_Transmit_Number_Digits();
}


#ifdef UART_DEBUG_DATA_PRINT_FUNCTIONS
void UART_Transmit_Fixed_Digit_Number(int32_t num, uint8_t digits){
  if(num<0){UART_Transmit_Byte('-');num=-num;}
  UART_Determine_Digit_Numbers((uint32_t)num);
  for(int i=0;i<(digits-UART.InputNumberDigits);i++){
    UART_Transmit_Byte(48);
  }
  UART_Transmit_Number_Digits();
}
#endif


#ifdef UART_DEBUG_DATA_PRINT_FUNCTIONS
void UART_Print_Debug_Data(int *data_in){
  for(int i=1;i<=data_in[0];i++){
    UART_Transmit_Fixed_Digit_Number(data_in[i], 4);
    UART_Transmit_Text("  ");
  }
}
#endif


#ifdef UART_CIRCULAR_BUFFER_READ
uint8_t UART_Read_From_Buffer(uint16_t index){
  return UART.Buffer[index];
}
#endif


#ifdef UART_CIRCULAR_BUFFER_READ
uint16_t UART_Current_Buffer_Index(void){
  return UART.BufferIndex;
}
#endif


#ifdef UART_CIRCULAR_BUFFER_READ
uint16_t UART_Current_Read_Index(void){
  return UART.ReadIndex;
}
#endif


#ifdef UART_CIRCULAR_BUFFER_READ
uint16_t UART_Bytes_Available_To_Read(void){
  return UART.AvailableBytes;
}
#endif


#ifdef UART_CIRCULAR_BUFFER_READ
uint8_t UART_Read(void){
  
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
#endif



void UART_Flush_Buffer(void){
  #ifdef UART_ENABLE_RX_INT
  UART.Error=0x00;
  for(uint16_t i=0;i<UART.BufferSize;i++){
    UART.Buffer[i]=0x00;
  }
  UART.BufferIndex=0;
    
  #ifdef UART_CIRCULAR_BUFFER_READ   
  UART.ReadIndex=0;    
  UART.AvailableBytes=0;
  #endif
  
  #endif
}


#ifdef UART_LAST_RECEIVED_TIMESTAMP
uint32_t UART_Last_Byte_Recevied_Time_Stamp(void){
  return UART.LastRecivedTimeStamp;
}
#endif


#ifdef UART_ENABLE_RX_INT
void UART_Interrupt_Service_Routine(void){
  volatile uint8_t received_byte=0;
	received_byte=(uint8_t)UART_Receive_Byte();
  if(UART.Error==0x00){
    UART.Buffer[UART.BufferIndex]=received_byte;
    UART.BufferIndex++;

    #ifdef UART_CIRCULAR_BUFFER_READ
    UART.AvailableBytes++;
    if(UART.AvailableBytes>UART.BufferSize){
      UART.AvailableBytes=UART.BufferSize;
      UART.ReadIndex++;
      if(UART.ReadIndex>=UART.BufferSize){
        UART.ReadIndex=0;
      }
    }
    #endif

    if(UART.BufferIndex>=UART.BufferSize){
      UART.BufferIndex=0;
    }
    #ifdef UART_LAST_RECEIVED_TIMESTAMP
      UART.LastRecivedTimeStamp=UART_Reference_Time();
    #endif
  }
}
#endif


#ifdef UART_ENABLE_RX_INT
ISR(USART_RX_vect){
  UART_Interrupt_Service_Routine();
}
#endif
