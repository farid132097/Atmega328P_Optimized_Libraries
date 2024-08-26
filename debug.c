

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "debug.h"

/*Prescaler, offset, tx_bit, rx_bit, rx_half_bit*/
#ifdef  DEBUG_USE_TIMER0
uint16_t TIM0_BAUD_9600[5]  =  {2,  40, 109, 1180, 590};
uint16_t TIM0_BAUD_19200[5] =  {2,  40,  31,  570, 230};
uint16_t TIM0_BAUD_38400[5] =  {1, 100, 160,  210,  20};
uint16_t TIM0_BAUD_57600[5] =  {1, 100,  49,  210,  20};
uint16_t TIM0_BAUD_115200[5] = {1,  10,  40,  210,  20};
#endif


#ifdef  DEBUG_USE_TIMER1
uint16_t TIM1_BAUD_9600[5] = {1230, 1180, 590};
uint16_t TIM1_BAUD_19200[5]= { 590,  570, 230};
uint16_t TIM1_BAUD_38400[5]= { 280,  280,  70};
uint16_t TIM1_BAUD_57600[5] =  {1, 100, 160,  210,  20};
uint16_t TIM1_BAUD_115200[5] = {1, 100, 160,  210,  20};
#endif


#ifdef  DEBUG_USE_TIMER2
uint16_t TIM2_BAUD_9600[5] = {1230, 1180, 590};
uint16_t TIM2_BAUD_19200[5]= { 590,  570, 230};
uint16_t TIM2_BAUD_38400[5]= { 280,  270,  70};
uint16_t TIM2_BAUD_57600[5] =  {1, 100, 160,  210,  20};
uint16_t TIM2_BAUD_115200[5] = {1, 100, 160,  210,  20};
#endif


typedef struct debug_t{
  volatile uint8_t   error;
  volatile uint8_t   datareg;
  volatile uint8_t   databsy;
  volatile uint8_t   buf[DEBUG_RX_BUF_SIZE];
  volatile uint8_t   bufindex;
  uint8_t            digits[8];
  uint8_t            input_num_digits;
}debug_t;

debug_t debug;

void Debug_Struct_Init(void){
  debug.error=0;
  debug.datareg=0;
  debug.databsy=1;
  for(uint8_t i=0;i<DEBUG_RX_BUF_SIZE;i++){
    debug.buf[i]=0;
  }
  debug.bufindex=0;
  for(uint8_t i=0;i<8;i++){
    debug.digits[i]=0;
  }
  debug.input_num_digits=0;
}

void Debug_Timings_Init(void){

  #ifdef DEBUG_USE_TIMER0
  TCCR0A=0x00;
  TCCR0B=0x00;
  TIMSK0=0x00;
  TIFR0 =0xff;
  #define TIMER_PRESCALER_REG TCCR0B
  #define TIMER_COUNT_REG     TCNT0
  #ifdef DEBUG_ENABLE_RX
  #define TIMER_OVF_VECT TIMER0_OVF_vect
  #endif
  #endif
  
  #ifdef DEBUG_USE_TIMER1
  TCCR1A=0x00;
  TCCR1B=0x00;
  TCCR1C=0x00;
  TIMSK1=0x00;
  TIFR1 =0xff;
  #define TIMER_PRESCALER_REG TCCR1B
  #define TIMER_COUNT_REG     TCNT1
  #ifdef DEBUG_ENABLE_RX
  #define TIMER_OVF_VECT TIMER1_OVF_vect
  #endif
  #endif
  
  #ifdef DEBUG_USE_TIMER2
  TCCR2A=0x00;
  TCCR2B=0x00;
  TIMSK2=0x00;
  TIFR2 =0xff;
  #define TIMER_PRESCALER_REG TCCR2B
  #define TIMER_COUNT_REG     TCNT2
  #ifdef DEBUG_ENABLE_RX
  #define TIMER_OVF_VECT TIMER2_OVF_vect
  #endif
  #endif
  
  
  
  #ifdef  DEBUG_ENABLE_RX
  
  #ifdef DEBUG_RX_DDRB
  PCICR |=(1<<PCIE0);
  PCMSK0|=(1<<DEBUG_RX_bp);
  PCIFR |=(1<<PCIF0);
  sei();
  #define ISR_PCINT_VECT PCINT0_vect
  #endif
  
  #ifdef DEBUG_RX_DDRC
  PCICR |=(1<<PCIE1);
  PCMSK1|=(1<<DEBUG_RX_bp);
  PCIFR |=(1<<PCIF1);
  sei();
  #define ISR_PCINT_VECT PCINT1_vect
  #endif
  
  #ifdef DEBUG_RX_DDRD
  PCICR |=(1<<PCIE2);
  PCMSK2|=(1<<DEBUG_RX_bp);
  PCIFR |=(1<<PCIF2);
  sei();
  #define ISR_PCINT_VECT PCINT2_vect
  #endif
  
  #endif
}

