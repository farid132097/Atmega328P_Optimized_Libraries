
void        PID_Struct_Init(void);
void        PID_Set_Kp(uint8_t index, signed long kp);
void        PID_Set_Ki(uint8_t index, signed long ki);
void        PID_Set_Kd(uint8_t index, signed long kd);
void        PID_Set_Constants(uint8_t index, signed long kp, signed long ki, signed long kd);
void        PID_Set_SetPoint(uint8_t index, signed long set_point);
void        PID_Set_ProcessValue(uint8_t index, signed long process_value);
void        PID_Set_Max_IError(uint8_t index, signed long max_ierror);
void        PID_Set_Scaling_Factor(uint8_t index, signed long scale_fact);

void        PID_Reset_IError(uint8_t index);
void        PID_Calculate_Error(uint8_t index);
void        PID_Calculate_Error_Products(uint8_t index);
void        PID_Execute_Routine(uint8_t index);

signed long PID_Get_Kp(uint8_t index);
signed long PID_Get_Ki(uint8_t index);
signed long PID_Get_Kd(uint8_t index);

signed long PID_Get_Set_Point(uint8_t index);
signed long PID_Get_Process_Value(uint8_t index);

signed long PID_Get_PError(uint8_t index);
signed long PID_Get_IError(uint8_t index);
signed long PID_Get_DError(uint8_t index);
signed long PID_Get_Max_IError(uint8_t index);

signed long PID_Get_PError_Product(uint8_t index);
signed long PID_Get_IError_Product(uint8_t index);
signed long PID_Get_DError_Product(uint8_t index);
signed long PID_Get_Scaling_Factor(uint8_t index);

signed long PID_Get_Output(uint8_t index);
signed long PID_Get_Output_NonNegative(uint8_t index);

void        PID_Init(void);
void        PID_Init_Module(uint8_t index, signed long kp, signed long ki, signed long kd, signed long max_ierror, signed long scaling_fact);
