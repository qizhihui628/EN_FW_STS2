#include "App.h"
#include "Adc1.h"
#include "math.h"
#include "stdio.h"
#include "EN_GPIO.h"
#include "Timer2.h"

#define T1MS	1000
#define T3MS	1000
#define T2MS_MAX	50
#define T2MS_MIN 	20
#define VRMS_COUNTER_Max 5000			//4S to get the RMS Voltage.

__IO	u32 T2MS = T2MS_MIN;
__IO	u32 VRMS_A_Total = 0;
__IO	u32 VRMS_B_Total = 0;
__IO	u32 VRMS_A_counter = 0;
__IO	u32 VRMS_B_counter = 0;

__IO	u32 VRMS_A = 0;
__IO	u32 VRMS_B = 0;
__IO	u32 VRMS_OUT = 0;
__IO  int32_t sync_counter = 0;

__IO u8 Status_Flag = 0;
__IO u8 SW_Flag = 1;
__IO u8 VA_Flag = V_FAIL_1;
__IO u8 VB_Flag = V_FAIL_1;


enum Status_No{
	STATUS_IDLE,
	STATUS_AOUT1,
	STATUS_AOUT2,
	STATUS_BOUT1,
	STATUS_BOUT2
	
};

void STATUS_BOUT1_Pro_SW0(void)
{
		Relay_ON_OFF_OFF_OFF();	//CLOSE B1 B2						
		T100us_Delay(T2MS_MAX);					//WAIT FOR 5MS
		Relay_Status_A_Out_1();
		T100us_Delay(T3MS);				//WAIT FOR A2 READY
		Status_Flag = STATUS_AOUT1;
}
void STATUS_AOUT1_Pro_SW1(void)
{
		Relay_OFF_OFF_ON_OFF();	//CLOSE B1 B2						
		T100us_Delay(T2MS_MAX);					//WAIT FOR 5MS
		Status_Flag = STATUS_BOUT1;
}
void Power12V_Check(void)
{
	if( (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)) && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)) )
	{
//		Signal_Relay_OFF();
		LED_OFF(LED_ER_Group,LED_ER);
	}
	else
	{
//		Signal_Relay_ON();
		LED_ON(LED_ER_Group,LED_ER);
	}
}