void Debug_Timer_Start(void){
  #ifdef DEBUG_USE_TIMER0
  TIFR0 =0xff;
  TIMSK0=0x00;
  TCNT0 =200;
  TCCR0B=(1<<CS02);
  TIFR0 =(1<<TOV0);
  TIMSK0=(1<<TOIE0);
  #endif

  #ifdef DEBUG_USE_TIMER1
  TIFR1 =0xff;
  TIMSK1=0x00;
  TCNT1 =53000;
  TCCR1B=(1<<CS10);
  TIFR1 =(1<<TOV1);
  TIMSK1=(1<<TOIE1);
  #endif
  
  #ifdef DEBUG_USE_TIMER2
  TIFR2 =0xff;
  TIMSK2=0x00;
  TCNT2 =10;
  TCCR2B=(1<<CS22);
  TIFR2 =(1<<TOV2);
  TIMSK2=(1<<TOIE2);
  #endif
}

void Debug_Timer_Stop(void){
  #ifdef DEBUG_USE_TIMER0
  TIMSK0=0;
  TCCR0B=0;
  TCNT0 =0;
  #endif
  
  #ifdef DEBUG_USE_TIMER1
  TIMSK1=0;
  TCCR1B=0;
  TCNT1 =0;
  #endif
  
  #ifdef DEBUG_USE_TIMER2
  TIMSK2=0;
  TCCR2B=0;
  TCNT2 =0;
  #endif
}

void Debug_GPIO_Init(void){
  #ifdef DEBUG_ENABLE_TX
  DEBUG_TX_DDR   |= (1<<DEBUG_TX_bp);
  DEBUG_TX_PORT  |= (1<<DEBUG_TX_bp);
  #endif
  
  #ifdef DEBUG_ENABLE_RX
  DEBUG_RX_DDR   &=~(1<<DEBUG_RX_bp);
  DEBUG_RX_PORT  |= (1<<DEBUG_RX_bp);
  #endif
  
  #ifdef DEBUG_ENABLE_DBG
  DEBUG_TEST_DDR |= (1<<DEBUG_TEST_bp);
  DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
  #endif
}

#ifdef DEBUG_ENABLE_TX
void Debug_Tx_Pin_High(void){
  DEBUG_TX_PORT  |= (1<<DEBUG_TX_bp);
} 
#endif

#ifdef DEBUG_ENABLE_TX
void Debug_Tx_Pin_Low(void){
  DEBUG_TX_PORT  &=~(1<<DEBUG_TX_bp);
}
#endif


#ifdef DEBUG_ENABLE_TX
void Debug_Tx_Pin_Set_State(uint8_t pin_state){
  if(pin_state==1){
    Debug_Tx_Pin_High();
  }else{
    Debug_Tx_Pin_Low();
  }
}
#endif


uint8_t Debug_Rx_Pin_Get_State(void){
  if(DEBUG_RX_PIN & (1<<DEBUG_RX_bp)){
    return 1;
  }else{
    return 0;
  }
}

