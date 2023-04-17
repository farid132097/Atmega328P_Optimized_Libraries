

/* 
 * File:   timebase.h
 * Author: MD. Faridul Islam
 * Atmega328P Timebase Library
 * Created on October 30, 2022, 19:00
 */

void     Timebase_Struct_Init(void);
void     Timebase_Timer_Config(uint16_t UpdateRateHz);
void     Timebase_Wait_Unit_Time(void);
uint8_t  Timebase_Tokens_Executing(void);
void     Timebase_Add_Token(void);
void     Timebase_Remove_Token(void);
void     Timebase_Remove_All_Tokens(void);
uint32_t Timebase_Get_SubSeconds(void);
uint32_t Timebase_Get_Seconds(void);
void     Timebase_Set_SubSeconds(uint32_t value);
void     Timebase_Set_Seconds(uint32_t value);
void     Timebase_Reset_UpCounter(uint8_t window);
void     Timebase_Start_UpCounter(uint8_t window);
void     Timebase_Stop_UpCounter(uint8_t window);
uint8_t  Timebase_Check_UpCounter_Status(uint8_t window);
uint32_t Timebase_Get_UpCounter_Value(uint8_t window);
void     Timebase_Securely_Set_UpCounter(uint8_t window, uint32_t value);
void     Timebase_Forcefully_Set_UpCounter(uint8_t window, uint32_t value);
void     Timebase_Update_UpCounter(uint8_t window);
uint8_t  Timebase_Check_UpCounter_Periodic_Flag(uint8_t window);
void     Timebase_Set_UpCounter_Periodic_Flag(uint8_t window);
void     Timebase_Clear_UpCounter_Periodic_Flag(uint8_t window);
void     Timebase_Securely_Set_UpCounter_Periodic_Value(uint8_t window, uint32_t value);
uint32_t Timebase_Get_UpCounter_Remaining_Periodic_Value(uint8_t window);
uint8_t  Timebase_UpCounter_Periodic_Value_Expired(uint8_t window);
uint8_t  Timebase_UpCounter_Periodic_Value_Expired_Event(uint8_t window);
void     Timebase_UpCounter_Clear_Flags(uint8_t window);
uint8_t  Timebase_UpCounter_Expired(uint8_t window);
uint8_t  Timebase_UpCounter_Expired_Event(uint8_t window);
void     Timebase_Update_UpCounters(void);
void     Timebase_Reset_UpCounters(void);
void     Timebase_Reset_DownCounter(uint8_t window);
void     Timebase_Start_DownCounter(uint8_t window);
void     Timebase_Stop_DownCounter(uint8_t window);
uint8_t  Timebase_Check_DownCounter_Status(uint8_t window);
uint32_t Timebase_Get_DownCounter_Value(uint8_t window);
void     Timebase_Securely_Set_DownCounter(uint8_t window, uint32_t value);
void     Timebase_Forcefully_Set_DownCounter(uint8_t window, uint32_t value);
void     Timebase_Update_DownCounter(uint8_t window);
uint8_t  Timebase_Check_DownCounter_Periodic_Flag(uint8_t window);
void     Timebase_Set_DownCounter_Periodic_Flag(uint8_t window);
void     Timebase_Clear_DownCounter_Periodic_Flag(uint8_t window);
void     Timebase_Securely_Set_DownCounter_Periodic_Value(uint8_t window, uint32_t value);
uint32_t Timebase_Get_DownCounter_Remaining_Periodic_Value(uint8_t window);
uint8_t  Timebase_DownCounter_Periodic_Value_Expired(uint8_t window);
uint8_t  Timebase_DownCounter_Periodic_Value_Expired_Event(uint8_t window);
void     Timebase_DownCounter_Clear_Flags(uint8_t window);
uint8_t  Timebase_DownCounter_Expired(uint8_t window);
uint8_t  Timebase_DownCounter_Expired_Event(uint8_t window);
void     Timebase_Update_DownCounters(void);
void     Timebase_Reset_DownCounters(void);
void     Timebase_Reset(void);
void     Timebase_Init(uint16_t UpdateRateHz);
void     Timebase_Main_Loop_Executables(void);
void     Timebase_ISR_Executables(void);