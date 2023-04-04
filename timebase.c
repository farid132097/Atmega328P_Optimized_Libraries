
/* 
 * File:   timebase.c
 * Author: MD. Faridul Islam
 * Atmega328P Timebase Library
 * Created on October 30, 2022, 19:00
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define  TIMEBASE_DOWNCOUNTER_REQUIRED 3 //max 8

typedef union {
  struct {
    volatile uint8_t ActiveTokens  :6;
    volatile uint8_t ResetRequest  :1;
    volatile uint8_t Busy          :1;
  };
} timebase_status_t;

typedef union {
  struct {
    volatile uint8_t WatchDogTimer  :1;
    volatile uint8_t RealTimeCounter:1;
    volatile uint8_t GeneralTimer   :1;
    volatile uint8_t Number         :5;
  };
  volatile uint8_t TimerByte;
} timebase_timer_t;

typedef struct timebase_time_t{
  volatile uint32_t LastSample;
  volatile uint32_t SubSeconds;
  volatile uint32_t Seconds;
}timebase_upcounter_t;

typedef struct timebase_downcounter_t{
  uint8_t  Status;
  uint8_t  Freeze;
  int32_t  Seconds[TIMEBASE_DOWNCOUNTER_REQUIRED];
  int32_t  TimeStamp[TIMEBASE_DOWNCOUNTER_REQUIRED];
}timebase_downcounter_t;

typedef struct timebase_config_t{
  timebase_timer_t  TimerType;
  volatile uint16_t UpdateRate;
}timebase_config_t;

typedef struct timebase_t{
  timebase_status_t      Status     ;
  timebase_config_t      Config     ;
  timebase_upcounter_t   UpCounter  ;
  timebase_downcounter_t DownCounter;
}timebase_t;

timebase_t Timebase_type;
timebase_t *Timebase;

void Timebase_Struct_Init(void){
  Timebase=&Timebase_type;
  Timebase->Status.Busy=0;
  Timebase->Status.ResetRequest=0;
  Timebase->Status.ActiveTokens=0;
  Timebase->Config.TimerType.TimerByte=0;  
  Timebase->Config.TimerType.GeneralTimer=1;
  Timebase->Config.TimerType.Number=0;
  Timebase->Config.UpdateRate=1;
  Timebase->UpCounter.SubSeconds=0;
  Timebase->UpCounter.Seconds=0;
  Timebase->UpCounter.LastSample=0;
  for(uint8_t i=0; i < TIMEBASE_DOWNCOUNTER_REQUIRED; i++){
    Timebase->DownCounter.Seconds[i] = 0;
    Timebase->DownCounter.TimeStamp[i] = 0;
  }
  Timebase->DownCounter.Status=0;
  Timebase->DownCounter.Freeze=0;
}


void Timebase_Timer_Config(uint16_t UpdateRateHz){
  if(Timebase->Config.TimerType.WatchDogTimer == 1){
    uint8_t Prescaler_val=0;
    if(UpdateRateHz==1){
      Prescaler_val=0x06;
    }else if(UpdateRateHz==2){
      Prescaler_val=0x05;
    }else if(UpdateRateHz==4){
      Prescaler_val=0x04;
    }else if(UpdateRateHz==8){
      Prescaler_val=0x03;
    }else if(UpdateRateHz==16){
      Prescaler_val=0x02;
    }else if(UpdateRateHz==32){
      Prescaler_val=0x01;
    }else if(UpdateRateHz==64){
      Prescaler_val=0x00;
    }else{
      Prescaler_val=0x00;
    }
    cli();
    MCUSR  &=~(1<<WDRF);
    WDTCSR |= (1<<WDIF);
    WDTCSR |= (1<<WDCE)|(1<<WDE);
    WDTCSR  = (1<<WDIE)|Prescaler_val;
    sei();
  }else if(Timebase->Config.TimerType.RealTimeCounter == 1){
    //add rtc functions
  }else if(Timebase->Config.TimerType.GeneralTimer == 1){
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TIMSK0 = 0x00;
    TIFR0  = 0x00;
    OCR0A  = 0x00;
    OCR0B  = 0x00;
    TCNT0  = 0x00;
    OCR0A  = 250 ;
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS00)|(1<<CS01);
    TIMSK0 = (1<<OCIE0A);
    sei();
  }
  Timebase->Config.UpdateRate=UpdateRateHz;
}


void Timebase_Init(uint16_t UpdateRateHz){
  Timebase_Struct_Init();
  Timebase_Timer_Config(UpdateRateHz);
}

void Timebase_Wait_Unit_Time(void){
  while(Timebase->UpCounter.LastSample==Timebase->UpCounter.SubSeconds);
  Timebase->UpCounter.LastSample=Timebase->UpCounter.SubSeconds;
}

uint8_t Timebase_Total_Tokens_Executing(void){
  return Timebase->Status.ActiveTokens;
}

void Timebase_Add_Token(void){ 
  if( Timebase_Total_Tokens_Executing() < 63){
    Timebase->Status.ActiveTokens+=1;
  }
}

void Timebase_Remove_Token(void){
  if( Timebase_Total_Tokens_Executing() > 0){
    Timebase->Status.ActiveTokens-=1;
  }
}

void Timebase_Remove_All_Tokens(void){
  Timebase->Status.ActiveTokens=0;
}

void Timebase_Set_Status_Busy(void){
  Timebase->Status.Busy=1;
}

void Timebase_Clear_Status_Busy(void){
  Timebase->Status.Busy=0;
}

uint8_t Timebase_Check_Busy_Status(void){
  return Timebase->Status.Busy;
}

void Timebase_Set_Timer_Reset_Request_Status(void){
  Timebase->Status.ResetRequest=1;
}

void Timebase_Clear_Timer_Reset_Request_Status(void){
  Timebase->Status.ResetRequest=0;
}

uint8_t Timebase_Check_Timer_Reset_Request_Status(void){
  return Timebase->Status.ResetRequest;
}

uint32_t Timebase_Get_SubSeconds(void){
  return Timebase->UpCounter.SubSeconds;
}

uint32_t Timebase_Get_Seconds(void){
  return Timebase->UpCounter.Seconds;
}

void Timebase_Set_SubSeconds(uint32_t value){
  Timebase->UpCounter.SubSeconds=value;
}

void Timebase_Set_Seconds(uint32_t value){
  Timebase->UpCounter.Seconds=value;
}

void Timebase_Reset_SubSeconds(void){
  Timebase->UpCounter.SubSeconds=0;
}

void Timebase_Reset_Seconds(void){
  Timebase->UpCounter.Seconds=0;
}

void Timebase_Set_DownCounter_Status(uint8_t window){
  Timebase->DownCounter.Status |= (1<<window);
}

void Timebase_Clear_DownCounter_Status(uint8_t window){
  Timebase->DownCounter.Status &=~ (1<<window);
}

uint8_t Timebase_Check_DownCounter_Status(uint8_t window){
  if(Timebase->DownCounter.Status & (1<<window)){
    return 1;
  }else{
    return 0;
  }
}

void Timebase_Pause_DownCounter(uint8_t window){
  Timebase->DownCounter.Freeze |= (1<<window);
}

void Timebase_Resume_DownCounter(uint8_t window){
  Timebase->DownCounter.Freeze &=~ (1<<window);
}

uint8_t Timebase_Check_DownCounter_Freeze_Status(uint8_t window){
  if(Timebase->DownCounter.Freeze & (1<<window)){
    return 1;
  }else{
    return 0;
  }
}

void Timebase_Securely_Set_DownCounter_Seconds(uint8_t window, uint32_t value){
  if( Timebase_Check_DownCounter_Status(window) == 0 ){ //no operation ongoing
    Timebase->DownCounter.TimeStamp[window] = Timebase->UpCounter.Seconds + value;
    Timebase->DownCounter.Seconds[window] = value ;
    Timebase->DownCounter.Status |= (1<<window);
  }
}

void Timebase_Forcefully_Set_DownCounter_Seconds(uint8_t window, uint32_t value){
  Timebase_Clear_DownCounter_Status( window );
  Timebase_Securely_Set_DownCounter_Seconds( window, value );
} 

void Timebase_Reset_DownCounter(uint8_t window){
  Timebase->DownCounter.TimeStamp[window] = 0;
  Timebase->DownCounter.Seconds[window] = 0;
  Timebase_Clear_DownCounter_Status( window );
  Timebase_Resume_DownCounter( window );
} 

uint32_t Timebase_Get_DownCounter_Seconds(uint8_t window){
  if(Timebase_Check_DownCounter_Freeze_Status( window ) ==1 ){
    Timebase->DownCounter.TimeStamp[window] = Timebase->DownCounter.Seconds[window] + Timebase->UpCounter.Seconds;
  }else{
    if(Timebase->DownCounter.Seconds[window] > 0){
      Timebase->DownCounter.Seconds[window] = Timebase->DownCounter.TimeStamp[window] - Timebase->UpCounter.Seconds;
      if(Timebase->DownCounter.Seconds[window] < 0 ){
        Timebase->DownCounter.Seconds[window] = 0;
        Timebase_Clear_DownCounter_Status( window );
      }
    }else{
      Timebase->DownCounter.Seconds[window] = 0;
      Timebase_Clear_DownCounter_Status( window );
    }
  }
  return Timebase->DownCounter.Seconds[window];
}

void Timebase_Update_DownCounters(void){
  for(uint8_t i=0; i<TIMEBASE_DOWNCOUNTER_REQUIRED; i++){
    Timebase_Get_DownCounter_Seconds(i);
  }
}

void Timebase_Reset_DownCounters(void){
  for(uint8_t i=0; i<TIMEBASE_DOWNCOUNTER_REQUIRED; i++){
    Timebase_Reset_DownCounter(i);
  }
}

void Timebase_Safely_Reset_Timer(void){
  if(Timebase_Check_Timer_Reset_Request_Status() && (Timebase_Total_Tokens_Executing()==0)){
    Timebase_Set_Status_Busy();
    Timebase_Reset_SubSeconds();
    Timebase_Remove_All_Tokens();
    Timebase_Clear_Timer_Reset_Request_Status();
    Timebase_Clear_Status_Busy();
  }
}

void Timebase_ISR_Executables(void){
  Timebase->UpCounter.SubSeconds++;
  if((Timebase->UpCounter.SubSeconds % Timebase->Config.UpdateRate) == 0){
    Timebase->UpCounter.Seconds++;
    if(Timebase_Total_Tokens_Executing() == 0){
      Timebase->UpCounter.SubSeconds = 0;
    }
  }
}

ISR(TIMER0_COMPA_vect){
  Timebase_ISR_Executables();
}