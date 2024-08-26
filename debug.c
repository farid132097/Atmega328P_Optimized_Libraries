

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "debug.h"

#ifdef  DEBUG_USE_TIMER0
uint16_t TIM0_BAUD_9600[3] = {1230, 1180, 590};
uint16_t TIM0_BAUD_19200[3]= { 590,  570, 230};
uint16_t TIM0_BAUD_38400[3]= { 200,  210,  20};
#endif


#ifdef  DEBUG_USE_TIMER1
uint16_t TIM1_BAUD_9600[3] = {1230, 1180, 590};
uint16_t TIM1_BAUD_19200[3]= { 590,  570, 230};
uint16_t TIM1_BAUD_38400[3]= { 280,  280,  70};
#endif


#ifdef  DEBUG_USE_TIMER2
uint16_t TIM2_BAUD_9600[3] = {1230, 1180, 590};
uint16_t TIM2_BAUD_19200[3]= { 590,  570, 230};
uint16_t TIM2_BAUD_38400[3]= { 280,  270,  70};
#endif


typedef struct debug_t{
  volatile uint8_t   Error;
  volatile uint8_t   DataReg;
  volatile uint8_t   DataBusy;
  volatile uint8_t   Buf[DEBUG_RX_BUF_SIZE];
  volatile uint8_t   BufIndex;
  uint8_t            Digits[8];
  uint8_t            InputNumDigits;
}debug_t;

debug_t Debug;

void Debug_Struct_Init(void){
  Debug.Error=0;
  Debug.DataReg=0;
  Debug.DataBusy=1;
  for(uint8_t i=0;i<DEBUG_RX_BUF_SIZE;i++){
    Debug.Buf[i]=0;
  }
  Debug.BufIndex=0;
  for(uint8_t i=0;i<8;i++){
    Debug.Digits[i]=0;
  }
  Debug.InputNumDigits=0;
}

void Debug_Timings_Init(void){
  #ifdef DEBUG_USE_TIMER0
  TCCR0A=0x00;
  TCCR0B=0x00;
  TIMSK0=0x00;
  TIFR0 =0xff;
  #endif
  
  #ifdef DEBUG_USE_TIMER1
  TCCR1A=0x00;
  TCCR1B=0x00;
  TCCR1C=0x00;
  TIMSK1=0x00;
  TIFR1 =0xff;
  #endif
  
  #ifdef DEBUG_USE_TIMER2
  TCCR2A=0x00;
  TCCR2B=0x00;
  TIMSK2=0x00;
  TIFR2 =0xff;
  #endif
  
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
}

void Debug_Timer_Start(void){
  #ifdef DEBUG_USE_TIMER0
  TIFR0 =0xff;
  TIMSK0=0x00;
  TCNT0 =200;
  TCCR0B=(1<<CS02);
  TIFR0 =(1<<TOV0);
  TIMSK0=(1<<TOIE0);
  #define TIMER_OVF_VECT TIMER0_OVF_vect
  #endif

  #ifdef DEBUG_USE_TIMER1
  TIFR1 =0xff;
  TIMSK1=0x00;
  TCNT1 =53000;
  TCCR1B=(1<<CS10);
  TIFR1 =(1<<TOV1);
  TIMSK1=(1<<TOIE1);
  #define TIMER_OVF_VECT TIMER1_OVF_vect
  #endif
  
  #ifdef DEBUG_USE_TIMER2
  TIFR2 =0xff;
  TIMSK2=0x00;
  TCNT2 =10;
  TCCR2B=(1<<CS22);
  TIFR2 =(1<<TOV2);
  TIMSK2=(1<<TOIE2);
  #define TIMER_OVF_VECT TIMER2_OVF_vect
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
  DEBUG_TX_DDR |= (1<<DEBUG_TX_bp);
  DEBUG_TX_PORT|= (1<<DEBUG_TX_bp);
  DEBUG_RX_DDR &=~(1<<DEBUG_RX_bp);
  DEBUG_RX_PORT|= (1<<DEBUG_RX_bp);
  
  #ifdef DEBUG_GPIO_SWITCH_DEBUGGING
  DEBUG_TEST_DDR|=(1<<DEBUG_TEST_bp);
  DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
  #endif
}

void Debug_Tx_High(void){
  DEBUG_TX_PORT|= (1<<DEBUG_TX_bp);
}

void Debug_Tx_Low(void){
  DEBUG_TX_PORT&=~(1<<DEBUG_TX_bp);
}

void Debug_Tx_Set(uint8_t pin_state){
  if(pin_state==1){
    Debug_Tx_High();
  }else{
    Debug_Tx_Low();
  }
}



uint8_t Debug_Rx_Get(void){
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
  
  #ifdef DEBUG_USE_TIMER0
  TCNT0=0;
    #ifdef DEBUG_BAUD_RATE_9600
	#endif
	
	#ifdef DEBUG_BAUD_RATE_19200
	#endif
	
	#ifdef DEBUG_BAUD_RATE_38400
	TCCR0B=(1<<CS00);
	while(TCNT0<50){}
    TCNT0=0;
	#endif
  
  while(TCNT0<val){}
  TCCR0B=0;
  #endif
  
  #ifdef DEBUG_USE_TIMER1
  TCNT1=0;
  TCCR1B=(1<<CS10);
  while(TCNT1<val){}
  TCCR1B=0;
  #endif
  
  #ifdef DEBUG_USE_TIMER2
  TCNT2=0;
  TCCR2B=(1<<CS20);
  while(TCNT2<val){}
  TCCR2B=0;
  #endif
}


