#include "DAC_task.h"


void AcceleratorControl(u16 AcceleratorVal)
{
	DAC_SetChannel1Data(DAC_Align_12b_R,AcceleratorVal);
}

/*��������ź�*/
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
		
	#elif STM32_BOARD /*ʹ��STM32�ڲ���DACģ��������*/
	
		u16 DACbuf[2] = {0};
        u16 DACValue = 0;
		if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING )
		{
			/*��Ҫ���յ���14BIT������ת����12BIT�����ݣ�Ȼ���ٽ��в���*/
			DACbuf[0] = (((u16)g_VCU2RecvVal.Accelerator1HIGH << 8) | g_VCU2RecvVal.Accelerator1LOW) ;

		}
		else
		{
			DACbuf[0] = (((u16)g_BCM1SendVal.Accelerator1SignalHigh << 8) | g_BCM1SendVal.Accelerator1SignalLow) ;
		}
		//printf("DACBUF:%d\r\n",DACbuf[0]);
		DACValue = (u32)DACbuf[0] * 5 * 4095 / 16383 / 6.6;
		AcceleratorControl(DACValue);	
		//AcceleratorControl(4095);
		//Dac1_Set_Vol(3200);
	#endif	
}
