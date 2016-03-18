/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"
#include "EN_GPIO.h"
#include "Timer2.h"
#include "usart.h"
#include "stdio.h"
#include "Adc1.h"
#include "math.h"
#include "App.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
__IO uint32_t LsiFreq = 40000;
void IWDG_Init(void)
{
	/* Check if the system has resumed from IWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* IWDGRST flag set */
    /* Turn on LED1 */

    /* Clear reset flags */
    RCC_ClearFlag();
  }
  else
  {
    /* IWDGRST flag is not set */
    /* Turn off LED1 */
  }
	 /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* Set counter reload value to obtain 250ms IWDG TimeOut.
     Counter Reload Value = 250ms/IWDG counter clock period
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
													0.5s = 64
													1s = 32
   */
  IWDG_SetReload(LsiFreq/32);

  /* Reload IWDG counter */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */   
	u32 i = 0;
	IWDG_Init();
	
	EN_GPIO_Init();
	Relay_Status_No_Init();
	Signal_Relay_OFF();
	LED_OFF(LED_A_1_G_Group ,LED_A_1_G );
	LED_OFF(LED_B_1_G_Group ,LED_B_1_G );
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_1_R_Group ,LED_A_1_R );
	LED_OFF(LED_B_1_R_Group ,LED_B_1_R );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );
	LED_OFF(LED_NS_Group ,LED_NS  );
	LED_OFF(LED_ER_Group ,LED_ER );
	Timer2_Init();
	ADC1_Init();
	uart_init(115200);
	//printf("Helloworld!!\r\n");

	//T100us_Delay(50000);
	
  /* Infinite loop */
	/* Reload IWDG counter */
  IWDG_ReloadCounter(); 
  while (1)
  {
		/* Reload IWDG counter */
    IWDG_ReloadCounter(); 
		Power12V_Check();
		Voltage_Check();
		//printf("VA is %d\r\n",VA_Flag);
		//printf("VB is %d\r\n",VB_Flag);
		Status_Process();
		if( t1s_SW_counter < 500 )		//task tick.
		{
			t1s_SW_counter = 501;
			SW_Status_Check();
		}
		
  /* Enable ADC1 DMA */									//SAMPLE 1 
  // ADC_DMACmd(ADC1, ENABLE);
				//T100us_Delay(200);
		  /* Enable ADC1 */
		  /* Enable ADC1 */
  /* Enable ADC1 DMA */
  //ADC_DMACmd(ADC1, DISABLE);
																				//CACULATE

		

		
	



			
			//SAMPLE 2 
		 // ADC_DMACmd(ADC1, ENABLE);
			//	T100us_Delay(200);
		  /* Enable ADC1 */
		  /* Enable ADC1 */
			/* Enable ADC1 DMA */
			//ADC_DMACmd(ADC1, DISABLE);
#if 0		
		for(i=0;i<990;)
		{
			printf("%03d:%d\r\n",i,ADCConvertedValue[i]);
			i = i + 1;
		}
		
#endif
	//	printf("%d\r\n",temp);
		//printf("%d\r\n",temp2);
		//printf("%d\r\n",temp3);
		//printf("%d\r\n",lv_counter);
		


#if 0		
		T100us_Delay(50000);		//DELAY 20S
		Relay_Status_No_Init();
		T100us_Delay(50000);
		Relay_Status_No_Out();
		T100us_Delay(50000);
		Relay_Status_A_Out();
		T100us_Delay(50000);		//DELAY 20S
		Relay_Status_AtoB();
		T100us_Delay(20);				//DELAY 20MS
		Relay_Status_B_Out();
		T100us_Delay(50000);		//DELAY 20S		
		Relay_Status_BtoA();
		T100us_Delay(20);				//DELAY 20MS	
		Relay_Status_A_Out();		
		//T100us_Delay(200000);		//DELAY 20S		
#endif
		
		

  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
