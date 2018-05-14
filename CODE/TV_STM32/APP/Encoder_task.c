#include "Encoder_task.h"



void Encoder_task(void)
{
    static volatile u16 n_Counter = 0;
	if(g_TIMFlag.bits.EncoderFlag == 1)
	{
		g_TIMFlag.bits.EncoderFlag = 0;
		n_Counter =(u16) TIM_GetCounter(TIM1);
		//printf("n_Count:%d\r\n",n_Counter);
	}
}