void Voltage_Check(void)
{
	u32 i = 0;
	u32 lv_A =0;
	u32 lv_B =0;
	u32 lv_OUT =0;
	u32 temp1 =0;
	u32 temp2 =0;
	u32 temp3 =0;	
	int32_t syn_temp1 = 10000;		//default value
	int32_t syn_temp2 = 10000;
	u32 VRMS_A_temp = 0;
	u32 VRMS_B_temp = 0;


	for(i=0;i<BUFFER_SIZE;)																											// Check VA	
		{
			temp1 = temp1 + (ADCConvertedValue[i] - MID_VALUE) * ( ADCConvertedValue[i] - MID_VALUE ) ;
			if(( ADCConvertedValue[i] > Z_MIN )&&( ADCConvertedValue[i] < Z_MAX ))		// low voltage counter.
			{
				lv_A++;
			}
			if(syn_temp1 == 10000)
			{
				if(	((ADCConvertedValue[i] - MID_VALUE) < 20) && ((ADCConvertedValue[i] - MID_VALUE) > -20) )	//20 according the zero detector value
				{
					syn_temp1 = i;
				}
			}
			i = i+3;
		}
	VRMS_A_temp = sqrt(temp1/(BUFFER_SIZE/3));
		VRMS_A_Total = VRMS_A_Total + VRMS_A_temp;
		VRMS_A_counter ++ ;
		if(VRMS_A_counter > VRMS_COUNTER_Max)
		{
				VRMS_A_counter = 0;
			VRMS_A = VRMS_A_Total/VRMS_COUNTER_Max;
			printf("A:%d\r\n",VRMS_A);
				VRMS_A_Total = 0;
		}
	if ( lv_A > Z_LIMIT) 		//VA drop off error
	{
		VA_Flag = V_FAIL_1;
		LED_OFF(LED_A_1_G_Group ,LED_A_1_G );
		LED_OFF(LED_A_1_R_Group ,LED_A_1_R );
	}
	else if ((VRMS_A < VRMS_MIN_L) || (VRMS_A > VRMS_MAX_H))		//V rms error need to bigger the delay time
	{
		VA_Flag = V_FAIL_2;
		LED_OFF(LED_A_1_G_Group ,LED_A_1_G );
		LED_ON(LED_A_1_R_Group ,LED_A_1_R );
	}
	else if((VRMS_A >= VRMS_MIN_H) && (VRMS_A <= VRMS_MAX_L))																																				//VA OK
	{
		VA_Flag = V_OK;
		LED_ON(LED_A_1_G_Group ,LED_A_1_G );
		LED_OFF(LED_A_1_R_Group ,LED_A_1_R );
	}


		for(i=1;i<BUFFER_SIZE;)																										// Check VB	
		{
			temp2 = temp2 + (ADCConvertedValue[i] - MID_VALUE) * ( ADCConvertedValue[i] - MID_VALUE ) ;
			if(( ADCConvertedValue[i] > Z_MIN )&&( ADCConvertedValue[i] < Z_MAX ))		// low voltage counter.
			{
				lv_B++;
			}
			if(syn_temp2 == 10000)
			{
				if(	((ADCConvertedValue[i] - MID_VALUE) < 20) && ((ADCConvertedValue[i] - MID_VALUE) > -20) )
				{
					syn_temp2 = i;
				}
			}
			i = i+3;
		}
	VRMS_B_temp = sqrt(temp2/(BUFFER_SIZE/3));
		VRMS_B_Total = VRMS_B_Total + VRMS_B_temp;
		VRMS_B_counter ++ ;
		if(VRMS_B_counter > VRMS_COUNTER_Max)
		{
				VRMS_B_counter = 0;
			VRMS_B = VRMS_B_Total/VRMS_COUNTER_Max;
			printf("B:%d\r\n",VRMS_B);
				VRMS_B_Total = 0;
		}
	if ( lv_B > Z_LIMIT)			//VB error
	{
		VB_Flag = V_FAIL_1;
		LED_OFF(LED_B_1_G_Group ,LED_B_1_G );
		LED_OFF(LED_B_1_R_Group ,LED_B_1_R );
	}
	else if((VRMS_B < VRMS_MIN_L) || (VRMS_B > VRMS_MAX_H))		//V rms error need to bigger the delay time
	{
		VB_Flag = V_FAIL_2;
		LED_OFF(LED_B_1_G_Group ,LED_B_1_G );
		LED_ON(LED_B_1_R_Group ,LED_B_1_R );
	}
	else if((VRMS_B >= VRMS_MIN_H) && (VRMS_B <= VRMS_MAX_L))																																			//VB OK
	{
		VB_Flag = V_OK;
		LED_ON(LED_B_1_G_Group ,LED_B_1_G );
		LED_OFF(LED_B_1_R_Group ,LED_B_1_R );
	}
	//printf("1:%d\r\n",syn_temp1);
	//printf("2:%d\r\n",syn_temp2);
	if(sync_counter > 100)			//counter max
	{
		sync_counter = 100;
	}
	else if(sync_counter < 0)		//counter minimal
	{
		sync_counter = 0;
	}
	else												//counter scale
	{
		if((syn_temp1 - syn_temp2> 30) || (syn_temp1-syn_temp2 < -30 ))		//30 -> according the phase shift degree.
		{
			sync_counter++;
		}
		else
		{
			sync_counter--;
		}
	}
	if((sync_counter > 70) && (VB_Flag == V_OK) && (VA_Flag == V_OK))		//delta value for phase shift.
	{
		LED_ON(LED_NS_Group ,LED_NS );																	//not sync
	}
	else if((sync_counter < 30)|| (VB_Flag != V_OK) || (VA_Flag != V_OK))
	{
		LED_OFF(LED_NS_Group ,LED_NS );																	//sync or single Power.
	}
}


