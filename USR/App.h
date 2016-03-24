#ifndef  __APP_H
#define  __APP_H
#include "stm32f10x.h"

#define V_FAIL_1 1
#define V_FAIL_2 2
#define V_OK 0
#define MID_VALUE	2048
#define Z_MAX 2148
#define Z_MIN 1948
#define Z_LIMIT 50						//because the total sample point increased so ...
#define VRMS_MAX_H 1226				//265 -> 268
#define VRMS_MAX_L 1202				//260 -> 263
#define VRMS_MIN_H 904			//195  ->	198
#define VRMS_MIN_L 880				//190 -> 193

extern __IO	u32 VRMS_A;
extern __IO	u32 VRMS_B;
extern __IO	u32 VRMS_OUT;

extern __IO u8 Status_Flag;
extern __IO u8 SW_Flag;
extern __IO u8 VA_Flag;
extern __IO u8 VB_Flag;

extern void Voltage_Check(void);
extern void Status_Process(void);
extern void Signal_Relay_OFF(void);
extern void Signal_Relay_ON(void);
extern void SW_Status_Check(void);
extern void Power12V_Check(void);
#endif
