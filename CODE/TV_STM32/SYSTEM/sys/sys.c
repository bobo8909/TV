#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}




CanRxMsg CANSendBuf[3] = {0,0,0,0,0,{0},0};

STRUCT_GLOBAL_FLAG g_StructGlobalFlag = {0};
STRUCT_EXTI_FLAG g_StructExtiFlag = {0};

static void System_clock(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 
						 | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM6 |RCC_APB1Periph_DAC, ENABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
						 |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_SPI1 | RCC_APB2Periph_USART1 
						 | RCC_APB2Periph_ADC1, ENABLE);														  
}


void System_init(void)
{
	System_clock();
	
    delay_init();	    	 //��ʱ������ʼ��	  
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    
    uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
    
 	CanModeInit(CAN_SJW_1tq,CAN_BS2_8tq, CAN_BS1_9tq, 8, CAN_Mode_Normal);//CAN��ͨģʽ��ʼ��, ������500Kbps 
 	
    IWDG_Init(4,1250);                                                   //���Ƶ��Ϊ64,����ֵΪ1250,���ʱ��Ϊ2s

	TIM_INIT();
	
	LED_Init(); 			 //LED�˿ڳ�ʼ��
	
	#if STM32_BOARD
		SpeedGPIOInit();

		AngleSensorGPIOInit();
	
		Adc_Init();
		
		Dac1_Init();
		
		EXTI0_Init();

		EXTI1_Init();
		
		EXTI2_Init();

	#endif

	#if JSDG_BOARD
		IO_init();
	#endif

	#if ZXKZ_BOARD
		AD5734R_Init();	
	#endif
	
	printf("system init finish\r\n");
}

