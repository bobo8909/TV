#include "timer.h"





//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

//#define SPEED_OUT_GPIO GPIOE
//#define SPEED_OUT_GPIO_PIN GPIO_Pin_2
#define SPEED_IN_GPIO GPIOE

#define SPEED_IN_GPIO_PIN GPIO_Pin_1

#define READ_SPEED_IO GPIO_ReadInputDataBit(SPEED_IN_GPIO, SPEED_IN_GPIO_PIN)
//#define SET_SPEED_IO_HIGH GPIO_SetBits(SPEED_OUT_GPIO, SPEED_OUT_GPIO_PIN)
//#define SET_SPEED_IO_LOW GPIO_ResetBits(SPEED_OUT_GPIO, SPEED_OUT_GPIO_PIN)


/*global value*/
STRUCT_TIMFLAG g_TIMFlag;

 u8 TIM3CH2_CAPTURE_STA = 0;	//	通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM3CH2_CAPTURE_UPVAL;
static u16 TIM3CH2_CAPTURE_DOWNVAL;
static u32 tim3_T2;
static u16 tem3pup2;
int PWM3OUT2; 				//输出占空比

 u8 TIM3CH4_CAPTURE_STA = 0;	//	通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM3CH4_CAPTURE_UPVAL;
static u16 TIM3CH4_CAPTURE_DOWNVAL;
static u32 tim3_T4;
static u16 tem3pup4;
int PWM3OUT4; 				//输出占空比

static u8 TIM4CH2_CAPTURE_STA = 0;	//	通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM4CH2_CAPTURE_UPVAL;
static u16 TIM4CH2_CAPTURE_DOWNVAL;
static u32 tim4_T2;
static u16 tem4pup2;
int PWM4OUT2; 				//输出占空比

#if 0 
static u8 TIM5CH2_CAPTURE_STA = 0;	//	通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM5CH2_CAPTURE_UPVAL;
static u16 TIM5CH2_CAPTURE_DOWNVAL;
static u32 tim5_T2;
static u16 tem5pup2;
int PWM5OUT2; 				//输出占空比

static u8 TIM5CH4_CAPTURE_STA = 0;	//	通道4输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM5CH4_CAPTURE_UPVAL;
static u16 TIM5CH4_CAPTURE_DOWNVAL;
static u32 tim5_T4;
static u16 tem5pup4;
int PWM5OUT4; 				//输出占空比
#endif
#if 0
static u8 TIM2CH2_CAPTURE_STA = 0;	//	通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM2CH2_CAPTURE_UPVAL;
static u16 TIM2CH2_CAPTURE_DOWNVAL;
static u32 tim2_T2;
static u16 tem2pup2;
int PWM2OUT2; 				//输出占空比
#endif

#if 1
static u8 TIM2CH4_CAPTURE_STA = 0;	//	通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志位	
static u16 TIM2CH4_CAPTURE_UPVAL;
static u16 TIM2CH4_CAPTURE_DOWNVAL;
static u32 tim2_T4;
static u16 tem2pup4;
int PWM2OUT4; 				//输出占空比
#endif

/**
  * @brief  初始化TIM2定时器，通过此定时器来读取速度传感器传来的脉冲信号，并且此定时器也当CAN发送数据的定时器
  *        
  * @param  u16 arr：自动重装载值
  * @param  U16 psc:预分频系数
  *
  * @retval None
  */

static void TIM6_Init(u16 arr,u16 psc)
{  
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
		
		//定时器2的初始化
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	 
		TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断，允许更新中断
	
		//中断优先级NVIC设置
		NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//从优先级2级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	
		TIM_Cmd(TIM6, ENABLE);	//使能TIMx					 

}

//定时器2中断服务程序
static u8 SpeedTotalCount = 0;
//static u8 AngSensorATotalCount = 0;
//static u8 AngSensorBTotalCount = 0;
u16 AngSensorAMinus = 0;
u16 AngSensorBMinus = 0;
//static u8 AFlag =0;
//static u8 BFlag =0;
//static u8 TIM6PluseCount = 0;
//u8 g_TIM2PluseCount = 0;
//u8 g_CANSpeedFlag = 0;

//static u8 TIM6PluseFlag = 0;

