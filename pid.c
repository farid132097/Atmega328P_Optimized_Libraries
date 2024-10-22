



#include <avr/io.h>
#include "pid.h"





PID_t PID = {
  .Input.Kp = 0,
  .Input.Ki = 0,
  .Input.Kd = 0,
  .Input.SetPoint = 0,
  .Input.ProcessValue = 0,
  .Error.PError = 0,
  .Error.DError = 0,
  .Error.IError = 0,
  .Error.IErrorLimit = 0,
  .Error.LastError = 0,
  .Products.PError = 0,
  .Products.IError = 0,
  .Products.DError = 0,
  .Products.ScaleFact = 0,
  .Output = 0,
  .OutputNonNegative = 0,
  .SetKp = &PID_Set_Kp,
  .SetKi = &PID_Set_Ki,
  .SetKd = &PID_Set_Kd,
  .SetConstants = &PID_Set_Constants,
  .SetSetpoint  = &PID_Set_SetPoint,
  .SetProcessValue = &PID_Set_ProcessValue,
  .SetMaxIError = &PID_Set_Max_IError,
  .SetScalingFactor = &PID_Set_Scaling_Factor,
  .ResetIError  = &PID_Reset_IError,
  .CalculateError   = &PID_Calculate_Error,
  .CalculateErrorProducts = &PID_Calculate_Error_Products,
  .ExecuteRoutine = &PID_Execute_Routine,
  .GetKp = &PID_Get_Kp,
  .GetKi = &PID_Get_Ki,
  .GetKd = &PID_Get_Kd,
  .GetSetpoint = &PID_Get_Set_Point,
  .GetProcessValue = &PID_Get_Process_Value,
  .GetPError = &PID_Get_PError,
  .GetIError = &PID_Get_PError,
  .GetPError = &PID_Get_PError,
  .GetDError = &PID_Get_DError,
  .GetMaxIError = &PID_Get_Max_IError,
  .GetPErrorProduct = &PID_Get_PError_Product,
  .GetIErrorProduct = &PID_Get_IError_Product,
  .GetDErrorProduct = &PID_Get_DError_Product,
  .GetScalingFactor = &PID_Get_Scaling_Factor,
  .GetOutput = &PID_Get_Output,
  .GetOutputNonNegative = &PID_Get_Output_NonNegative,
  .Init = &PID_Init,
  .InitModule = &PID_Init_Module
};



void PID_Struct_Init(void){
  PID.Input.Kp = 0;
  PID.Input.Ki = 0;
  PID.Input.Kd = 0;
  PID.Input.SetPoint = 0;
  PID.Input.ProcessValue = 0;
  PID.Error.PError = 0;
  PID.Error.DError = 0;
  PID.Error.IError = 0;
  PID.Error.IErrorLimit = 0;
  PID.Error.LastError = 0;
  PID.Products.PError = 0;
  PID.Products.IError = 0;
  PID.Products.DError = 0;
  PID.Products.ScaleFact = 0;
  PID.Output = 0;
  PID.OutputNonNegative = 0;
}


void PID_Set_Kp(signed long kp){
  PID.Input.Kp = kp;
}

void PID_Set_Ki(signed long ki){
  PID.Input.Ki = ki;
}

void PID_Set_Kd(signed long kd){
  PID.Input.Kd = kd;
}

void PID_Set_Constants(signed long kp, signed long ki, signed long kd){
  PID_Set_Kp(kp);
  PID_Set_Ki(ki);
  PID_Set_Kd(kd);
}

void PID_Set_SetPoint(signed long set_point){
  PID.Input.SetPoint = set_point;
}

void PID_Set_ProcessValue(signed long process_value){
  PID.Input.ProcessValue = process_value;
}

void PID_Set_Max_IError(signed long max_ierror){
  PID.Error.IErrorLimit = max_ierror;
}

void PID_Set_Scaling_Factor(signed long scale_fact){
  PID.Products.ScaleFact = scale_fact;
}






void PID_Reset_IError(void){
  PID.Error.IError = 0;
}

void PID_Calculate_Error(void){
  PID.Error.PError = PID.Input.SetPoint - PID.Input.ProcessValue;
  PID.Error.DError = PID.Error.PError - PID.Error.LastError;
  PID.Error.LastError = PID.Error.PError;
  PID.Error.IError += PID.Error.PError;
  if     ( PID.Error.IError > PID.Error.IErrorLimit ){
    PID.Error.IError = PID.Error.IErrorLimit;
  }
  else if( PID.Error.IError < -PID.Error.IErrorLimit ){
    PID.Error.IError = -PID.Error.IErrorLimit;
  }
}

void PID_Calculate_Error_Products(void){
  PID.Products.PError = ( PID.Error.PError * PID.Input.Kp );
  PID.Products.IError = ( PID.Error.IError * PID.Input.Ki );
  PID.Products.DError = ( PID.Error.DError * PID.Input.Kd );
  PID.Products.PError/= PID.Products.ScaleFact;
  PID.Products.IError/= PID.Products.ScaleFact;
  PID.Products.DError/= PID.Products.ScaleFact;
}

void PID_Execute_Routine(void){
  signed long control_value = 0;
  PID_Calculate_Error();
  PID_Calculate_Error_Products();
  control_value  = PID.Products.PError;
  control_value += PID.Products.IError;
  control_value += PID.Products.DError;
  PID.Output = control_value;
  if(control_value < 0){
    PID.OutputNonNegative = 0;
  }else{
    PID.OutputNonNegative = control_value;
  }
}




signed long PID_Get_Kp(void){
  return PID.Input.Kp;
}

signed long PID_Get_Ki(void){
  return PID.Input.Ki;
}

signed long PID_Get_Kd(void){
  return PID.Input.Kd;
}

signed long PID_Get_Set_Point(void){
  return PID.Input.SetPoint;
}

signed long PID_Get_Process_Value(void){
  return PID.Input.ProcessValue;
}

signed long PID_Get_PError(void){
  return PID.Error.PError;
}

signed long PID_Get_IError(void){
  return PID.Error.IError;
}

signed long PID_Get_DError(void){
  return PID.Error.DError;
}

signed long PID_Get_Max_IError(void){
  return PID.Error.IErrorLimit;
}

signed long PID_Get_PError_Product(void){
  return PID.Products.PError;
}

signed long PID_Get_IError_Product(void){
  return PID.Products.IError;
}

signed long PID_Get_DError_Product(void){
  return PID.Products.DError;
}

signed long PID_Get_Scaling_Factor(void){
  return PID.Products.ScaleFact;
}


signed long PID_Get_Output(void){
  return PID.Output;
}

signed long PID_Get_Output_NonNegative(void){
  return PID.OutputNonNegative;
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