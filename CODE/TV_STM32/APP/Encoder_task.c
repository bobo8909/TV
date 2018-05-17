#include "Encoder_task.h"

#define DEGREEMAX 360
#define MAXPPR	5000

/*附加角度传感器任务，使用STM32 TIM1的编码器模式来进行计算*/
void Encoder_task(void)
{
    static volatile u16 n_Counter = 0;
	u16 RotationalSpeed = 0; //转动速度
    static u16 backupCounter = 0;

	
	if(g_TIMFlag.bits.EncoderFlag == 1)
	{
		g_TIMFlag.bits.EncoderFlag = 0;
		n_Counter =(u16) TIM_GetCounter(TIM1);

		/*定时器设置的是上升沿，下降沿都会计数，因此实际的脉冲数需要除以4,
		**由于是100ms发送一次，计算时需要乘以10变成1秒进行计算
		*/
		RotationalSpeed = DEGREEMAX * 10 * (n_Counter - backupCounter) / 4 / MAXPPR;


		g_BCM3SendVal.FrontSpeedHigh = (RotationalSpeed >> 8) & 0xFF;
		g_BCM3SendVal.FrontSpeedLow = (u8)(RotationalSpeed & 0xFF);
		
		
		//printf("n_Count:%d\r\n",n_Counter - backupCounter);
        
        backupCounter = n_Counter;

	}
}
