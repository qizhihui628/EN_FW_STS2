#ifndef  __APP_H
#define  __APP_H
#include "stm32f10x.h"

#define V_FAIL 0
#define V_OK 1
#define MID_VALUE	2048
#define Z_MAX 2148
#define Z_MIN 1948
#define Z_LIMIT 15
#define VRMS_MAX 1500
#define VRMS_MIN 800

extern __IO	u32 VRMS_A;
extern __IO	u32 VRMS_B;
extern __IO	u32 VRMS_OUT;

extern __IO u8 Status_Flag;
extern __IO u8 SW_Flag;
extern __IO u8 VA_Flag;
extern __IO u8 VB_Flag;

extern void Voltage_Check(void);

#endif
