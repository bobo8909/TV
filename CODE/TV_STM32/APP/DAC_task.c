#include "DAC_task.h"


void AcceleratorControl(u16 AcceleratorVal)
{
	DAC_SetChannel1Data(DAC_Align_12b_R,AcceleratorVal);
}

void DAC_task(void)
{
	#if ZXKZ_BOARD	
		u8 DABuf[8] = {0};

		DABuf[0] = g_VCU2RecvVal.Accelerator1LOW;
		DABuf[1] = g_VCU2RecvVal.Accelerator1HIGH;
		DABuf[2] = g_VCU2RecvVal.Accelerator2LOW;
		DABuf[3] = g_VCU2RecvVal.Accelerator2HIGH;
		DABuf[4] = 0;
		DABuf[5] = 0;
		DABuf[6] = 0;
		DABuf[7] = 0;
		
		AD5734R_Control(DABuf);
		
	#elif STM32_BOARD
	
		u16 DACbuf[2] = {0};

		if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING )
		{
			/*需要把收到的14BIT的数据转换成12BIT的数据，然后再进行操作*/
			DACbuf[0] = (((u16)g_VCU2RecvVal.Accelerator1HIGH << 8) | g_VCU2RecvVal.Accelerator1LOW) >> 2;
			//DACbuf[1] = ((u16)g_VCU2RecvVal.Accelerator2HIGH << 8) | g_VCU2RecvVal.Accelerator2LOW;
			//DAC_SetChannel1Data(DAC_Align_12b_R, DACbuf[0]);
		}
		else
		{
			DACbuf[0] = (((u16)g_BCM1SendVal.Accelerator1SignalHigh << 8) | g_BCM1SendVal.Accelerator1SignalLow) >> 2;
		}
		AcceleratorControl(DACbuf[0]);	
	#endif	
}