void Debug_Delay(uint16_t val){
  #ifdef DEBUG_USE_DELAY
  _delay_us(val);
  #endif
  
  TIMER_COUNT_REG=0;
  #ifdef DEBUG_BAUD_RATE_9600
  TIMER_PRESCALER_REG=TIM0_BAUD_9600[0];
  while(TIMER_COUNT_REG<TIM0_BAUD_9600[1]){}
  TIMER_COUNT_REG=0;
  #endif
	
  #ifdef DEBUG_BAUD_RATE_19200
  TIMER_PRESCALER_REG=TIM0_BAUD_19200[0];
  while(TIMER_COUNT_REG<TIM0_BAUD_19200[1]){}
  TIMER_COUNT_REG=0;
  #endif
	
  #ifdef DEBUG_BAUD_RATE_38400
  TIMER_PRESCALER_REG=TIM0_BAUD_38400[0];
  while(TIMER_COUNT_REG<TIM0_BAUD_38400[1]){}
  TIMER_COUNT_REG=0;
  #endif
	
  #ifdef DEBUG_BAUD_RATE_57600
  TIMER_PRESCALER_REG=TIM0_BAUD_57600[0];
  while(TIMER_COUNT_REG<TIM0_BAUD_57600[1]){}
  TIMER_COUNT_REG=0;
  #endif
	
  #ifdef DEBUG_BAUD_RATE_115200
  TIMER_PRESCALER_REG=TIM0_BAUD_115200[0];
  while(TIMER_COUNT_REG<TIM0_BAUD_115200[1]){}
  TIMER_COUNT_REG=0;
  #endif
	
  while(TIMER_COUNT_REG<val){}
  TIMER_PRESCALER_REG=0;
}


void Debug_Delay_Tx_Bits(void){
  #ifdef  DEBUG_USE_TIMER0
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM0_BAUD_9600[2]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM0_BAUD_19200[2]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM0_BAUD_38400[2]);
    #endif
	
	#ifdef DEBUG_BAUD_RATE_57600
    Debug_Delay(TIM0_BAUD_57600[2]);
    #endif
	
	#ifdef DEBUG_BAUD_RATE_115200
    Debug_Delay(TIM0_BAUD_115200[2]);
    #endif
  #endif

  #ifdef  DEBUG_USE_TIMER1
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM1_BAUD_9600[2]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM1_BAUD_19200[2]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM1_BAUD_38400[2]);
    #endif
	
	#ifdef DEBUG_BAUD_RATE_57600
    Debug_Delay(TIM1_BAUD_57600[2]);
    #endif
	
	#ifdef DEBUG_BAUD_RATE_115200
    Debug_Delay(TIM1_BAUD_115200[2]);
    #endif
  #endif
  
  #ifdef  DEBUG_USE_TIMER2
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM2_BAUD_9600[2]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM2_BAUD_19200[2]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM2_BAUD_38400[2]);
    #endif
	
	#ifdef DEBUG_BAUD_RATE_57600
    Debug_Delay(TIM2_BAUD_57600[2]);
    #endif
	
	#ifdef DEBUG_BAUD_RATE_115200
    Debug_Delay(TIM2_BAUD_115200[2]);
    #endif
	
  #endif
}


#ifdef  DEBUG_ENABLE_RX
void Debug_Delay_Rx_Bits(void){
  #ifdef  DEBUG_USE_TIMER0
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM0_BAUD_9600[3]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM0_BAUD_19200[3]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM0_BAUD_38400[3]);
    #endif
  #endif

  #ifdef  DEBUG_USE_TIMER1
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM1_BAUD_9600[3]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM1_BAUD_19200[3]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM1_BAUD_38400[3]);
    #endif
  #endif
  
  #ifdef  DEBUG_USE_TIMER2
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM2_BAUD_9600[3]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM2_BAUD_19200[3]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM2_BAUD_38400[3]);
    #endif
  #endif
}
#endif


#ifdef  DEBUG_ENABLE_RX
void Debug_Delay_Rx_Half_Bit(void){
  #ifdef  DEBUG_USE_TIMER0
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM0_BAUD_9600[4]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM0_BAUD_19200[4]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM0_BAUD_38400[4]);
    #endif
  #endif

  #ifdef  DEBUG_USE_TIMER1
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM1_BAUD_9600[4]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM1_BAUD_19200[4]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM1_BAUD_38400[4]);
    #endif
  #endif
  
  #ifdef  DEBUG_USE_TIMER2
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM2_BAUD_9600[4]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM2_BAUD_19200[4]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM2_BAUD_38400[4]);
    #endif
  #endif
}
#endif

#ifdef DEBUG_ENABLE_TX
void Debug_Tx_Byte(uint8_t val){
  uint8_t buf[10]={0,0,0,0,0,0,0,0,0,1};
  for(uint8_t i=0;i<8;i++){
    if(val & (1<<i)){
	  buf[i+1]=1;
	}else{
	  buf[i+1]=0;
	}
  }
  uint8_t sreg=SREG;
  cli();
  for(uint8_t i=0;i<10;i++){
    Debug_Tx_Pin_Set_State(buf[i]);
	Debug_Delay_Tx_Bits();
  }
  SREG=sreg;
}
#endif


