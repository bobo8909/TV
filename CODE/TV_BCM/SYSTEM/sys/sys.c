#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}




CanRxMsg CANSendBuf[3] = {0,0,0,0,0,{0},0};

STRUCT_GLOBAL_FLAG g_StructGlobalFlag = {0};
STRUCT_EXTI_FLAG g_StructExtiFlag = {0};
STRUCT_BCM_STATUS g_StructBCMStatus = {0};

static void System_clock(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 
						 | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7 |RCC_APB1Periph_DAC, ENABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
						 | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOF
						 |RCC_APB2Periph_GPIOG|RCC_APB2Periph_SPI1  | RCC_APB2Periph_USART1 | RCC_APB2Periph_ADC1 |RCC_APB2Periph_TIM8
						 , ENABLE);														  
}


void System_init(void)
{
	System_clock();
	
    delay_init();	    	 //延时函数初始化	  
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    
    uart_init(115200);	 //串口初始化为115200
    
 	CanModeInit(CAN_SJW_1tq,CAN_BS2_2tq, CAN_BS1_3tq, 24, CAN_Mode_Normal);//CAN普通模式初始化, 波特率250Kbps 
 	
    IWDG_Init(4,1250);                                                   //与分频数为64,重载值为1250,溢出时间为2s

	TIM_INIT();
	
	LED_Init(); 			 //LED端口初始化
	
	#if STM32_BOARD
		SpeedGPIOInit();

		//AngleSensorGPIOInit();
	
		Adc_Init();
		
		DAC1_Init();
		
		SpeedEXTI_Init();
		
		IO_init();

	#endif

	#if JSDG_BOARD
		IO_init();
	#endif

	#if ZXKZ_BOARD
		AD5734R_Init();	
	#endif

	g_StructBCMStatus.DrivingMode = CheckDrivingMode();
	
	printf("system init finish\r\n");
}

