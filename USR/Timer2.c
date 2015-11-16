#include "Timer2.h"
__IO u32 t100us_counter;


void Timer2_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

    /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 100;				//100->100us
  TIM_TimeBaseStructure.TIM_Prescaler = (72-1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
  

}


/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		t100us_counter ++;
		if(t100us_counter > 0X0FFFFFFF)		//100us*10000 = 1s
		{
			t100us_counter = 0;
		}
	}
	
}

void T100us_Delay(u32 cc)				//key Delay , can not be interrupt
{
	t100us_counter = 0;
	while(t100us_counter < cc);
}




