#ifndef  __EN_GPIO_H
#define  __EN_GPIO_H
#include "stm32f10x.h"

#define Relay_A_1 GPIO_Pin_12
#define Relay_A_2 GPIO_Pin_14
#define Relay_B_1 GPIO_Pin_13
#define Relay_B_2 GPIO_Pin_15
#define SW_Pin	GPIO_Pin_3
#define SW_Group	GPIOA

void EN_GPIO_Init(void);

void Relay_Status_B_Out_2(void);
void Relay_Status_B_Out_1(void);
void Relay_Status_A_Out_1(void);
void Relay_Status_A_Out_2(void);
//void Relay_Status_No_Out(void);
void Relay_Status_No_Init(void);
//void Relay_Status_AtoB(void);
//void Relay_Status_BtoA(void);

#endif
