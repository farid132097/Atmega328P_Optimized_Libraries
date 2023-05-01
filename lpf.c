
/* 
 * File:   lpf.c
 * Author: MD. Faridul Islam
 * IIR Butterworth LPF Library
 * Created on May 1, 2023, 19:00
 */


#include <avr/io.h>
#define NUMBER_OF_LPF 3

typedef struct lpf_t{
  uint8_t  Alpha ;
  int32_t  Input ;
  int32_t  Output;
}lpf_t;

lpf_t LPF[NUMBER_OF_LPF];

void LPF_Struct_Init(void){
  for(uint8_t i=0; i<NUMBER_OF_LPF; i++){
    LPF[i].Alpha = 0;
    LPF[i].Input = 0;
	LPF[i].Output = 0;
  }
}

void LPF_Set_Alpha(uint8_t lpf_index, uint8_t val){
  LPF[lpf_index].Alpha = val;
}

uint8_t LPF_Get_Alpha(uint8_t lpf_index){
  return LPF[lpf_index].Alpha;
}

int32_t LPF_Filter(uint8_t lpf_index, int32_t val){
  return LPF[lpf_index].Output = (val * LPF_Get_Alpha(lpf_index)) + (LPF[lpf_index].Output * (100 - LPF_Get_Alpha(lpf_index)));
}

void LPF_Init(void){
  LPF_Struct_Init();
}