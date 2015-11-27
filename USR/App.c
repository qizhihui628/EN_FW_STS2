#include "App.h"
#include "Adc1.h"
#include "math.h"
#include "stdio.h"

#define T1MS	1000
#define T2MS	20

__IO	u32 VRMS_A = 0;
__IO	u32 VRMS_B = 0;
__IO	u32 VRMS_OUT = 0;

__IO u8 Status_Flag = 0;
__IO u8 SW_Flag = 0;
__IO u8 VA_Flag = V_FAIL;
__IO u8 VB_Flag = V_FAIL;


enum Status_No{
	STATUS_IDLE,
	STATUS_AOUT1,
	STATUS_AOUT2,
	STATUS_BOUT1,
	STATUS_BOUT2
	
};


void Voltage_Check(void)
{
	u32 i = 0;
	u32 lv_A =0;
	u32 lv_B =0;
	u32 lv_OUT =0;
	u32 temp1 =0;
	u32 temp2 =0;
	u32 temp3 =0;	


	for(i=0;i<BUFFER_SIZE;)																											// Check VA	
		{
			temp1 = temp1 + (ADCConvertedValue[i] - MID_VALUE) * ( ADCConvertedValue[i] - MID_VALUE ) ;
			if(( ADCConvertedValue[i] > Z_MIN )&&( ADCConvertedValue[i] < Z_MAX ))		// low voltage counter.
			{
				lv_A++;
			}
			i = i+3;
		}
	VRMS_A = sqrt(temp1/(BUFFER_SIZE/3));
	if(	(VRMS_A < VRMS_MIN) || (VRMS_A > VRMS_MAX)	|| ( lv_A > Z_LIMIT) )			//VA error
	{
		VA_Flag = V_FAIL;
	}
	else																																				//VA OK
	{
		VA_Flag = V_OK;
	}


		for(i=1;i<BUFFER_SIZE;)																										// Check VB	
		{
			temp2 = temp2 + (ADCConvertedValue[i] - MID_VALUE) * ( ADCConvertedValue[i] - MID_VALUE ) ;
			if(( ADCConvertedValue[i] > Z_MIN )&&( ADCConvertedValue[i] < Z_MAX ))		// low voltage counter.
			{
				lv_B++;
			}
			i = i+3;
		}
	VRMS_B = sqrt(temp2/(BUFFER_SIZE/3));
	if(	(VRMS_B < VRMS_MIN) || (VRMS_B > VRMS_MAX)	|| ( lv_B > Z_LIMIT) )			//VB error
	{
		VB_Flag = V_FAIL;
	}
	else																																				//VB OK
	{
		VB_Flag = V_OK;
	}
	
}


void STATUS_IDLE_Pro_SW0(void)
{
	Relay_Status_No_Init();
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag == V_FAIL))		//Aout2
	{
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag == V_FAIL)&&(VB_Flag == V_OK))		//Bout2
	{
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_AOUT1_Pro(void)
{
	Relay_Status_A_Out_1();
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag == V_FAIL))		//Aout2
	{
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag == V_FAIL)&&(VB_Flag == V_OK))		//Bout2
	{
		Relay_OFF_OFF_ON_OFF();
		T100us_Delay(T2MS);
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_AOUT2_Pro(void)
{
	Relay_Status_A_Out_2();
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag == V_FAIL))		//Aout2
	{
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag == V_FAIL)&&(VB_Flag == V_OK))		//Bout2
	{
		Relay_ON_ON_ON_OFF();
		Relay_OFF_OFF_ON_OFF();
		T100us_Delay(T2MS);					//WAIT FOR 2MS			
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}


void STATUS_BOUT2_Pro(void)
{
	Relay_Status_B_Out_2();
	if((VA_Flag == V_OK)&&(VB_Flag == V_OK))					//Aout1
	{
		Relay_ON_OFF_ON_ON();	//A1 ON
		T100us_Delay(T1MS);				//WAIT FOR A1 READY
		Relay_ON_OFF_ON_OFF();	//CLOSE B1 B2						
		T100us_Delay(T2MS);					//WAIT FOR 4MS
		Status_Flag = STATUS_AOUT1;
	}
	else if((VA_Flag == V_OK)&&(VB_Flag == V_FAIL))		//Aout2
	{
		Relay_ON_OFF_ON_ON();
		Relay_ON_OFF_OFF_OFF();
		T100us_Delay(T2MS);					//WAIT FOR 2MS	
		Status_Flag = STATUS_AOUT2;
	}
	else if((VA_Flag == V_FAIL)&&(VB_Flag == V_OK))		//Bout2
	{
		Status_Flag = STATUS_BOUT2;
	}
	else																							//Idle
	{
		Status_Flag = STATUS_IDLE;
	}

}



void Status_Process(void)
{
	if( SW_Flag == 0 )		//A for Pri
	{
		switch (Status_Flag){
		case STATUS_IDLE:	STATUS_IDLE_Pro_SW0(); break;
		case STATUS_AOUT1:	STATUS_AOUT1_Pro();  break;
		case STATUS_AOUT2:	STATUS_AOUT2_Pro();  break;
		case STATUS_BOUT2:	STATUS_BOUT2_Pro();  break;
		default:						break;
		}
	}
	else if(SW_Flag == 1)	//B for Pri
	{
		switch (Status_Flag){
		case STATUS_IDLE:		break;
		case STATUS_AOUT2:	break;
		case STATUS_BOUT1:	break;
		case STATUS_BOUT2:	break;
		default:						break;
		}	
	}
	else
	{
			//All Relays off
	}
	
}




