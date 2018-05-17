#include "Encoder_task.h"

#define DEGREEMAX 360
#define MAXPPR	5000

/*���ӽǶȴ���������ʹ��STM32 TIM1�ı�����ģʽ�����м���*/
void Encoder_task(void)
{
    static volatile u16 n_Counter = 0;
	u16 RotationalSpeed = 0; //ת���ٶ�
    static u16 backupCounter = 0;

	
	if(g_TIMFlag.bits.EncoderFlag == 1)
	{
		g_TIMFlag.bits.EncoderFlag = 0;
		n_Counter =(u16) TIM_GetCounter(TIM1);

		/*��ʱ�����õ��������أ��½��ض�����������ʵ�ʵ���������Ҫ����4,
		**������100ms����һ�Σ�����ʱ��Ҫ����10���1����м���
		*/
		RotationalSpeed = DEGREEMAX * 10 * (n_Counter - backupCounter) / 4 / MAXPPR;


		g_BCM3SendVal.FrontSpeedHigh = (RotationalSpeed >> 8) & 0xFF;
		g_BCM3SendVal.FrontSpeedLow = (u8)(RotationalSpeed & 0xFF);
		
		
		//printf("n_Count:%d\r\n",n_Counter - backupCounter);
        
        backupCounter = n_Counter;

	}
}
