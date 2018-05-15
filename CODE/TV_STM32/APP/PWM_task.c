#include "PWM_task.h"


/******************************************************
*四路PWM的频率：
*	 转角S：TIM4(200Hz)
*	 转角P: TIM2(1KHz)
*    扭矩1：TIM3(2KHz)
*	 扭矩2: TIM3(2KHz)
******************************************************/
#define TIM_SEND_RESOLUTION 10000
/**
  * @brief  处理四路PWM输入输出的任务，把从IO口读取的波形PWM占空比按照相同的占空比从相应的IO口输出
  *         
  * @param  None
  *   
  * @retval None
  */
void PWM_task(void)
{
	static u32 PWMVal[4] = {0};
	
	static int BackupPWM3OUT2 = 0;
	static int BackupPWM4OUT2 = 0;
	static int BackupPWM2OUT4 = 0;
	static int BackupPWM3OUT4 = 0;

	u32 SendVal = 0;
	
/*
*已根据实际的频率进行了修改，把读取的占空比按照CAN协议发送出去
*/
	if(g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
	{
        if(BackupPWM2OUT4 != PWM2OUT4) 
		{
			BackupPWM2OUT4 = PWM2OUT4;	
			PWM2OUT4 = 0;
			TIM_SetCompare3(TIM2, BackupPWM2OUT4);
			printf("BackupPWM2OUT4 = %d\r\n", BackupPWM2OUT4);
			SendVal = BackupPWM2OUT4 * ARR_1KHz / TIM_SEND_RESOLUTION ;
			g_BCM2SendVal.AngleSensorSignalPHigh = SendVal >> 8;
			g_BCM2SendVal.AngleSensorSignalPLow = (u8)(SendVal & 0xFF);
		}

		if(BackupPWM3OUT2 != PWM3OUT2) 
		{
			BackupPWM3OUT2 = PWM3OUT2;		
			PWM3OUT2 = 0;
			TIM_SetCompare1(TIM3, BackupPWM3OUT2);
			printf("BackupPWM3OUT2 = %d\r\n", BackupPWM3OUT2);
			
			SendVal = BackupPWM3OUT2 * ARR_2KHz / TIM_SEND_RESOLUTION ;
			g_BCM2SendVal.EPSMomentalSignal1High = SendVal >> 8;
			g_BCM2SendVal.EPSMomentalSignal1Low = (u8)(SendVal & 0xFF);
		}

		if(BackupPWM3OUT4 != PWM3OUT4)
		{
			BackupPWM3OUT4 = PWM3OUT4;		
			PWM3OUT4 = 0;
			TIM_SetCompare3(TIM3, BackupPWM3OUT4);	
			printf("BackupPWM3OUT4 = %d\r\n", BackupPWM3OUT4);
			
			SendVal = BackupPWM3OUT4 * ARR_2KHz / TIM_SEND_RESOLUTION;
			g_BCM2SendVal.EPSMomentalSignal2High = SendVal >> 8;
			g_BCM2SendVal.EPSMomentalSignal2Low = (u8)(SendVal & 0xFF);
		}

		if(BackupPWM4OUT2 != PWM4OUT2)
		{
			BackupPWM4OUT2 = PWM4OUT2;		
			PWM4OUT2 = 0;
			TIM_SetCompare1(TIM4, BackupPWM4OUT2);	
			printf("BackupPWM4OUT2 = %d\r\n", BackupPWM4OUT2);
			
			SendVal = BackupPWM4OUT2 * ARR_200Hz / TIM_SEND_RESOLUTION;
			g_BCM2SendVal.AngleSensorSignalSHigh = SendVal >> 8;
			g_BCM2SendVal.AngleSensorSignalSLow = (u8)(SendVal & 0xFF);
		}
	}
	else if(g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == AUTOMATIC_DRIVING)
	{
		//目前只有转角的两路需要输出20180503 
		//这里还需要根据实际情况进行修改
		PWMVal[0] = ((u32)g_VCU3RecvVal.AngleSensorSigSHigh << 8 | g_VCU3RecvVal.AngleSensorSigSLow) * ARR_200Hz / TIM_SEND_RESOLUTION;
		PWMVal[1] = ((u32)g_VCU3RecvVal.AngleSensorSigPHigh << 8 | g_VCU3RecvVal.AngleSensorSigPLow) * ARR_1KHz / TIM_SEND_RESOLUTION  ;
		PWMVal[2] = ((u32)g_VCU3RecvVal.EPSMomentalSig1HIGH << 8 | g_VCU3RecvVal.EPSMomentalSig1LOW) * ARR_2KHz / TIM_SEND_RESOLUTION;
		PWMVal[3] = ((u32)g_VCU3RecvVal.EPSMomentalSig2HIGH << 8 | g_VCU3RecvVal.EPSMomentalSig2LOW) * ARR_2KHz / TIM_SEND_RESOLUTION;
		
		TIM_SetCompare1(TIM4, PWMVal[0]);		   
		TIM_SetCompare3(TIM2, PWMVal[1]);		   
		TIM_SetCompare1(TIM3, PWMVal[2]);		   
		TIM_SetCompare3(TIM3, PWMVal[3]);	
		
		printf("PWMVal0 = %d\r\n", PWMVal[0]);
		printf("PWMVal1 = %d\r\n", PWMVal[1]);
		printf("PWMVal2 = %d\r\n", PWMVal[2]);
		printf("PWMVal3 = %d\r\n", PWMVal[3]);
	}
}

