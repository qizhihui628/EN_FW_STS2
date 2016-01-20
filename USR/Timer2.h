#ifndef __TIMER2_H
#define __TIMER2_H
#include "stm32f10x.h"
void Timer2_Init(void);
extern void T100us_Delay(u32 cc);
extern __IO u32 t1s_SW_counter;


#endif
