



#include <avr/io.h>


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
  void           (*PFunc_V_V)(void);
  void           (*PFunc_V_SL)(signed long val);
  void           (*PFunc_V_SL_SL_SL)(signed long val1, signed long val2, signed long val3);
}PID_t;


PID_t PID_type;
PID_t *PID;

void PID_Struct_Init(void){
  PID=&PID_type;
  PID->Input.Kp = 0;
  PID->Input.Ki = 0;
  PID->Input.Kd = 0;
  PID->Input.SetPoint = 0;
  PID->Input.ProcessValue = 0;
  PID->Error.PError = 0;
  PID->Error.DError = 0;
  PID->Error.IError = 0;
  PID->Error.IErrorLimit = 0;
  PID->Error.LastError = 0;
  PID->Products.PError = 0;
  PID->Products.IError = 0;
  PID->Products.DError = 0;
  PID->Products.ScaleFact = 0;
  PID->Output = 0;
}


void PID_Set_Kp(signed long kp){
  PID->Input.Kp = kp;
}

void PID_Set_Ki(signed long ki){
  PID->Input.Ki = ki;
}

void PID_Set_Kd(signed long kd){
  PID->Input.Kd = kd;
}

void PID_Set_Constants(signed long kp, signed long ki, signed long kd){
  PID_Set_Kp(kp);
  PID_Set_Ki(ki);
  PID_Set_Kd(kd);
}

void PID_Set_SetPoint(signed long set_point){
  PID->Input.SetPoint = set_point;
}

void PID_Set_ProcessValue(signed long process_value){
  PID->Input.ProcessValue = process_value;
}

void PID_Set_Max_IError(signed long max_ierror){
  PID->Error.IErrorLimit = max_ierror;
}

void PID_Set_Scaling_Factor(signed long scale_fact){
  PID->Products.ScaleFact = scale_fact;
}






void PID_Reset_IError(void){
  PID->Error.IError = 0;
}

void PID_Calculate_Error(void){
  PID->Error.PError = PID->Input.SetPoint - PID->Input.ProcessValue;
  PID->Error.DError = PID->Error.PError - PID->Error.LastError;
  PID->Error.LastError = PID->Error.PError;
  PID->Error.IError += PID->Error.PError;
  if     ( PID->Error.IError > PID->Error.IErrorLimit ){
    PID->Error.IError = PID->Error.IErrorLimit;
  }
  else if( PID->Error.IError < -PID->Error.IErrorLimit ){
    PID->Error.IError = -PID->Error.IErrorLimit;
  }
}

void PID_Calculate_Error_Products(void){
  PID->Products.PError = ( PID->Error.PError * PID->Input.Kp );
  PID->Products.IError = ( PID->Error.IError * PID->Input.Ki );
  PID->Products.DError = ( PID->Error.DError * PID->Input.Kd );
  PID->Products.PError/= PID->Products.ScaleFact;
  PID->Products.IError/= PID->Products.ScaleFact;
  PID->Products.DError/= PID->Products.ScaleFact;
}

void PID_Execute_Routine(void){
  signed long control_value = 0;
  PID_Calculate_Error();
  PID_Calculate_Error_Products();
  control_value  = PID->Products.PError;
  control_value += PID->Products.IError;
  control_value += PID->Products.DError;
  PID->Output = control_value;
  if(control_value < 0){
    PID->OutputNonNegative = 0;
  }else{
    PID->OutputNonNegative = control_value;
  }
}

signed long PID_Get_Kp(void){
  return PID->Input.Kp;
}

signed long PID_Get_Ki(void){
  return PID->Input.Ki;
}

signed long PID_Get_Kd(void){
  return PID->Input.Kd;
}

signed long PID_Get_Set_Point(void){
  return PID->Input.SetPoint;
}

signed long PID_Get_Process_Value(void){
  return PID->Input.ProcessValue;
}

signed long PID_Get_PError(void){
  return PID->Error.PError;
}

signed long PID_Get_IError(void){
  return PID->Error.IError;
}

signed long PID_Get_DError(void){
  return PID->Error.DError;
}

signed long PID_Get_Max_IError(void){
  return PID->Error.IErrorLimit;
}

signed long PID_Get_PError_Product(void){
  return PID->Products.PError;
}

signed long PID_Get_IError_Product(void){
  return PID->Products.IError;
}

signed long PID_Get_DError_Product(void){
  return PID->Products.DError;
}

signed long PID_Get_Scaling_Factor(void){
  return PID->Products.ScaleFact;
}


signed long PID_Get_Output(void){
  return PID->Output;
}

signed long PID_Get_Output_NonNegative(void){
  return PID->OutputNonNegative;
}



void PID_Init(void){
  PID_Struct_Init();
}

void PID_Init_Module(signed long kp, signed long ki, signed long kd, signed long max_ierror, signed long scaling_fact){
  PID_Set_Kp(kp);
  PID_Set_Ki(ki);
  PID_Set_Kd(kd);
  PID_Set_Max_IError(max_ierror);
  PID_Set_Scaling_Factor(scaling_fact);
  PID_Reset_IError();
}