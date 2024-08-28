/* 
 * File:   timebase.c
 * Author: MD. Faridul Islam
 * Atmega328P Timebase Library
 * Created on October 30, 2022, 19:00
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//#include "uart.h"

#define  TIMEBASE_UPCOUNTER     1
#define  TIMEBASE_DOWNCOUNTER   1
#define  TIMEBASE_TOKEN_FUNCTIONS
#define  TIMEBASE_TIME_WINDOW_CALCULATION

typedef union {
  struct {
    volatile uint8_t       WatchDogTimer  :1;
    volatile uint8_t       RealTimeCounter:1;
    volatile uint8_t       GeneralTimer   :1;
    volatile uint8_t       TimerIndex     :5;
  };
  volatile uint8_t         Value            ;
} timebase_timer_t;

typedef struct timebase_time_t{
  volatile uint8_t         OVFUpdateValue   ;
  volatile int32_t         LastSample       ;
  volatile uint16_t        SubSeconds       ;
  volatile int32_t         Seconds          ;
  #ifdef TIMEBASE_TIME_WINDOW_CALCULATION
  int32_t                  StartTimeSeconds   ;
  int32_t                  StartTimeSubSeconds;
  uint8_t                  Status             ;
  #endif
}timebase_time_t;

typedef union {
  struct {
    uint8_t               PeriodFlag     :1;
    uint8_t               Value          :4;
  };
  volatile uint8_t        StatusByte       ;
} timebase_status_t;


#ifdef TIMEBASE_UPCOUNTER
typedef struct timebase_upcounter_t{
  timebase_status_t       Status           ;
  int32_t                 EndValue         ;
  int32_t                 Target           ;
  int32_t                 Temporary        ;
  int32_t                 Value            ;
  int32_t                 PeriodValue      ;
}timebase_upcounter_t;
#endif

#ifdef TIMEBASE_DOWNCOUNTER
typedef struct timebase_downcounter_t{
  timebase_status_t       Status           ;
  int32_t                 EndValue         ;
  int32_t                 Value            ;
  int32_t                 PeriodValue      ;
}timebase_downcounter_t;
#endif

typedef struct timebase_config_t{
  timebase_timer_t        TimerType        ;
  volatile uint16_t       UpdateRate       ;
}timebase_config_t;

typedef struct timebase_t{
  timebase_config_t      Config                             ;
  timebase_time_t        Time                               ;
  
  #ifdef TIMEBASE_TOKEN_FUNCTIONS
  volatile uint8_t       ActiveTokens                       ;
  #endif
    
  #ifdef TIMEBASE_UPCOUNTER
  timebase_upcounter_t   UpCounter[TIMEBASE_UPCOUNTER]      ;
  #endif
  
  #ifdef TIMEBASE_DOWNCOUNTER
  timebase_downcounter_t DownCounter[TIMEBASE_DOWNCOUNTER]  ;
  #endif
  
}timebase_t;

enum{
  COUNTER_STATE_RESET = 0,
  COUNTER_STATE_START = 1,
  COUNTER_STATE_STARTED = 1,
  COUNTER_STATE_STOP = 2,
  COUNTER_STATE_STOPPED = 2,
  COUNTER_STATE_EXPIRED = 4
};
  

timebase_t Timebase_type;
timebase_t *Timebase;

void Timebase_Struct_Init(void){
  Timebase=&Timebase_type;
  Timebase->Config.TimerType.Value = 0;
  Timebase->Config.TimerType.GeneralTimer = 1;
  Timebase->Config.TimerType.TimerIndex = 0;
  Timebase->Config.UpdateRate = 1;
  Timebase->Time.OVFUpdateValue=0;
  Timebase->Time.SubSeconds = 0;
  Timebase->Time.Seconds = 0;
  Timebase->Time.LastSample = 0;
  
  #ifdef TIMEBASE_TIME_WINDOW_CALCULATION
    Timebase->Time.StartTimeSeconds = 0;
	Timebase->Time.StartTimeSubSeconds = 0;
	Timebase->Time.Status = 0;
  #endif
  
  #ifdef TIMEBASE_TOKEN_FUNCTIONS
  Timebase->ActiveTokens = 0;
  #endif

  #ifdef TIMEBASE_UPCOUNTER
  for(uint8_t i=0; i < TIMEBASE_UPCOUNTER; i++){
    Timebase->UpCounter[i].Status.StatusByte = 0;  
    Timebase->UpCounter[i].EndValue = 0;
    Timebase->UpCounter[i].Target = 0;
    Timebase->UpCounter[i].Temporary = 0;    
    Timebase->UpCounter[i].Value = 0;
    Timebase->UpCounter[i].PeriodValue = 0;
  }
  #endif

  #ifdef TIMEBASE_DOWNCOUNTER
  for(uint8_t i=0; i < TIMEBASE_DOWNCOUNTER; i++){
    Timebase->DownCounter[i].Status.StatusByte = 0; 
    Timebase->DownCounter[i].EndValue = 0;
    Timebase->DownCounter[i].Value = 0;
    Timebase->DownCounter[i].PeriodValue = 0;
  }
  #endif
}


void Timebase_Timer_Config(uint16_t UpdateRateHz){
  if(Timebase->Config.TimerType.WatchDogTimer == 1){
    uint8_t Prescaler_val = 0;
    if(UpdateRateHz == 1){
      Prescaler_val = 0x06;
    }else if(UpdateRateHz ==2 ){
      Prescaler_val = 0x05;
    }else if(UpdateRateHz == 4){
      Prescaler_val = 0x04;
    }else if(UpdateRateHz == 8){
      Prescaler_val = 0x03;
    }else if(UpdateRateHz == 16){
      Prescaler_val = 0x02;
    }else if(UpdateRateHz == 32){
      Prescaler_val = 0x01;
    }else if(UpdateRateHz == 64){
      Prescaler_val = 0x00;
    }else{
      Prescaler_val = 0x00;
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
    
	int32_t temp = F_CPU;
	uint16_t clock_div_factor[5] = {1, 8, 64, 256, 1024};
	
	temp /= UpdateRateHz;
	
	uint8_t  clock_div_index = 0;
	int32_t  curr_freq = temp;
	
	while( curr_freq > 0xFF ){
	  curr_freq  = temp;
	  curr_freq /= clock_div_factor[clock_div_index];
	  clock_div_index++;
	  if(clock_div_index == 5){
	    break;
	  }
	}
	
	clock_div_index -= 1;
	
	TCCR0A = 0x00;
    TCCR0B = 0x00;
    TIMSK0 = 0x00;
    TIFR0  = 0x00;
    OCR0A  = 0x00;
    OCR0B  = 0x00;
    TCNT0  = 0x00;
    OCR0A  = 0x00;
	
	if(clock_div_index == 0){
	  TCCR0B = (1<<CS00);
	}else if(clock_div_index == 1){
	  TCCR0B = (1<<CS01);
	}else if(clock_div_index == 2){
	  TCCR0B = (1<<CS00)|(1<<CS01);
	}else if(clock_div_index == 3){
	  TCCR0B = (1<<CS02);
	}else if(clock_div_index == 4){
	  TCCR0B = (1<<CS00)|(1<<CS02);
	}
	
	Timebase->Time.OVFUpdateValue = (0xFF-curr_freq);
	TCNT0  = Timebase->Time.OVFUpdateValue;
	TIMSK0 = (1<<TOIE0);
    sei();
  }
  Timebase->Config.UpdateRate = UpdateRateHz;
}


//Token Functions
#ifdef TIMEBASE_TOKEN_FUNCTIONS
uint8_t Timebase_Token_Executing(void){
  return Timebase->ActiveTokens;
}

void Timebase_Token_Add(void){ 
  if( Timebase_Token_Executing() < 63){
    Timebase->ActiveTokens += 1;
  }
}

void Timebase_Token_Remove(void){
  if( Timebase_Token_Executing() > 0){
    Timebase->ActiveTokens -= 1;
  }
}

void Timebase_Token_Remove_All(void){
  Timebase->ActiveTokens = 0;
}
#endif


//Timer Functions
uint16_t Timebase_Timer_Get_SubSeconds(void){
  return Timebase->Time.SubSeconds;
}

int32_t Timebase_Timer_Get_Seconds(void){
  return Timebase->Time.Seconds;
}

void Timebase_Timer_Set_SubSeconds(uint16_t value){
  Timebase->Time.SubSeconds = value;
}

void Timebase_Timer_Set_Seconds(int32_t value){
  Timebase->Time.Seconds = value;
}

void Timebase_Timer_Delay_SubSeconds(uint16_t value){
  #ifdef TIMEBASE_TOKEN_FUNCTIONS
  Timebase_Token_Add();
  #endif
  
  int32_t temp = Timebase_Timer_Get_SubSeconds();
  temp += value;
  while(temp > Timebase_Timer_Get_SubSeconds());
  
  #ifdef TIMEBASE_TOKEN_FUNCTIONS
  Timebase_Token_Remove();
  #endif
}


void Timebase_Timer_Await_SubSeconds(uint16_t value){
  while(Timebase_Timer_Get_SubSeconds() != Timebase->Time.LastSample);
  
  Timebase->Time.LastSample = Timebase_Timer_Get_SubSeconds() + value;
  if(Timebase->Time.LastSample >= Timebase->Config.UpdateRate){
    Timebase->Time.LastSample -= Timebase->Config.UpdateRate;
  }
}


void Timebase_Timer_Delay_Seconds(uint16_t value){
  int32_t temp = Timebase_Timer_Get_Seconds() + value;
  while(temp > Timebase_Timer_Get_Seconds());
}


void Timebase_Window_Timer_Reset(void){
  Timebase->Time.StartTimeSeconds = 0;
  Timebase->Time.StartTimeSubSeconds = 0;
  Timebase->Time.Status = COUNTER_STATE_RESET;
}

void Timebase_Window_Timer_Start(void){
  if(Timebase->Time.Status == COUNTER_STATE_RESET){
    Timebase->Time.StartTimeSeconds = Timebase_Timer_Get_Seconds();
    Timebase->Time.StartTimeSubSeconds = Timebase_Timer_Get_SubSeconds();
    Timebase->Time.Status = COUNTER_STATE_STARTED;
  }
}

//Unit in subseconds -> 20mS
int32_t Timebase_Window_Timer_Get_Interval(void){
  if(Timebase->Time.Status == COUNTER_STATE_STARTED){
    int32_t curr_s = Timebase_Timer_Get_Seconds();
    int32_t curr_ss = Timebase_Timer_Get_SubSeconds();
	curr_s -= Timebase->Time.StartTimeSeconds;
	curr_ss -= Timebase->Time.StartTimeSubSeconds;
	curr_s *= Timebase->Config.UpdateRate;
	curr_s += curr_ss;
	return curr_s;
  }else{
    return 0;
  }
}

int32_t Timebase_Window_Timer_Get_Interval_Reset(void){
  int32_t val=Timebase_Window_Timer_Get_Interval();
  if(Timebase->Time.Status == COUNTER_STATE_STARTED){
    Timebase_Window_Timer_Reset();
  }
  return val;
}



//UpCounter Functions
#ifdef TIMEBASE_UPCOUNTER
uint8_t Timebase_UpCounter_Get_Status(uint8_t window){
  return Timebase->UpCounter[window].Status.Value;
}

void Timebase_UpCounter_Set_Status(uint8_t window, uint8_t value){
  Timebase->UpCounter[window].Status.Value = value;
}

int32_t Timebase_UpCounter_Get_Value(uint8_t window){
  return Timebase->UpCounter[window].Value;
}

void Timebase_UpCounter_Set_Value(uint8_t window, int32_t value){
  Timebase->UpCounter[window].Value = value;
}

int32_t Timebase_UpCounter_Get_EndValue(uint8_t window){
  return Timebase->UpCounter[window].EndValue;
}

void Timebase_UpCounter_Set_EndValue(uint8_t window, int32_t value){
  Timebase->UpCounter[window].EndValue = value;
}

int32_t Timebase_UpCounter_Get_TargetValue(uint8_t window){
  return Timebase->UpCounter[window].Target;
}

void Timebase_UpCounter_Set_TargetValue(uint8_t window, int32_t value){
  Timebase->UpCounter[window].Target = value;
}

int32_t Timebase_UpCounter_Get_TemporaryValue(uint8_t window){
  return Timebase->UpCounter[window].Temporary;
}

void Timebase_UpCounter_Set_TemporaryValue(uint8_t window, int32_t value){
  Timebase->UpCounter[window].Temporary = value;
}

int32_t Timebase_UpCounter_Get_PeriodValue(uint8_t window){
  return Timebase->UpCounter[window].PeriodValue;
}

void Timebase_UpCounter_Set_PeriodValue(uint8_t window, int32_t value){
  Timebase->UpCounter[window].PeriodValue = value;
}

uint8_t Timebase_UpCounter_Get_Period_Flag(uint8_t window){
  return Timebase->UpCounter[window].Status.PeriodFlag;
}

void Timebase_UpCounter_Set_Period_Flag(uint8_t window){
  Timebase->UpCounter[window].Status.PeriodFlag = 1;
}

void Timebase_UpCounter_Clear_Period_Flag(uint8_t window){
  Timebase->UpCounter[window].Status.PeriodFlag = 0;
}

void Timebase_UpCounter_Reset(uint8_t window){
  Timebase_UpCounter_Set_Status(window, COUNTER_STATE_RESET);
  Timebase_UpCounter_Set_Value(window, 0);
  Timebase_UpCounter_Set_EndValue(window, 0);
  Timebase_UpCounter_Set_TargetValue(window, 0);
  Timebase_UpCounter_Set_TemporaryValue(window, 0);
  Timebase_UpCounter_Set_PeriodValue(window, 0);
  Timebase_UpCounter_Clear_Period_Flag(window);
} 

void Timebase_UpCounter_Clear_All_Flags(uint8_t window){
  Timebase_UpCounter_Reset( window );
}

void Timebase_UpCounter_Start(uint8_t window){
  if(Timebase_UpCounter_Get_Status(window) != COUNTER_STATE_STARTED){
    Timebase_UpCounter_Set_Status(window , COUNTER_STATE_START);    
  }
}

void Timebase_UpCounter_Stop(uint8_t window){
  if(Timebase_UpCounter_Get_Status(window) != COUNTER_STATE_STOPPED){
    Timebase_UpCounter_Set_Status(window , COUNTER_STATE_STOP);
  }
}


void Timebase_UpCounter_Set_Securely(uint8_t window, int32_t value){
  if( Timebase_UpCounter_Get_Status( window ) == COUNTER_STATE_RESET ){
    Timebase_UpCounter_Set_Value(window, 0);
    Timebase_UpCounter_Set_TemporaryValue(window, 0);
    Timebase_UpCounter_Set_TargetValue(window, value);
    Timebase_UpCounter_Set_EndValue(window, Timebase_Timer_Get_Seconds() + value);  
    Timebase_UpCounter_Start(window);
  }
}

void Timebase_UpCounter_Set_Forcefully(uint8_t window, int32_t value){
  Timebase_UpCounter_Reset( window );
  Timebase_UpCounter_Set_Securely( window, value );
} 

void Timebase_UpCounter_Update(uint8_t window){
  if( Timebase_UpCounter_Get_Status( window ) == COUNTER_STATE_STARTED ){ 
    Timebase_UpCounter_Set_TemporaryValue(window, Timebase_UpCounter_Get_EndValue(window) - Timebase_Timer_Get_Seconds() );
    Timebase_UpCounter_Set_Value(window, Timebase_UpCounter_Get_TargetValue(window) - Timebase_UpCounter_Get_TemporaryValue(window) );
    if(Timebase_UpCounter_Get_TemporaryValue(window) <= 0){
      Timebase_UpCounter_Set_EndValue(window, 0);
      Timebase_UpCounter_Set_TemporaryValue(window, 0);
      Timebase_UpCounter_Set_Value(window, Timebase_UpCounter_Get_TargetValue(window));
      Timebase_UpCounter_Set_Status(window, COUNTER_STATE_EXPIRED);
    }
  } else if (Timebase_UpCounter_Get_Status( window ) == COUNTER_STATE_STOPPED){
    Timebase_UpCounter_Set_EndValue(window, Timebase_UpCounter_Get_TemporaryValue(window) + Timebase_Timer_Get_Seconds());
    Timebase_UpCounter_Set_Value(window, Timebase_UpCounter_Get_TargetValue(window) - Timebase_UpCounter_Get_TemporaryValue(window));
  }
}


uint8_t Timebase_UpCounter_Expired(uint8_t window){
  if(Timebase_UpCounter_Get_Status( window ) == COUNTER_STATE_EXPIRED){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Timebase_UpCounter_Expired_Event(uint8_t window){
  if(Timebase_UpCounter_Get_Status( window ) == COUNTER_STATE_EXPIRED){
    Timebase_UpCounter_Clear_All_Flags( window );
    return 1;
  }else{
    return 0;
  }
}

void Timebase_UpCounter_Set_Period_Value_Securely(uint8_t window, int32_t value){
  if(Timebase_UpCounter_Get_Period_Flag( window ) == 0){
    Timebase_UpCounter_Set_PeriodValue( window, Timebase_UpCounter_Get_Value( window ) + value);
    Timebase_UpCounter_Set_Period_Flag( window);
  }
}

int32_t Timebase_UpCounter_Get_Remaining_Period_Value(uint8_t window){
  int32_t temp = Timebase_UpCounter_Get_PeriodValue( window ) - Timebase_UpCounter_Get_Value( window );
  if(temp < 0){
    temp = 0;
  }
  return temp;
}

uint8_t Timebase_UpCounter_Period_Value_Expired(uint8_t window){
  if( (Timebase_UpCounter_Get_Remaining_Period_Value( window ) == 0) && Timebase_UpCounter_Get_Period_Flag( window )){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Timebase_UpCounter_Period_Value_Expired_Event(uint8_t window){
  if(Timebase_UpCounter_Period_Value_Expired( window ) == 1){
    Timebase_UpCounter_Clear_Period_Flag( window );
    return 1;
  }else{
    return 0;
  }
}

void Timebase_UpCounter_Update_All(void){
  for(uint8_t i=0; i<TIMEBASE_UPCOUNTER; i++){
    Timebase_UpCounter_Update(i);
  }
}

void Timebase_UpCounter_Reset_All(void){
  for(uint8_t i=0; i<TIMEBASE_UPCOUNTER; i++){
    Timebase_UpCounter_Reset(i);
  }
}
#endif



#ifdef TIMEBASE_DOWNCOUNTER
uint8_t Timebase_DownCounter_Get_Status(uint8_t window){
  return Timebase->DownCounter[window].Status.Value;
}

void Timebase_DownCounter_Set_Status(uint8_t window, uint8_t value){
  Timebase->DownCounter[window].Status.Value = value;
}

int32_t Timebase_DownCounter_Get_Value(uint8_t window){
  return Timebase->DownCounter[window].Value;
}

void Timebase_DownCounter_Set_Value(uint8_t window, int32_t value){
  Timebase->DownCounter[window].Value = value;
}

int32_t Timebase_DownCounter_Get_EndValue(uint8_t window){
  return Timebase->DownCounter[window].EndValue;
}

void Timebase_DownCounter_Set_EndValue(uint8_t window, int32_t value){
  Timebase->DownCounter[window].EndValue = value;
}


int32_t Timebase_DownCounter_Get_PeriodValue(uint8_t window){
  return Timebase->DownCounter[window].PeriodValue;
}

void Timebase_DownCounter_Set_PeriodValue(uint8_t window, int32_t value){
  if(value < 0){
    Timebase->DownCounter[window].PeriodValue = 0;
  }else{
    Timebase->DownCounter[window].PeriodValue = value;
  }
  
}

uint8_t Timebase_DownCounter_Get_Period_Flag(uint8_t window){
  return Timebase->DownCounter[window].Status.PeriodFlag;
}

void Timebase_DownCounter_Set_Period_Flag(uint8_t window){
  Timebase->DownCounter[window].Status.PeriodFlag = 1;
}

void Timebase_DownCounter_Clear_Period_Flag(uint8_t window){
  Timebase->DownCounter[window].Status.PeriodFlag = 0;
}


void Timebase_DownCounter_Reset(uint8_t window){
  Timebase_DownCounter_Set_EndValue(window, 0);
  Timebase_DownCounter_Set_Value(window, 0);
  Timebase_DownCounter_Set_Status(window, COUNTER_STATE_RESET);
  Timebase_DownCounter_Clear_Period_Flag(window);
} 

void Timebase_DownCounter_Clear_All_Flags(uint8_t window){
  Timebase_DownCounter_Reset( window );
}

void Timebase_DownCounter_Start(uint8_t window){
  if(Timebase_DownCounter_Get_Status(window) != COUNTER_STATE_STARTED){
    Timebase_DownCounter_Set_Status(window, COUNTER_STATE_START); 
  }
}

void Timebase_DownCounter_Stop(uint8_t window){
  if(Timebase_DownCounter_Get_Status(window) != COUNTER_STATE_STOPPED){
    Timebase_DownCounter_Set_Status(window, COUNTER_STATE_STOP);    
  }
}


void Timebase_DownCounter_Set_Securely(uint8_t window, int32_t value){
  if( Timebase_DownCounter_Get_Status( window ) == COUNTER_STATE_RESET ){
    Timebase_DownCounter_Set_Value(window, value);
    Timebase_DownCounter_Set_EndValue(window, Timebase_Timer_Get_Seconds() + value);
    Timebase_DownCounter_Start(window);
  }
}

void Timebase_DownCounter_Set_Forcefully(uint8_t window, int32_t value){
  Timebase_DownCounter_Reset( window );
  Timebase_DownCounter_Set_Securely( window, value );
} 

void Timebase_DownCounter_Update(uint8_t window){
  if( Timebase_DownCounter_Get_Status( window ) == COUNTER_STATE_STARTED ){ 
    Timebase_DownCounter_Set_Value(window, Timebase_DownCounter_Get_EndValue(window) - Timebase_Timer_Get_Seconds());
    if(Timebase_DownCounter_Get_Value(window) <= 0){
      Timebase_DownCounter_Reset(window);
      Timebase_DownCounter_Set_Status(window, COUNTER_STATE_EXPIRED);
    }
  } else if (Timebase_DownCounter_Get_Status( window ) == COUNTER_STATE_STOPPED){ 
    Timebase_DownCounter_Set_EndValue(window, Timebase_DownCounter_Get_Value(window) + Timebase_Timer_Get_Seconds());
  }
}

uint8_t Timebase_DownCounter_Expired(uint8_t window){
  if(Timebase_DownCounter_Get_Status( window ) == COUNTER_STATE_EXPIRED){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Timebase_DownCounter_Expired_Event(uint8_t window){
  if(Timebase_DownCounter_Get_Status( window ) == COUNTER_STATE_EXPIRED){
    Timebase_DownCounter_Clear_All_Flags( window );
    return 1;
  }else{
    return 0;
  }
}


void Timebase_DownCounter_Set_Period_Value_Securely(uint8_t window, int32_t value){
  if(Timebase_DownCounter_Get_Period_Flag( window ) == 0){
    Timebase_DownCounter_Set_PeriodValue(window, Timebase_DownCounter_Get_Value(window) - value);
    Timebase_DownCounter_Set_Period_Flag(window);
  }
}

int32_t Timebase_DownCounter_Get_Remaining_Period_Value(uint8_t window){
  int32_t temp = Timebase_DownCounter_Get_Value(window) - Timebase_DownCounter_Get_PeriodValue(window) ;
  if(temp < 0){
    temp = 0;
  }
  return temp;
}

uint8_t Timebase_DownCounter_Period_Value_Expired(uint8_t window){
  if( (Timebase_DownCounter_Get_Remaining_Period_Value( window ) == 0) && Timebase_DownCounter_Get_Period_Flag( window )){
    return 1;
  }else{
    return 0;
  }
}

uint8_t Timebase_DownCounter_Period_Value_Expired_Event(uint8_t window){
  if(Timebase_DownCounter_Period_Value_Expired( window ) == 1){
    Timebase_DownCounter_Clear_Period_Flag( window );
    return 1;
  }else{
    return 0;
  }
}


void Timebase_DownCounter_Update_All(void){
  for(uint8_t i=0; i<TIMEBASE_DOWNCOUNTER; i++){
    Timebase_DownCounter_Update(i);
  }
}

void Timebase_DownCounter_Reset_All(void){
  for(uint8_t i=0; i<TIMEBASE_DOWNCOUNTER; i++){
    Timebase_DownCounter_Reset(i);
  }
}
#endif



void Timebase_Reset(void){
  #ifdef TIMEBASE_UPCOUNTER
  Timebase_UpCounter_Reset_All();
  #endif
  #ifdef TIMEBASE_DOWNCOUNTER
  Timebase_DownCounter_Reset_All();
  #endif
}

void Timebase_Init(uint16_t UpdateRateHz){
  Timebase_Struct_Init();
  Timebase_Timer_Config(UpdateRateHz);
  Timebase_Reset();
}

void Timebase_Main_Loop_Executables(void){
  #ifdef TIMEBASE_UPCOUNTER
  Timebase_UpCounter_Update_All();
  #endif
  #ifdef TIMEBASE_DOWNCOUNTER
  Timebase_DownCounter_Update_All();
  #endif
}

void Timebase_ISR_Executables(void){
  Timebase->Time.SubSeconds++;
  TCNT0  = Timebase->Time.OVFUpdateValue;
  if((Timebase->Time.SubSeconds % Timebase->Config.UpdateRate) == 0){
    Timebase->Time.Seconds++;
	#ifdef TIMEBASE_TOKEN_FUNCTIONS
    if(Timebase_Token_Executing() == 0){
      Timebase->Time.SubSeconds = 0;
    }
	#endif
  }
}

ISR(TIMER0_OVF_vect){
  PORTD|=(1<<5);
  Timebase_ISR_Executables();
  PORTD&=~(1<<5);
}








