#ifndef  __EN_GPIO_H
#define  __EN_GPIO_H
#include "stm32f10x.h"

/*LED */


#define LED_A_1_G_Group GPIOA
#define LED_A_1_G 			GPIO_Pin_4

#define LED_A_1_R_Group GPIOA
#define LED_A_1_R 			GPIO_Pin_5

#define LED_B_1_G_Group GPIOA
#define LED_B_1_G 			GPIO_Pin_6

#define LED_B_1_R_Group GPIOA
#define LED_B_1_R	 			GPIO_Pin_11

#define LED_B_2_G_Group GPIOA
#define LED_B_2_G 			GPIO_Pin_15

#define LED_B_2_R_Group GPIOB
#define LED_B_2_R 			GPIO_Pin_9

#define LED_A_2_G_Group GPIOB
#define LED_A_2_G 			GPIO_Pin_8

#define LED_A_2_R_Group GPIOB
#define LED_A_2_R 			GPIO_Pin_5

#define LED_NS_Group GPIOB
#define LED_NS 			GPIO_Pin_4

#define LED_ER_Group GPIOB
#define LED_ER 			GPIO_Pin_3


#define LED_ON GPIO_SetBits
#define LED_OFF GPIO_ResetBits


/*LED */

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

extern void Relay_ON_ON_ON_OFF(void);
extern void Relay_ON_OFF_OFF_OFF(void);
extern void Relay_ON_OFF_ON_OFF(void);
extern void Relay_ON_OFF_ON_ON(void);
extern void Relay_OFF_OFF_ON_OFF(void);

#endif