static u16 LEDCount = 0;
static u8 CANSendCount = 0;
static u8 DACSendCount = 0;
void TIM6_IRQHandler(void)	 //TIM2中断
{

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx更新中断标志

		/*Speed count*/
		if(g_StructExtiFlag.bits.SpeedStartFlag == 1)
		{
			if(g_TIMFlag.bits.SpeedFlag == 0)
			{
				SpeedTotalCount++;

				if(SpeedTotalCount == 99)
				{
					SpeedTotalCount = 0;
					g_TIMFlag.bits.SpeedFlag = 1;
					g_StructExtiFlag.bits.SpeedStartFlag = 0;
				}
			}
		}
#if 0
		/*Angle Sensor A count*/
		if(g_StructExtiFlag.bits.ExtiAngSensorAStartFlag == 1)
		{
			if(g_TIMFlag.bits.TIMAngSensorAFlag == 0)
			{
				AngSensorATotalCount++;
				if(AngSensorATotalCount == 99)
				{
					AngSensorATotalCount = 0;
					g_TIMFlag.bits.TIMAngSensorAFlag = 1;
					g_StructExtiFlag.bits.ExtiAngSensorAStartFlag = 0;
				}
			}
		}
		else
		{
				if(AngSensorAMinus != 0)
				{
					AngSensorAMinus--;
				}
				else
				{
					g_StructExtiFlag.bits.ExtiAngSensorAFlag = 0;
				}
		}
		/*Angle Sensor B count*/
		if(g_StructExtiFlag.bits.ExtiAngSensorBStartFlag == 1)
		{
			if(g_TIMFlag.bits.TIMAngSensorBFlag == 0)
			{
				AngSensorBTotalCount++;
				if(AngSensorBTotalCount == 99)
				{
					AngSensorBTotalCount = 0;
					g_TIMFlag.bits.TIMAngSensorBFlag = 1;
					g_StructExtiFlag.bits.ExtiAngSensorBStartFlag = 0;
					//g_StructExtiFlag.bits.ExtiAngSensorBFlag = 0;
				}
			}
		}
		else
		{
			if(AngSensorBMinus != 0)
			{
				AngSensorBMinus--;
			}
			else
			{
				g_StructExtiFlag.bits.ExtiAngSensorBFlag = 0;
			}
		}
#endif
		/*can count*/
		if(g_TIMFlag.bits.CANFlag == 0)
		{
			CANSendCount++;
			if(CANSendCount == 99)
			{
				g_TIMFlag.bits.CANFlag = 1;
				g_TIMFlag.bits.EncoderFlag = 1;
				CANSendCount = 0;
			}
		}
		
		/*LED count*/
		LEDCount++;
		if(LEDCount == 499)
		{
			g_TIMFlag.bits.LedFlag = 1;
		}
		else if(LEDCount == 999)
		{
			g_TIMFlag.bits.LedFlag = 0;
			LEDCount = 0;
		}

		/*ADC接收到CAN上的数据后99ms需要把数据清零一次，避免一直有DA电压输出*/
		if(g_TIMFlag.bits.DACSendFlag == 1)
		{
			DACSendCount++;
			if(DACSendCount == 98)
			{
				g_TIMFlag.bits.DACSendFlag = 0;
				DACSendCount = 0;
				g_VCU2RecvVal.Accelerator1HIGH = 0;
				g_VCU2RecvVal.Accelerator1LOW = 0;
			}
		}
	}
}
#if 0
u8 TIM6PluseFunc(void)
{
	u8 TIMPluse = 0;
	
	TIMPluse = TIM6PluseCount;
	
//	printf("TIMCount = %d\r\n",TIMPluse);
	
	TIM6PluseCount = 0;
	
	return TIMPluse;
}
#endif

#if STM32_BOARD
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
static void TIM3_Torque_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3完全重映射  TIM3_CH1->PC6 CH2->PC7 CH3->PC8 CH4->PC9                                                                      	 //用于TIM3的CH2输出的PWM通过该LED显示
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#if 1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ; //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //输入//这里设置成浮空输入，是因为在板子上有上拉电阻
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//CH1	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH2 输入捕获
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
#if 1	
	//CH3	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH4 输入捕获
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
#endif
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ITConfig(TIM3,  TIM_IT_CC2 | TIM_IT_CC4,
			ENABLE);   //不允许更新中断，允许CC2IE捕获中断
	
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
 

}

void TIM3_IRQHandler(void)   //TIM3中断
{

	if ((TIM3CH2_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);		//清除中断标志位
			if (TIM3CH2_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM3CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM3);//记录下此时的定时器计数值
				if (TIM3CH2_CAPTURE_DOWNVAL < TIM3CH2_CAPTURE_UPVAL)
				{
					tim3_T2 = ARR_2KHz;
				}
				else
					tim3_T2 = 0;
				
				tem3pup2 = TIM3CH2_CAPTURE_DOWNVAL - TIM3CH2_CAPTURE_UPVAL
						+ tim3_T2;		//得到总的高电平的时间
				PWM3OUT2 = tem3pup2;		//总的高电平的时间
				TIM3CH2_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM3CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM3);		//获取上升沿数据
                //PWM3OUT2 = 0;
				TIM3CH2_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}	
