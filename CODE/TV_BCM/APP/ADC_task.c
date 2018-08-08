#include "ADC_task.h"

/*采集油门信号*/
/*使用STM32内部的ADC模块进行采集信号*/
void ADC_task(void)

{
	u16 ADCbuf = 0;
    u16 CANADbuf = 0;
	//u16 printfad = 0;

	/*AD采样的是12bit，需要转换成14bit*/
#if STM32_EVALUATION
	ADCbuf = Get_Adc_Average(ADC_Channel_10,10) << 2;
#else
    // ADCbuf = (Get_Adc_Average(ADC_Channel_12,10) * 2) << 2;
#endif
	//printfad = Get_Adc_Average(ADC_Channel_12,10) ;
    ADCbuf = Get_Adc_Average(ADC_Channel_12,10);
    CANADbuf = (u32)ADCbuf * 66 * 16383 / 4095 / 50;
	//printf("ADCbuf = %d;%d\r\n",ADCbuf,CANADbuf);

	g_BCM1SendVal.Accelerator1SignalHigh = CANADbuf >> 8;
	g_BCM1SendVal.Accelerator1SignalLow = CANADbuf & 0x00ff;	
}

