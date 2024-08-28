/* 
 * File:   timebase.h
 * Author: MD. Faridul Islam
 * Atmega328P Timebase Library
 * Created on October 30, 2022, 19:00
 */


//Config Functions
void     Timebase_Struct_Init(void);
void     Timebase_Timer_Config(uint16_t UpdateRateHz);


//Token Functions
uint8_t  Timebase_Token_Executing(void);
void     Timebase_Token_Add(void);
void     Timebase_Token_Remove(void);
void     Timebase_Token_Remove_All(void);

//Timer Functions
uint16_t Timebase_Timer_Get_SubSeconds(void);
int32_t  Timebase_Timer_Get_Seconds(void);
void     Timebase_Timer_Set_SubSeconds(uint16_t value);
void     Timebase_Timer_Set_Seconds(int32_t value);
void     Timebase_Timer_Delay_SubSeconds(uint16_t value);
void     Timebase_Timer_Await_SubSeconds(uint16_t value);
void     Timebase_Timer_Delay_Seconds(uint16_t value);
void     Timebase_Window_Timer_Reset(void);
void     Timebase_Window_Timer_Start(void);
int32_t  Timebase_Window_Timer_Get_Interval(void);
int32_t  Timebase_Window_Timer_Get_Interval_Reset(void);

//UpCounter Functions
uint8_t  Timebase_UpCounter_Get_Status(uint8_t window);
void     Timebase_UpCounter_Set_Status(uint8_t window, uint8_t value);
int32_t  Timebase_UpCounter_Get_Value(uint8_t window);
void     Timebase_UpCounter_Set_Value(uint8_t window, int32_t value);
int32_t  Timebase_UpCounter_Get_EndValue(uint8_t window);
void     Timebase_UpCounter_Set_EndValue(uint8_t window, int32_t value);
int32_t  Timebase_UpCounter_Get_TargetValue(uint8_t window);
void     Timebase_UpCounter_Set_TargetValue(uint8_t window, int32_t value);
int32_t  Timebase_UpCounter_Get_TemporaryValue(uint8_t window);
void     Timebase_UpCounter_Set_TemporaryValue(uint8_t window, int32_t value);
int32_t  Timebase_UpCounter_Get_PeriodValue(uint8_t window);
void     Timebase_UpCounter_Set_PeriodValue(uint8_t window, int32_t value);
uint8_t  Timebase_UpCounter_Get_Period_Flag(uint8_t window);
void     Timebase_UpCounter_Set_Period_Flag(uint8_t window);
void     Timebase_UpCounter_Clear_Period_Flag(uint8_t window);
void     Timebase_UpCounter_Reset(uint8_t window);
void     Timebase_UpCounter_Clear_All_Flags(uint8_t window);
void     Timebase_UpCounter_Start(uint8_t window);
void     Timebase_UpCounter_Stop(uint8_t window);
void     Timebase_UpCounter_Set_Securely(uint8_t window, int32_t value);
void     Timebase_UpCounter_Set_Forcefully(uint8_t window, int32_t value);
void     Timebase_UpCounter_Update(uint8_t window);
uint8_t  Timebase_UpCounter_Expired(uint8_t window);
uint8_t  Timebase_UpCounter_Expired_Event(uint8_t window);
void     Timebase_UpCounter_Set_Period_Value_Securely(uint8_t window, int32_t value);
int32_t  Timebase_UpCounter_Get_Remaining_Period_Value(uint8_t window);
uint8_t  Timebase_UpCounter_Period_Value_Expired(uint8_t window);
uint8_t  Timebase_UpCounter_Period_Value_Expired_Event(uint8_t window);
void     Timebase_UpCounter_Update_All(void);
void     Timebase_UpCounter_Reset_All(void);

//DownCounter Functions
uint8_t  Timebase_DownCounter_Get_Status(uint8_t window);
void     Timebase_DownCounter_Set_Status(uint8_t window, uint8_t value);
int32_t  Timebase_DownCounter_Get_Value(uint8_t window);
void     Timebase_DownCounter_Set_Value(uint8_t window, int32_t value);
int32_t  Timebase_DownCounter_Get_EndValue(uint8_t window);
void     Timebase_DownCounter_Set_EndValue(uint8_t window, int32_t value);
int32_t  Timebase_DownCounter_Get_PeriodValue(uint8_t window);
void     Timebase_DownCounter_Set_PeriodValue(uint8_t window, int32_t value);
uint8_t  Timebase_DownCounter_Get_Period_Flag(uint8_t window);
void     Timebase_DownCounter_Set_Period_Flag(uint8_t window);
void     Timebase_DownCounter_Clear_Period_Flag(uint8_t window);
void     Timebase_DownCounter_Reset(uint8_t window);
void     Timebase_DownCounter_Clear_All_Flags(uint8_t window);
void     Timebase_DownCounter_Start(uint8_t window);
void     Timebase_DownCounter_Stop(uint8_t window);
void     Timebase_DownCounter_Set_Securely(uint8_t window, int32_t value);
void     Timebase_DownCounter_Set_Forcefully(uint8_t window, int32_t value);
void     Timebase_DownCounter_Update(uint8_t window);
uint8_t  Timebase_DownCounter_Expired(uint8_t window);
uint8_t  Timebase_DownCounter_Expired_Event(uint8_t window);
void     Timebase_DownCounter_Set_Period_Value_Securely(uint8_t window, int32_t value);
int32_t  Timebase_DownCounter_Get_Remaining_Period_Value(uint8_t window);
uint8_t  Timebase_DownCounter_Period_Value_Expired(uint8_t window);
uint8_t  Timebase_DownCounter_Period_Value_Expired_Event(uint8_t window);
void     Timebase_DownCounter_Update_All(void);
void     Timebase_DownCounter_Reset_All(void);

//Common Functions
void     Timebase_Reset(void);
void     Timebase_Init(uint16_t UpdateRateHz);
void     Timebase_Main_Loop_Executables(void);
void     Timebase_ISR_Executables(void);