#ifdef DEBUG_ENABLE_RX
uint8_t Debug_Rx_Byte(void){
  if(Debug_Rx_Pin_Get_State()==0){
    uint8_t val=0, dval=0, st=1, sp=0;
	Debug_Delay_Rx_Half_Bit();
	#ifdef DEBUG_ENABLE_DBG
	DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
	st=Debug_Rx_Pin_Get_State();
	for(uint8_t i=0;i<8;i++){
	  Debug_Delay_Rx_Bits();
	  #ifdef DEBUG_ENABLE_DBG
	  DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
      DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
      #endif
	  if(Debug_Rx_Pin_Get_State()){
	    val|=0x80;
	  }else{
	    dval|=0x80;
	  }
	  if(i<7){
	    val>>=1;
	  }
	}
	Debug_Delay_Rx_Bits();
	#ifdef DEBUG_ENABLE_DBG
	DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
	sp=Debug_Rx_Pin_Get_State();
	if( (st==0) && (sp==1) ){
	  debug.datareg=val;
	  debug.error=0x00;
	}else{
	  debug.error=0x01;
	}
	
	#ifdef DEBUG_ENABLE_DBG
	DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
  }else{
    debug.error=0x02;
  }
  return debug.datareg;
}
#endif


/*
uint8_t debug_rx_byte_get(void){
  return debug.datareg;
}

void debug_rx_byte_clear(void){
  debug.datareg=0;
}

void debug_rx_byte_set(uint8_t val){
  debug.datareg=val;
}*/




////////////////////////////////////For Text & Commons//////////////////////////////////
void Debug_Tx_NL(void){
  Debug_Tx_Byte('\r');
  Debug_Tx_Byte('\n');
}

void Debug_Tx_SP(void){
  Debug_Tx_Byte(' ');
}

void Debug_Tx_CM(void){
  Debug_Tx_Byte(',');
}

void Debug_Tx_Text(char *str){
    uint8_t i=0;
    while(str[i]!='\0'){
        Debug_Tx_Byte(str[i]);
        i++;
    }
}

void Debug_Tx_Text_NL(char *str){
  Debug_Tx_Text(str);
  Debug_Tx_NL();
}

void Debug_Tx_Text_SP(char *str){
  Debug_Tx_Text(str);
  Debug_Tx_SP();
}

void Debug_Tx_Text_CM(char *str){
  Debug_Tx_Text(str);
  Debug_Tx_CM();
}



////////////////////////////////////For Decimal Numbers//////////////////////////////////

void Debug_Determine_Digit_Numbers(uint32_t num){
  uint8_t i=0;
  if(num==0){
    debug.digits[0]=0;
    debug.input_num_digits=1;
  }else{
    while(num!=0){
      debug.digits[i]=num%10;
      num/=10;
      i++;
    }
	debug.input_num_digits=i;
  }
}

void Debug_Tx_Number_Digits(void){
  for(uint8_t i=debug.input_num_digits; i>0; i--){
    uint8_t temp=i;
    temp-=1;
    temp=debug.digits[temp];
    temp+=48;
    Debug_Tx_Byte(temp);
  }
}

void Debug_Tx_Number(int32_t num){
  if(num<0){
    Debug_Tx_Byte('-');
	num=-num;
  }
  Debug_Determine_Digit_Numbers((uint32_t)num);
  Debug_Tx_Number_Digits();
}

void Debug_Tx_Number_NL(int32_t num){
  Debug_Tx_Number(num);
  Debug_Tx_NL();
}

void Debug_Tx_Number_SP(int32_t num){
  Debug_Tx_Number(num);
  Debug_Tx_SP();
}

void Debug_Tx_Number_CM(int32_t num){
  Debug_Tx_Number(num);
  Debug_Tx_CM();
}

void Debug_Tx_Parameter_NL(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_NL(num);
}

void Debug_Tx_Parameter_SP(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_SP(num);
}

void Debug_Tx_Parameter_CM(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_CM(num);
}