#if 1
	if ((TIM3CH4_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);		//清除中断标志位
			if (TIM3CH4_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM3CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM3);//记录下此时的定时器计数值
				if (TIM3CH4_CAPTURE_DOWNVAL < TIM3CH4_CAPTURE_UPVAL)
				{
					tim3_T4 = ARR_2KHz;
				}
				else
					tim3_T4 = 0;
				
				tem3pup4 = TIM3CH4_CAPTURE_DOWNVAL - TIM3CH4_CAPTURE_UPVAL
						+ tim3_T4;		//得到总的高电平的时间
				PWM3OUT4 = tem3pup4;		//总的高电平的时间
				TIM3CH4_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM3CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM3);		//获取上升沿数据
                //PWM3OUT4 = 0;
				TIM3CH4_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}
#endif

}


//TIM4

static void TIM4_AngleS_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM4_ICInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//CH1	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH2 输入捕获
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ITConfig(TIM4,  TIM_IT_CC2 ,
			ENABLE);   //不允许更新中断，允许CC2IE捕获中断
	
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
 

}

void TIM4_IRQHandler(void)   //TIM4中断
{
	if ((TIM4CH2_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);		//清除中断标志位
			if (TIM4CH2_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM4CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM4);//记录下此时的定时器计数值
				if (TIM4CH2_CAPTURE_DOWNVAL < TIM4CH2_CAPTURE_UPVAL)
				{
					tim4_T2 = ARR_200Hz;
				}
				else
					tim4_T2 = 0;
				
				tem4pup2 = TIM4CH2_CAPTURE_DOWNVAL - TIM4CH2_CAPTURE_UPVAL
						+ tim4_T2;		//得到总的高电平的时间
				PWM4OUT2 = tem4pup2;		//总的高电平的时间
				TIM4CH2_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM4CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM4);		//获取上升沿数据
                //PWM4OUT2 = 0;
				TIM4CH2_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}



}


/**********************************TIM5******************************/

#if 0
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
static void TIM5_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
#if 1	 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if 0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//CH1	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH2 输入捕获
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
#if 1
		//CH3	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH4 输入捕获
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM_ARRPreloadConfig(TIM5, ENABLE); //使能TIMx在ARR上的预装载寄存器
#endif
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ITConfig(TIM5,  TIM_IT_CC2 | TIM_IT_CC4,
			ENABLE);   //不允许更新中断，允许CC2IE捕获中断
	
 
	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设
 

}

void TIM5_IRQHandler(void)   //TIM3中断
{
	if ((TIM5CH2_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);		//清除中断标志位
			if (TIM5CH2_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM5CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM5);//记录下此时的定时器计数值
				if (TIM5CH2_CAPTURE_DOWNVAL < TIM5CH2_CAPTURE_UPVAL)
				{
					tim5_T2 = ARR_2KHz;
				}
				else
					tim5_T2 = 0;
				
				tem5pup2 = TIM5CH2_CAPTURE_DOWNVAL - TIM5CH2_CAPTURE_UPVAL
						+ tim5_T2;		//得到总的高电平的时间
				PWM5OUT2 = tem5pup2;		//总的高电平的时间
				TIM5CH2_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM5CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM5);		//获取上升沿数据
                PWM5OUT2 = 0;
				TIM5CH2_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}
#if 1
	if ((TIM5CH4_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);		//清除中断标志位
			if (TIM5CH4_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM5CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM5);//记录下此时的定时器计数值
				if (TIM5CH4_CAPTURE_DOWNVAL < TIM5CH4_CAPTURE_UPVAL)
				{
					tim5_T4 = ARR_2KHz;
				}
				else
					tim5_T4 = 0;
				
				tem5pup4 = TIM5CH4_CAPTURE_DOWNVAL - TIM5CH4_CAPTURE_UPVAL
						+ tim5_T4;		//得到总的高电平的时间
				PWM5OUT4 = tem5pup4;		//总的高电平的时间
				TIM5CH4_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC4PolarityConfig(TIM5, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM5CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM5);		//获取上升沿数据
                PWM5OUT4 = 0;
				TIM5CH4_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC4PolarityConfig(TIM5, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}
#endif
}

