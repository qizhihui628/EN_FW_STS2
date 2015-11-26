#ifndef  __ADC1_H
#define  __ADC1_H
#include "stm32f10x.h"
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
//#define HOLD_REG_TEMP 0X00
//#define SAMPLE_NUM 2000

#define VA_OFFSET	0
#define VB_OFFSET	1
#define VOUT_OFFSET	2
#define V12_1 3
#define V12_2 4
#define BUFFER_SIZE 444
extern	void ADC1_Init(void);
extern void ADC1_Poll(void);

extern __IO uint16_t ADCConvertedValue[BUFFER_SIZE];

#endif
