#include "ADC_task.h"

/*�ɼ������ź�*/
/*ʹ��STM32�ڲ���ADCģ����вɼ��ź�*/
void ADC_task(void)

{
	u16 ADCbuf = 0;
//	u16 printfad = 0;

	/*AD��������12bit����Ҫת����14bit*/
	ADCbuf = Get_Adc_Average(ADC_Channel_10,10) << 2;
//	printfad = Get_Adc_Average(ADC_Channel_10,10) ;

//	printf("ADCbuf = %d\r\n",printfad);

	g_BCM1SendVal.Accelerator1SignalHigh = ADCbuf >> 8;
	g_BCM1SendVal.Accelerator1SignalLow = ADCbuf & 0x00ff;	
}