#endif
//TIM6
#if 1
static void TIM2_AngleP_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
#if 0
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
   GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //Timer3完全重映射  TIM3_CH1->PC6 CH2->PC7 CH3->PC8 CH4->PC9																			//用于TIM3的CH2输出的PWM通过该LED显示
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ; //TIM_CH2
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉    
   
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; //TIM_CH2
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //输入
   GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif   
	
 #if 1
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
#if 0
	//CH1	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH2 输入捕获
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
#endif
	
	//CH3	PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	//CH4 输入捕获
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频，不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ITConfig(TIM2, /*TIM_IT_CC2 | */TIM_IT_CC4 ,
			ENABLE);   //不允许更新中断，允许CC2IE捕获中断
	
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
 

}
void TIM2_IRQHandler(void)   //TIM4中断
{
#if 0
	if ((TIM2CH2_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);		//清除中断标志位
			if (TIM2CH2_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM2CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM2);//记录下此时的定时器计数值
				if (TIM2CH2_CAPTURE_DOWNVAL < TIM2CH2_CAPTURE_UPVAL)
				{
					tim2_T2 = ARR_1KHz;
				}
				else
					tim2_T2 = 0;
				
				tem2pup2 = TIM2CH2_CAPTURE_DOWNVAL - TIM2CH2_CAPTURE_UPVAL
						+ tim2_T2;		//得到总的高电平的时间
				PWM2OUT2 = tem2pup2;		//总的高电平的时间
				TIM2CH2_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM2CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM2);		//获取上升沿数据
                //PWM2OUT2 = 0;
				TIM2CH2_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}
	#endif
if ((TIM2CH4_CAPTURE_STA & 0X80) == 0)		//还未成功捕获
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) 	//捕获2发生捕获事件
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);		//清除中断标志位
		if (TIM2CH4_CAPTURE_STA & 0X40) 	//捕获到一个下降沿
		{
			TIM2CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM2);//记录下此时的定时器计数值
			if (TIM2CH4_CAPTURE_DOWNVAL < TIM2CH4_CAPTURE_UPVAL)
			{
				tim2_T4 = ARR_2KHz;
			}
			else
				tim2_T4 = 0;
			
			tem2pup4 = TIM2CH4_CAPTURE_DOWNVAL - TIM2CH4_CAPTURE_UPVAL
					+ tim2_T4;		//得到总的高电平的时间
			PWM2OUT4 = tem2pup4;		//总的高电平的时间
			TIM2CH4_CAPTURE_STA = 0;		//捕获标志位清零
			TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising); //设置为上升沿捕获				  
		}
		else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
		{
			TIM2CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM2);		//获取上升沿数据
			//PWM2OUT4 = 0;
			TIM2CH4_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
			TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Falling);//设置为下降沿捕获
		}
	}
}



}
#endif
#endif

#if 0

//定时器5通道1输入捕获配置

TIM_ICInitTypeDef  TIM5_ICInitStructure;

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
   	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
   


}

u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
 
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
 
}

#endif

void TIM1_EncoderInit(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure; 
//	NVIC_InitTypeDef NVIC_InitStructure;
	/*完全重映像*/
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);                                                                     	 //用于TIM3的CH2输出的PWM通过该LED显示
	
#if 1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 0;  //选择输入比较滤波器 
	TIM_ICInit(TIM1, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3

//	TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清除TIM3的更新标志位
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);//运行更新中断

	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1, ENABLE);	
}


void TIM_INIT(void)
{

	TIM6_Init(999, 71);// 1kHz
	
	#if STM32_BOARD
	
		TIM1_EncoderInit(EncoderArr, EncoderPSC);
	
		TIM2_AngleP_Init(ARR_1KHz, PSC_1KHz);	

		TIM3_Torque_Init(ARR_2KHz, PSC_2KHz); 		//不分频。PWM频率=72 000 000/(35+1)(9999+1)=200hz

		//	TIM3_Torque_Init(4999,71);  //不分频。PWM频率=72000000/72=1MHz
		//	TIM4_Init(0XFFFF,72-1); //以1Mhz的频率计数 
		//	TIM_SetCompare1(TIM3,10000);		   
		 //   TIM_SetCompare2(TIM3,0x8000); 	   
		//	TIM_SetCompare3(TIM3,0xA000);		   
		//	TIM_SetCompare4(TIM3,0xF000);		   
		TIM4_AngleS_Init(ARR_200Hz, PSC_200Hz); 	 //不分频。PWM频率=72 000 000/(0+1)(35999+1)=2Khz
			//TIM_SetCompare1(TIM4,20000);		
		//TIM5_PWM_Init(ARR_2KHz, PSC_2KHz); 	 //不分频。PWM频率=72 000 000/(1+1)(35999+1)=1Khz
			//TIM_SetCompare1(TIM5,5000); 		
			//TIM_SetCompare3(TIM5,30000);
	#endif
}
