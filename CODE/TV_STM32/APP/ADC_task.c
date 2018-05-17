#include "ADC_task.h"

/*采集油门信号*/
/*使用STM32内部的ADC模块进行采集信号*/
void ADC_task(void)

{
	u16 ADCbuf = 0;
//	u16 printfad = 0;

	/*AD采样的是12bit，需要转换成14bit*/
	ADCbuf = Get_Adc_Average(ADC_Channel_10,10) << 2;
//	printfad = Get_Adc_Average(ADC_Channel_10,10) ;

//	printf("ADCbuf = %d\r\n",printfad);

	g_BCM1SendVal.Accelerator1SignalHigh = ADCbuf >> 8;
	g_BCM1SendVal.Accelerator1SignalLow = ADCbuf & 0x00ff;	
}

