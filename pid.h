

#ifndef _PID_H_
#define _PID_H_


typedef struct PID_Input_t{
  signed long    Kp;
  signed long    Ki;
  signed long    Kd;
  signed long    SetPoint;
  signed long    ProcessValue;
}PID_Input_t;

typedef struct PID_Error_t{
  signed long    PError;
  signed long    DError;
  signed long    IError;
  signed long    IErrorLimit;
  signed long    LastError;
}PID_Error_t;

typedef struct PID_Products_t{
  signed long    PError;
  signed long    IError;
  signed long    DError;
  signed long    ScaleFact;
}PID_Products_t;

typedef struct PID_t{
  PID_Input_t    Input;
  PID_Error_t    Error;
  PID_Products_t Products;
  signed long    Output;
  signed long    OutputNonNegative;
  void           (*SetKp)(signed long val);
  void           (*SetKi)(signed long val);
  void           (*SetKd)(signed long val);
  void           (*SetConstants)(signed long kp, signed long ki, signed long kd);
  void           (*SetSetpoint)(signed long val);
  void           (*SetProcessValue)(signed long val);
  void           (*SetMaxIError)(signed long val);
  void           (*SetScalingFactor)(signed long val);
  void           (*ResetIError)(void);
  void           (*CalculateError)(void);
  void           (*CalculateErrorProducts)(void);
  void           (*ExecuteRoutine)(void);
  signed long    (*GetKp)(void);
  signed long    (*GetKi)(void);
  signed long    (*GetKd)(void);
  signed long    (*GetSetpoint)(void);
  signed long    (*GetProcessValue)(void);
  signed long    (*GetPError)(void);
  signed long    (*GetIError)(void);
  signed long    (*GetDError)(void);
  signed long    (*GetMaxIError)(void);
  signed long    (*GetPErrorProduct)(void);
  signed long    (*GetIErrorProduct)(void);
  signed long    (*GetDErrorProduct)(void);
  signed long    (*GetScalingFactor)(void);
  signed long    (*GetOutput)(void);
  signed long    (*GetOutputNonNegative)(void);
  void           (*Init)(void);
  void           (*InitModule)(signed long kp, signed long ki, signed long kd, signed long max_ierror, signed long scaling_fact);
}PID_t;



void        PID_Struct_Init(void);
void        PID_Set_Kp(signed long kp);
void        PID_Set_Ki(signed long ki);
void        PID_Set_Kd(signed long kd);
void        PID_Set_Constants(signed long kp, signed long ki, signed long kd);
void        PID_Set_SetPoint(signed long set_point);
void        PID_Set_ProcessValue(signed long process_value);
void        PID_Set_Max_IError(signed long max_ierror);
void        PID_Set_Scaling_Factor(signed long scale_fact);

void        PID_Reset_IError(void);
void        PID_Calculate_Error(void);
void        PID_Calculate_Error_Products(void);
void        PID_Execute_Routine(void);

signed long PID_Get_Kp(void);
signed long PID_Get_Ki(void);
signed long PID_Get_Kd(void);

signed long PID_Get_Set_Point(void);
signed long PID_Get_Process_Value(void);

signed long PID_Get_PError(void);
signed long PID_Get_IError(void);
signed long PID_Get_DError(void);
signed long PID_Get_Max_IError(void);

signed long PID_Get_PError_Product(void);
signed long PID_Get_IError_Product(void);
signed long PID_Get_DError_Product(void);
signed long PID_Get_Scaling_Factor(void);

signed long PID_Get_Output(void);
signed long PID_Get_Output_NonNegative(void);

void        PID_Init(void);
void        PID_Init_Module(signed long kp, signed long ki, signed long kd, signed long max_ierror, signed long scaling_fact);



#endif

