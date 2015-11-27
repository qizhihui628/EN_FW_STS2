#include "EN_GPIO.h"
void EN_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOD Periph clock enable */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //disable JTAG.
	
//OUTPUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_8  | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | 
																GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//INPUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


/*	A1 ON 	A2 ON
		B1 OFF 	B2 OFF	*/
void Relay_Status_A_Out_2(void)
{
		GPIO_ResetBits(GPIOB,Relay_B_2);	//B2 OFF
		GPIO_ResetBits(GPIOB,Relay_B_1);		//B1 OFF
		GPIO_SetBits(GPIOB,Relay_A_1);		//A1 ON
		GPIO_SetBits(GPIOB,Relay_A_2);		//A2 ON
}


/*	A1 ON 	A2 ON
		B1 ON 	B2 OFF	*/
void Relay_Status_A_Out_1(void)
{
		GPIO_ResetBits(GPIOB,Relay_B_2);	//B2 OFF
		GPIO_SetBits(GPIOB,Relay_B_1);		//B1 ON
		GPIO_SetBits(GPIOB,Relay_A_1);		//A1 ON
		GPIO_SetBits(GPIOB,Relay_A_2);		//A2 ON
}

/*	A1 ON 	A2 OFF
		B1 ON 	B2 ON	*/
void Relay_Status_B_Out_1(void)
{
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_SetBits(GPIOB,Relay_A_1);
		GPIO_SetBits(GPIOB,Relay_B_1);
		GPIO_SetBits(GPIOB,Relay_B_2);
}

/*	A1 OFF 	A2 OFF
		B1 ON 	B2 ON	*/
void Relay_Status_B_Out_2(void)
{
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_ResetBits(GPIOB,Relay_A_1);
		GPIO_SetBits(GPIOB,Relay_B_1);
		GPIO_SetBits(GPIOB,Relay_B_2);
}


/*	A1 OFF 	A2 OFF
		B1 OFF 	B2 OFF	*/
void Relay_Status_No_Init(void)
{
		GPIO_ResetBits(GPIOB,Relay_A_1);
		GPIO_ResetBits(GPIOB,Relay_B_1);
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_ResetBits(GPIOB,Relay_B_2);
}

/*	A1 OFF 	A2 OFF
		B1 ON 	B2 OFF	*/
void Relay_OFF_OFF_ON_OFF(void)
{
		GPIO_ResetBits(GPIOB,Relay_A_1);
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_ResetBits(GPIOB,Relay_B_2);
		GPIO_SetBits(GPIOB,Relay_B_1);
}

/*	A1 ON 	A2 OFF
		B1 ON 	B2 ON	*/
void Relay_ON_OFF_ON_ON(void)
{
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_SetBits(GPIOB,Relay_A_1);
		GPIO_SetBits(GPIOB,Relay_B_1);
		GPIO_SetBits(GPIOB,Relay_B_2);
}
/*	A1 ON 	A2 OFF
		B1 ON 	B2 OFF	*/
void Relay_ON_OFF_ON_OFF(void)
{
		GPIO_ResetBits(GPIOB,Relay_B_2);
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_SetBits(GPIOB,Relay_A_1);
		GPIO_SetBits(GPIOB,Relay_B_1);
}

/*	A1 ON 	A2 OFF
		B1 OFF 	B2 OFF	*/
void Relay_ON_OFF_OFF_OFF(void)
{
		GPIO_ResetBits(GPIOB,Relay_B_2);
		GPIO_ResetBits(GPIOB,Relay_A_2);
		GPIO_ResetBits(GPIOB,Relay_B_1);
		GPIO_SetBits(GPIOB,Relay_A_1);

}

/*	A1 ON 	A2 ON
		B1 ON 	B2 OFF	*/
void Relay_ON_ON_ON_OFF(void)
{
		GPIO_ResetBits(GPIOB,Relay_B_2);
		GPIO_SetBits(GPIOB,Relay_A_2);
		GPIO_SetBits(GPIOB,Relay_B_1);
		GPIO_SetBits(GPIOB,Relay_A_1);

}



