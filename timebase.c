


/* 
 * File:   timebase.c
 * Author: MD. Faridul Islam
 * Atmega328P Timebase Library
 * Created on October 30, 2022, 19:00
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define  TIMEBASE_UPCOUNTER    3
#define  TIMEBASE_DOWNCOUNTER  3


typedef union {
  struct {
    volatile uint8_t          WatchDogTimer  :1;
    volatile uint8_t          RealTimeCounter:1;
    volatile uint8_t          GeneralTimer   :1;
    volatile uint8_t          TimerIndex     :5;
  };
  volatile uint8_t            Value            ;
} timebase_timer_t;

typedef struct timebase_time_t{
  volatile uint32_t           LastSample       ;
  volatile uint32_t           SubSeconds       ;
  volatile uint32_t           Seconds          ;
}timebase_time_t;

typedef struct timebase_upcounter_t{
  uint8_t                     Status           ;
  int32_t                     EndValue         ;
  int32_t                     TimeStamp        ;
  int32_t                     Value            ;
}timebase_upcounter_t;

typedef struct timebase_downcounter_t{
  uint8_t                     Status           ;
  int32_t                     EndValue         ;
  int32_t                     Value            ;
}timebase_downcounter_t;

typedef struct timebase_config_t{
  timebase_timer_t            TimerType        ;
  volatile uint16_t           UpdateRate       ;
}timebase_config_t;

typedef struct timebase_t{
  timebase_config_t      Config                             ;
  timebase_time_t        Time                               ;
  volatile uint8_t       ActiveTokens                       ;
  timebase_upcounter_t   UpCounter[TIMEBASE_UPCOUNTER]      ;
  timebase_downcounter_t DownCounter[TIMEBASE_DOWNCOUNTER]  ;
}timebase_t;

timebase_t Timebase_type;
timebase_t *Timebase;

void Timebase_Struct_Init(void){
  Timebase=&Timebase_type;
  Timebase->Config.TimerType.Value = 0;
  Timebase->Config.TimerType.GeneralTimer = 1;
  Timebase->Config.TimerType.TimerIndex = 0;
  Timebase->Config.UpdateRate = 1;
  Timebase->Time.SubSeconds = 0;
  Timebase->Time.Seconds = 0;
  Timebase->Time.LastSample = 0;
  Timebase->ActiveTokens = 0;
  for(uint8_t i=0; i < TIMEBASE_UPCOUNTER; i++){
    Timebase->UpCounter[i].Status = 0;  
    Timebase->UpCounter[i].EndValue = 0;
    Timebase->UpCounter[i].TimeStamp = 0;
    Timebase->UpCounter[i].Value = 0;
  }
  for(uint8_t i=0; i < TIMEBASE_DOWNCOUNTER; i++){
    Timebase->DownCounter[i].Status = 0; 
    Timebase->DownCounter[i].EndValue = 0;
    Timebase->DownCounter[i].Value = 0;
  }
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

void Timebase_Wait_Unit_Time(void){
  while(Timebase->Time.LastSample==Timebase->Time.SubSeconds);
  Timebase->Time.LastSample=Timebase->Time.SubSeconds;
}

uint8_t Timebase_Tokens_Executing(void){
  return Timebase->ActiveTokens;
}

void Timebase_Add_Token(void){ 
  if( Timebase_Tokens_Executing() < 63){
    Timebase->ActiveTokens+=1;
  }
}

void Timebase_Remove_Token(void){
  if( Timebase_Tokens_Executing() > 0){
    Timebase->ActiveTokens-=1;
  }
}

void Timebase_Remove_All_Tokens(void){
  Timebase->ActiveTokens=0;
}

uint32_t Timebase_Get_SubSeconds(void){
  return Timebase->Time.SubSeconds;
}

uint32_t Timebase_Get_Seconds(void){
  return Timebase->Time.Seconds;
}

void Timebase_Set_SubSeconds(uint32_t value){
  Timebase->Time.SubSeconds=value;
}

void Timebase_Set_Seconds(uint32_t value){
  Timebase->Time.Seconds=value;
}

void Timebase_Reset_UpCounter(uint8_t window){
  Timebase->UpCounter[window].Status = 0;
  Timebase->UpCounter[window].Value = 0;
  Timebase->UpCounter[window].EndValue = 0;
  Timebase->UpCounter[window].TimeStamp = 0;
} 

void Timebase_Start_UpCounter(uint8_t window){
  Timebase->UpCounter[window].Status = 1;
}

void Timebase_Stop_UpCounter(uint8_t window){
  Timebase->UpCounter[window].Status = 2;
}

uint8_t Timebase_Check_UpCounter_Status(uint8_t window){
  return Timebase->UpCounter[window].Status;
}

uint32_t Timebase_Get_UpCounter_Value(uint8_t window){
  return (uint32_t)Timebase->UpCounter[window].Value;
}

void Timebase_Securely_Set_UpCounter(uint8_t window, uint32_t value){
  if( Timebase_Check_UpCounter_Status( window ) == 0 ){
    Timebase->UpCounter[window].Value = 0;
    Timebase->UpCounter[window].TimeStamp = Timebase->Time.Seconds;
    Timebase->UpCounter[window].EndValue = Timebase->Time.Seconds + value;
    Timebase_Start_UpCounter(window);
  }
}

void Timebase_Forcefully_Set_UpCounter(uint8_t window, uint32_t value){
  Timebase_Reset_UpCounter( window );
  Timebase_Securely_Set_UpCounter( window, value );
} 

void Timebase_Update_UpCounter(uint8_t window){
  if( Timebase_Check_UpCounter_Status( window ) == 1 ){       //counter running
    Timebase->UpCounter[window].Value = Timebase->Time.Seconds - Timebase->UpCounter[window].TimeStamp;
    if(Timebase->Time.Seconds >= Timebase->UpCounter[window].EndValue){
      Timebase_Reset_UpCounter(window);
      Timebase->UpCounter[window].Status = 4;
    }
  } else if (Timebase_Check_UpCounter_Status( window ) == 2){ //counter stopped
    Timebase->UpCounter[window].EndValue = Timebase->UpCounter[window].Value + Timebase->Time.Seconds;
  }
}

void Timebase_UpCounter_Clear_Flags(uint8_t window){
  Timebase_Reset_UpCounter( window );
}

uint8_t Timebase_UpCounter_Expired(uint8_t window){
  if(Timebase_Check_UpCounter_Status( window ) == 4){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Timebase_UpCounter_Expired_Event(uint8_t window){
  if(Timebase_Check_UpCounter_Status( window ) == 4){
    Timebase_UpCounter_Clear_Flags( window );
    return 1;
  }else{
    return 0;
  }
}

void Timebase_Update_UpCounters(void){
  for(uint8_t i=0; i<TIMEBASE_UPCOUNTER; i++){
    Timebase_Update_UpCounter(i);
  }
}

void Timebase_Reset_UpCounters(void){
  for(uint8_t i=0; i<TIMEBASE_UPCOUNTER; i++){
    Timebase_Reset_UpCounter(i);
  }
}

void Timebase_Reset_DownCounter(uint8_t window){
  Timebase->DownCounter[window].EndValue = 0;
  Timebase->DownCounter[window].Value = 0;
  Timebase->DownCounter[window].Status = 0;
} 

void Timebase_Start_DownCounter(uint8_t window){
  Timebase->DownCounter[window].Status = 1;
}

void Timebase_Stop_DownCounter(uint8_t window){
  Timebase->DownCounter[window].Status = 2;
}

uint8_t Timebase_Check_DownCounter_Status(uint8_t window){
  return Timebase->DownCounter[window].Status;
}

uint32_t Timebase_Get_DownCounter_Value(uint8_t window){
  return (uint32_t)Timebase->DownCounter[window].Value;
}

void Timebase_Securely_Set_DownCounter(uint8_t window, uint32_t value){
  if( Timebase_Check_DownCounter_Status( window ) == 0 ){
    Timebase->DownCounter[window].Value = value;
    Timebase->DownCounter[window].EndValue = Timebase->Time.Seconds + value;
    Timebase_Start_DownCounter(window);
  }
}

void Timebase_Forcefully_Set_DownCounter(uint8_t window, uint32_t value){
  Timebase_Reset_DownCounter( window );
  Timebase_Securely_Set_DownCounter( window, value );
} 

void Timebase_Update_DownCounter(uint8_t window){
  if( Timebase_Check_DownCounter_Status( window ) == 1 ){       //counter running
    Timebase->DownCounter[window].Value = Timebase->DownCounter[window].EndValue - Timebase->Time.Seconds;
    if(Timebase->DownCounter[window].Value <= 0){
      Timebase_Reset_DownCounter(window);
      Timebase->DownCounter[window].Status = 4;                 //target reached
    }
  } else if (Timebase_Check_DownCounter_Status( window ) == 2){ //counter stopped
    Timebase->DownCounter[window].EndValue = Timebase->DownCounter[window].Value + Timebase->Time.Seconds;
  }
}

void Timebase_DownCounter_Clear_Flags(uint8_t window){
  Timebase_Reset_DownCounter( window );
}

uint8_t Timebase_DownCounter_Expired(uint8_t window){
  if(Timebase_Check_DownCounter_Status( window ) == 4){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Timebase_DownCounter_Expired_Event(uint8_t window){
  if(Timebase_Check_DownCounter_Status( window ) == 4){
    Timebase_DownCounter_Clear_Flags( window );
    return 1;
  }else{
    return 0;
  }
}

void Timebase_Update_DownCounters(void){
  for(uint8_t i=0; i<TIMEBASE_DOWNCOUNTER; i++){
    Timebase_Update_DownCounter(i);
  }
}

void Timebase_Reset_DownCounters(void){
  for(uint8_t i=0; i<TIMEBASE_DOWNCOUNTER; i++){
    Timebase_Reset_DownCounter(i);
  }
}

void Timebase_Reset(void){
  Timebase_Reset_UpCounters();
  Timebase_Reset_DownCounters();
}

void Timebase_Init(uint16_t UpdateRateHz){
  Timebase_Struct_Init();
  Timebase_Timer_Config(UpdateRateHz);
  Timebase_Reset();
}

void Timebase_Main_Loop_Executables(void){
  Timebase_Update_UpCounters();
  Timebase_Update_DownCounters();
}

void Timebase_ISR_Executables(void){
  Timebase->Time.SubSeconds++;
  if((Timebase->Time.SubSeconds % Timebase->Config.UpdateRate) == 0){
    Timebase->Time.Seconds++;
    if(Timebase_Tokens_Executing() == 0){
      Timebase->Time.SubSeconds = 0;
    }
  }
}

ISR(TIMER0_COMPA_vect){
  Timebase_ISR_Executables();
}