void Debug_Delay_Index_0(void){
  #ifdef  DEBUG_USE_TIMER0
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM0_BAUD_9600[0]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM0_BAUD_19200[0]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM0_BAUD_38400[0]);
    #endif
  #endif

  #ifdef  DEBUG_USE_TIMER1
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM1_BAUD_9600[0]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM1_BAUD_19200[0]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM1_BAUD_38400[0]);
    #endif
  #endif
  
  #ifdef  DEBUG_USE_TIMER2
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM2_BAUD_9600[0]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM2_BAUD_19200[0]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM2_BAUD_38400[0]);
    #endif
  #endif
}

void Debug_Delay_Index_1(void){
  #ifdef  DEBUG_USE_TIMER0
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM0_BAUD_9600[1]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM0_BAUD_19200[1]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM0_BAUD_38400[1]);
    #endif
  #endif

  #ifdef  DEBUG_USE_TIMER1
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM1_BAUD_9600[1]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM1_BAUD_19200[1]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM1_BAUD_38400[1]);
    #endif
  #endif
  
  #ifdef  DEBUG_USE_TIMER2
    #ifdef DEBUG_BAUD_RATE_9600
    Debug_Delay(TIM2_BAUD_9600[1]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_19200
    Debug_Delay(TIM2_BAUD_19200[1]);
    #endif
	
    #ifdef DEBUG_BAUD_RATE_38400
    Debug_Delay(TIM2_BAUD_38400[1]);
    #endif
  #endif
}

void Debug_Delay_Index_2(void){
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
  #endif
}


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
    Debug_Tx_Set(buf[i]);
	
	Debug_Delay_Index_0();    //tx one bit
  }
  SREG=sreg;
}

uint8_t Debug_Rx_Byte(void){
  if(Debug_Rx_Get()==0){
    uint8_t val=0, dval=0, st=1, sp=0;
	Debug_Delay_Index_2();    //half bit
	#ifdef DEBUG_GPIO_SWITCH_DEBUGGING
	DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
	st=Debug_Rx_Get();
	for(uint8_t i=0;i<8;i++){
	  Debug_Delay_Index_1();
	  #ifdef DEBUG_GPIO_SWITCH_DEBUGGING
	  DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
      DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
      #endif
	  if(Debug_Rx_Get()){
	    val|=0x80;
	  }else{
	    dval|=0x80;
	  }
	  if(i<7){
	    val>>=1;
	  }
	}
	Debug_Delay_Index_1();
	#ifdef DEBUG_GPIO_SWITCH_DEBUGGING
	DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
	sp=Debug_Rx_Get();
	if( (st==0) && (sp==1) ){
	  Debug.DataReg=val;
	  Debug.Error=0x00;
	}else{
	  Debug.Error=0x01;
	}
	
	#ifdef DEBUG_GPIO_SWITCH_DEBUGGING
	DEBUG_TEST_PORT|=(1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
  }else{
    Debug.Error=0x02;
  }
  return Debug.DataReg;
}

uint8_t Debug_Rx_Byte_Get(void){
  return Debug.DataReg;
}

void Debug_Rx_Byte_Clear(void){
  Debug.DataReg=0;
}

void Debug_Rx_Byte_Set(uint8_t val){
  Debug.DataReg=val;
}

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

void Debug_Determine_Digit_Numbers(uint32_t num){
  uint8_t i=0;
  if(num==0){
    Debug.Digits[0]=0;
    Debug.InputNumDigits=1;
  }else{
    while(num!=0){
      Debug.Digits[i]=num%10;
      num/=10;
      i++;
    }
	Debug.InputNumDigits=i;
  }
}

void Debug_Tx_Number_Digits(void){
  for(uint8_t i=Debug.InputNumDigits; i>0; i--){
    uint8_t temp=i;
    temp-=1;
    temp=Debug.Digits[temp];
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
  Debug_Tx_Text(",");
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
  Debug_Tx_Text(",");
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



uint8_t Debug_Buf_Get(uint8_t index){
  return Debug.Buf[index];
}

uint8_t Debug_Buf_Index_Get(void){
  return Debug.BufIndex;
}

void Debug_Flush_Buf(void){
  for(uint8_t i=0;i<DEBUG_RX_BUF_SIZE;i++){
    Debug.Buf[i]=0;
  }
  Debug.BufIndex=0;
}

uint8_t Debug_Data_Busy_Get(void){
  return Debug.DataBusy;
}

uint8_t Debug_Rx_Buf_Data_Available(void){
  if( (Debug_Data_Busy_Get()==0) && (Debug_Buf_Index_Get()!=0) ){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Debug_Rx_Buf_Available_Bytes(void){
  return Debug_Buf_Index_Get();
}

void Debug_Init(void){
  Debug_Struct_Init();
  Debug_Timings_Init();
  Debug_GPIO_Init();
}


ISR(ISR_PCINT_VECT){
  Debug.DataBusy=1;
  uint8_t val=Debug_Rx_Byte();
  if(Debug.Error==0){
    Debug.Buf[Debug.BufIndex]=val;
	Debug.BufIndex++;
	if(Debug.BufIndex>DEBUG_RX_BUF_SIZE){
	  Debug.BufIndex=0;
	}
    Debug_Timer_Start();
	#ifdef DEBUG_GPIO_SWITCH_DEBUGGING
    DEBUG_TEST_PORT|= (1<<DEBUG_TEST_bp);
    DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
    #endif
  }
}

ISR(TIMER_OVF_VECT){
  Debug_Timer_Stop();
  #ifdef DEBUG_GPIO_SWITCH_DEBUGGING
  DEBUG_TEST_PORT|= (1<<DEBUG_TEST_bp);
  DEBUG_TEST_PORT&=~(1<<DEBUG_TEST_bp);
  #endif
  Debug.DataBusy=0;
}
  
  
  
  
  
  
  
  