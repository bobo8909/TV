#include "Encoder_task.h"

#define DEGREEMAX 360
#define MAXPPR	5000


void Encoder_task(void)
{
    static volatile u16 n_Counter = 0;
	u16 RotationalSpeed = 0;
    static u16 backupCounter = 0;

	
	if(g_TIMFlag.bits.EncoderFlag == 1)
	{
		g_TIMFlag.bits.EncoderFlag = 0;
		n_Counter =(u16) TIM_GetCounter(TIM1);
		RotationalSpeed = DEGREEMAX * 10 * n_Counter / 4 / MAXPPR;

		//g_BCM3SendVal.FrontSpeedHigh = 
		
		//printf("n_Count:%d\r\n",n_Counter - backupCounter);
        
        backupCounter = n_Counter;

	}
}