///////////////////////////For Hexadecimal Numbers//////////////////////////////////
void Debug_Tx_Hex(uint32_t val){
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
  Debug_Tx_Byte('0');
  Debug_Tx_Byte('x');
	for(uint8_t i=0;i<loop_counter;i++){
	  index-=4;
	  hex_digit=(uint8_t)((val>>index) & 0x0F);
	  if(hex_digit>9){
	    hex_digit+=55;
	  }else {
	    hex_digit+=48;
	  }
	  Debug_Tx_Byte((uint8_t)hex_digit);
	}
}

void Debug_Tx_Number_Hex_NL(int32_t num){
  Debug_Tx_Hex(num);
  Debug_Tx_NL();
}

void Debug_Tx_Number_Hex_SP(int32_t num){
  Debug_Tx_Hex(num);
  Debug_Tx_SP();
}

void Debug_Tx_Number_Hex_CM(int32_t num){
  Debug_Tx_Hex(num);
  Debug_Tx_CM();
}

void Debug_Tx_Parameter_Hex_NL(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_Hex_NL(num);
}

void Debug_Tx_Parameter_Hex_SP(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_Hex_SP(num);
}

void Debug_Tx_Parameter_Hex_CM(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_Hex_CM(num);
}


//////////////////////////////////For Binary Numbers////////////////////////////////////
void Debug_Tx_Bin(uint32_t val){
  uint8_t loop_counter=0;
  if(val <= 0xFF){
    loop_counter=7;
  }else if(val <= 0xFFFF){
    loop_counter=15;     
  }else{
    loop_counter=31;
  }
  
  Debug_Tx_Byte('0');
  Debug_Tx_Byte('b');
  for(int i=loop_counter; i>=0; i--){
    if( (val>>i) & 1){
      Debug_Tx_Byte( 49 );   
    }else{
      Debug_Tx_Byte( 48 );         
    }
  }
}

void Debug_Tx_Number_Bin_NL(int32_t num){
  Debug_Tx_Bin(num);
  Debug_Tx_NL();
}

void Debug_Tx_Number_Bin_SP(int32_t num){
  Debug_Tx_Bin(num);
  Debug_Tx_SP();
}

void Debug_Tx_Number_Bin_CM(int32_t num){
  Debug_Tx_Bin(num);
  Debug_Tx_CM();
}

void Debug_Tx_Parameter_Bin_NL(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_Bin_NL(num);
}

void Debug_Tx_Parameter_Bin_SP(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_Bin_SP(num);
}

void Debug_Tx_Parameter_Bin_CM(char *name, int32_t num){
  Debug_Tx_Text(name);
  Debug_Tx_SP();
  Debug_Tx_Number_Bin_CM(num);
}


/*
uint8_t debug_buf_get(uint8_t index){
  return debug.buf[index];
}

uint8_t debug_buf_index_get(void){
  return debug.bufindex;
}

void debug_flush_buf(void){
  for(uint8_t i=0;i<DEBUG_RX_BUF_SIZE;i++){
    debug.buf[i]=0;
  }
  debug.bufindex=0;
}

uint8_t debug_databsy_get(void){
  return debug.databsy;
}

uint8_t debug_data_available_rx_buf(void){
  if( (debug_databsy_get()==0) && (debug_buf_index_get()!=0) ){
    return 1;
  }else{
    return 0;
  }
}

uint8_t debug_available_bytes_rx_buf(void){
  return debug_buf_index_get();
}



ISR(ISR_PCINT_VECT){
  debug.databsy=1;
  uint8_t val=debug_rx_byte();
  if(debug.error==0){
    debug.buf[debug.bufindex]=val;
	debug.bufindex++;
	if(debug.bufindex>DEBUG_RX_BUF_SIZE){
	  debug.bufindex=0;
	}
    debug_timer_start();
	#ifdef DEBUG_GPIO_SWITCH_DEBUGGING
    DEBUG_TEST_PORT|= (1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
  }
}

ISR(TIMER_OVF_VECT){
  debug_timer_stop();
  #ifdef DEBUG_GPIO_SWITCH_DEBUGGING
  DEBUG_TEST_PORT|= (1<<DEBUG_TEST_bp);
  DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
  #endif
  debug.databsy=0;
}*/
  
  
  
void Debug_Init(void){
  Debug_Struct_Init();
  Debug_Timings_Init();
  Debug_GPIO_Init();
}
  
  
  
  