void STATUS_IDLE_Pro_SW0(void)
{
	Relay_Status_No_Init();
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );		
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		T100us_Delay(T2MS);	
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		T100us_Delay(T2MS);	
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
		T100us_Delay(T2MS);	
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_AOUT1_Pro_SW0(void)
{
	Relay_Status_A_Out_1();
	LED_ON(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );		
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
			Relay_OFF_OFF_ON_OFF();
			if(VA_Flag == V_FAIL_1)
			{
				T100us_Delay(T2MS_MIN);
			}
			else
			{
				T100us_Delay(T2MS_MAX);
			}
			Relay_Status_B_Out_2();
			T100us_Delay(T3MS);				//WAIT FOR B2 READY
			Status_Flag = STATUS_BOUT2;

	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_AOUT2_Pro_SW0(void)
{
	Relay_Status_A_Out_2();
	LED_ON(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );	
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
			Relay_ON_ON_ON_OFF();
			Relay_OFF_OFF_ON_OFF();
			if(VA_Flag == V_FAIL_1)
			{
				T100us_Delay(T2MS_MIN);
			}
			else
			{
				T100us_Delay(T2MS_MAX);
			}	
			Relay_Status_B_Out_2();
			T100us_Delay(T3MS);				//WAIT FOR B2 READY
			Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_BOUT2_Pro_SW0(void)
{
	Relay_Status_B_Out_2();
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );	
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_ON(LED_B_2_R_Group ,LED_B_2_R );
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Relay_ON_OFF_ON_ON();	//A1 ON
		T100us_Delay(T1MS);				//WAIT FOR A1 READY
		Relay_ON_OFF_OFF_OFF();	//CLOSE B1 B2						
		T100us_Delay(T2MS_MAX);					//WAIT FOR 4MS
		Relay_Status_A_Out_1();
		T100us_Delay(T3MS);				//WAIT FOR A2 READY
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		Relay_ON_OFF_ON_ON();
		Relay_ON_OFF_OFF_OFF();
		if(VB_Flag == V_FAIL_1)
		{
			T100us_Delay(T2MS_MIN);
		}
		else
		{
			T100us_Delay(T2MS_MAX);
		}		
		Relay_Status_A_Out_2();
		T100us_Delay(T3MS);				//WAIT FOR A2 READY
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}





void STATUS_IDLE_Pro_SW1(void)
{
	Relay_Status_No_Init();
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );		
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Bout1
	{
		T100us_Delay(T2MS);	
		Status_Flag = STATUS_BOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		T100us_Delay(T2MS);	
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
		T100us_Delay(T2MS);	
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_BOUT1_Pro_SW1(void)
{
	Relay_Status_B_Out_1();
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );		
	LED_ON(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Bout1
	{
		Status_Flag = STATUS_BOUT1;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
		Status_Flag = STATUS_BOUT2;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		Relay_ON_OFF_OFF_OFF();
		if( VB_Flag == V_FAIL_1)
		{
			T100us_Delay(T2MS_MIN);
		}
		else
		{
			T100us_Delay(T2MS_MAX);
		}
		Status_Flag = STATUS_AOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_BOUT2_Pro_SW1(void)
{
	Relay_Status_B_Out_2();
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_OFF(LED_A_2_R_Group ,LED_A_2_R );
	LED_ON(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );	
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Bout1
	{
		Status_Flag = STATUS_BOUT1;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
		Status_Flag = STATUS_BOUT2;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		Relay_ON_OFF_ON_ON();
		Relay_ON_OFF_OFF_OFF();
		if( VB_Flag == V_FAIL_1)
		{
			T100us_Delay(T2MS_MIN);
		}
		else
		{
			T100us_Delay(T2MS_MAX);
		}			
		Status_Flag = STATUS_AOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_AOUT2_Pro_SW1(void)
{
	Relay_Status_A_Out_2();
	LED_OFF(LED_A_2_G_Group ,LED_A_2_G );
	LED_ON(LED_A_2_R_Group ,LED_A_2_R );	
	LED_OFF(LED_B_2_G_Group ,LED_B_2_G );
	LED_OFF(LED_B_2_R_Group ,LED_B_2_R );
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Bout1
	{
		Relay_ON_ON_ON_OFF();	//
		T100us_Delay(T1MS);				//
		Relay_OFF_OFF_ON_OFF();	//				
		T100us_Delay(T2MS_MAX);					//
		Status_Flag = STATUS_BOUT1;
	}
	else if((VA_Flag != V_OK)&&(VB_Flag == V_OK))		//Bout2
	{
		Relay_ON_ON_ON_OFF();
		Relay_OFF_OFF_ON_OFF();
		if( VA_Flag == V_FAIL_1)
		{
			T100us_Delay(T2MS_MIN);
		}
		else
		{
			T100us_Delay(T2MS_MAX);
		}
		Status_Flag = STATUS_BOUT2;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag != V_OK))		//Aout2
	{
		Status_Flag = STATUS_AOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void Status_Process(void)
{
	if( SW_Flag == 1 )		//A for Pri
	{
		switch (Status_Flag){
		case STATUS_IDLE:	STATUS_IDLE_Pro_SW0(); break;
		case STATUS_AOUT1:	STATUS_AOUT1_Pro_SW0();  break;
		case STATUS_AOUT2:	STATUS_AOUT2_Pro_SW0();  break;
		case STATUS_BOUT2:	STATUS_BOUT2_Pro_SW0();  break;
		case STATUS_BOUT1:STATUS_BOUT1_Pro_SW0();	break;
		default:		Status_Flag = STATUS_IDLE;			break;
		}
	}
	else if(SW_Flag == 0)	//B for Pri
	{
		switch (Status_Flag){
		case STATUS_IDLE:		STATUS_IDLE_Pro_SW1();break;
		case STATUS_AOUT2:	STATUS_AOUT2_Pro_SW1();break;
		case STATUS_BOUT1:	STATUS_BOUT1_Pro_SW1();break;
		case STATUS_BOUT2:	STATUS_BOUT2_Pro_SW1();break;
		case STATUS_AOUT1:	STATUS_AOUT1_Pro_SW1();  break;
		default:		Status_Flag = STATUS_IDLE;			break;
		}	
	}
	else
	{
			//All Relays off
	}
	
}


void SW_Status_Check(void)
{
	
	if(SW_Flag != GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))
	{
		T100us_Delay(2000);		//delay for 200ms
		if(SW_Flag != GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))
		{
			SW_Flag = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
		}
	}
	else
	{
	
	}

}

//relay
void Signal_Relay_ON(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);

}

void Signal_Relay_OFF(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);

}




