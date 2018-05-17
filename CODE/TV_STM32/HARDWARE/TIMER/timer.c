#include "timer.h"





//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

//#define SPEED_OUT_GPIO GPIOE
//#define SPEED_OUT_GPIO_PIN GPIO_Pin_2
#define SPEED_IN_GPIO GPIOE

#define SPEED_IN_GPIO_PIN GPIO_Pin_1

#define READ_SPEED_IO GPIO_ReadInputDataBit(SPEED_IN_GPIO, SPEED_IN_GPIO_PIN)
//#define SET_SPEED_IO_HIGH GPIO_SetBits(SPEED_OUT_GPIO, SPEED_OUT_GPIO_PIN)
//#define SET_SPEED_IO_LOW GPIO_ResetBits(SPEED_OUT_GPIO, SPEED_OUT_GPIO_PIN)


/*global value*/
STRUCT_TIMFLAG g_TIMFlag;

 u8 TIM3CH2_CAPTURE_STA = 0;	//	ͨ��2���벶���־������λ�������־����6λ�������־λ	
static u16 TIM3CH2_CAPTURE_UPVAL;
static u16 TIM3CH2_CAPTURE_DOWNVAL;
static u32 tim3_T2;
static u16 tem3pup2;
int PWM3OUT2; 				//���ռ�ձ�

 u8 TIM3CH4_CAPTURE_STA = 0;	//	ͨ��2���벶���־������λ�������־����6λ�������־λ	
static u16 TIM3CH4_CAPTURE_UPVAL;
static u16 TIM3CH4_CAPTURE_DOWNVAL;
static u32 tim3_T4;
static u16 tem3pup4;
int PWM3OUT4; 				//���ռ�ձ�

static u8 TIM4CH2_CAPTURE_STA = 0;	//	ͨ��2���벶���־������λ�������־����6λ�������־λ	
static u16 TIM4CH2_CAPTURE_UPVAL;
static u16 TIM4CH2_CAPTURE_DOWNVAL;
static u32 tim4_T2;
static u16 tem4pup2;
int PWM4OUT2; 				//���ռ�ձ�

#if 0 
static u8 TIM5CH2_CAPTURE_STA = 0;	//	ͨ��2���벶���־������λ�������־����6λ�������־λ	
static u16 TIM5CH2_CAPTURE_UPVAL;
static u16 TIM5CH2_CAPTURE_DOWNVAL;
static u32 tim5_T2;
static u16 tem5pup2;
int PWM5OUT2; 				//���ռ�ձ�

static u8 TIM5CH4_CAPTURE_STA = 0;	//	ͨ��4���벶���־������λ�������־����6λ�������־λ	
static u16 TIM5CH4_CAPTURE_UPVAL;
static u16 TIM5CH4_CAPTURE_DOWNVAL;
static u32 tim5_T4;
static u16 tem5pup4;
int PWM5OUT4; 				//���ռ�ձ�
#endif
#if 0
static u8 TIM2CH2_CAPTURE_STA = 0;	//	ͨ��2���벶���־������λ�������־����6λ�������־λ	
static u16 TIM2CH2_CAPTURE_UPVAL;
static u16 TIM2CH2_CAPTURE_DOWNVAL;
static u32 tim2_T2;
static u16 tem2pup2;
int PWM2OUT2; 				//���ռ�ձ�
#endif

#if 1
static u8 TIM2CH4_CAPTURE_STA = 0;	//	ͨ��2���벶���־������λ�������־����6λ�������־λ	
static u16 TIM2CH4_CAPTURE_UPVAL;
static u16 TIM2CH4_CAPTURE_DOWNVAL;
static u32 tim2_T4;
static u16 tem2pup4;
int PWM2OUT4; 				//���ռ�ձ�
#endif

/**
  * @brief  ��ʼ��TIM2��ʱ����ͨ���˶�ʱ������ȡ�ٶȴ����������������źţ����Ҵ˶�ʱ��Ҳ��CAN�������ݵĶ�ʱ��
  *        
  * @param  u16 arr���Զ���װ��ֵ
  * @param  U16 psc:Ԥ��Ƶϵ��
  *
  * @retval None
  */

static void TIM6_Init(u16 arr,u16 psc)
{  
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
		
		//��ʱ��2�ĳ�ʼ��
		TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
		TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�жϣ���������ж�
	
		//�ж����ȼ�NVIC����
		NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ�2��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	
		TIM_Cmd(TIM6, ENABLE);	//ʹ��TIMx					 

}

//��ʱ��2�жϷ������
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
void TIM6_IRQHandler(void)	 //TIM2�ж�
{

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־

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

		/*ADC���յ�CAN�ϵ����ݺ�99ms��Ҫ����������һ�Σ�����һֱ��DA��ѹ���*/
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
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
static void TIM3_Torque_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3��ȫ��ӳ��  TIM3_CH1->PC6 CH2->PC7 CH3->PC8 CH4->PC9                                                                      	 //����TIM3��CH2�����PWMͨ����LED��ʾ
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#if 1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ; //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //����//�������óɸ������룬����Ϊ�ڰ���������������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//CH1	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH2 ���벶��
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
#if 1	
	//CH3	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH4 ���벶��
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
#endif
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM3,  TIM_IT_CC2 | TIM_IT_CC4,
			ENABLE);   //����������жϣ�����CC2IE�����ж�
	
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 

}

void TIM3_IRQHandler(void)   //TIM3�ж�
{

	if ((TIM3CH2_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);		//����жϱ�־λ
			if (TIM3CH2_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM3CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH2_CAPTURE_DOWNVAL < TIM3CH2_CAPTURE_UPVAL)
				{
					tim3_T2 = ARR_2KHz;
				}
				else
					tim3_T2 = 0;
				
				tem3pup2 = TIM3CH2_CAPTURE_DOWNVAL - TIM3CH2_CAPTURE_UPVAL
						+ tim3_T2;		//�õ��ܵĸߵ�ƽ��ʱ��
				PWM3OUT2 = tem3pup2;		//�ܵĸߵ�ƽ��ʱ��
				TIM3CH2_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM3CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM3);		//��ȡ����������
                //PWM3OUT2 = 0;
				TIM3CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}	
#if 1
	if ((TIM3CH4_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);		//����жϱ�־λ
			if (TIM3CH4_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM3CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH4_CAPTURE_DOWNVAL < TIM3CH4_CAPTURE_UPVAL)
				{
					tim3_T4 = ARR_2KHz;
				}
				else
					tim3_T4 = 0;
				
				tem3pup4 = TIM3CH4_CAPTURE_DOWNVAL - TIM3CH4_CAPTURE_UPVAL
						+ tim3_T4;		//�õ��ܵĸߵ�ƽ��ʱ��
				PWM3OUT4 = tem3pup4;		//�ܵĸߵ�ƽ��ʱ��
				TIM3CH4_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM3CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM3);		//��ȡ����������
                //PWM3OUT4 = 0;
				TIM3CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
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
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//CH1	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH2 ���벶��
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM4,  TIM_IT_CC2 ,
			ENABLE);   //����������жϣ�����CC2IE�����ж�
	
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
 

}

void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if ((TIM4CH2_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);		//����жϱ�־λ
			if (TIM4CH2_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM4CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM4);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM4CH2_CAPTURE_DOWNVAL < TIM4CH2_CAPTURE_UPVAL)
				{
					tim4_T2 = ARR_200Hz;
				}
				else
					tim4_T2 = 0;
				
				tem4pup2 = TIM4CH2_CAPTURE_DOWNVAL - TIM4CH2_CAPTURE_UPVAL
						+ tim4_T2;		//�õ��ܵĸߵ�ƽ��ʱ��
				PWM4OUT2 = tem4pup2;		//�ܵĸߵ�ƽ��ʱ��
				TIM4CH2_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM4CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM4);		//��ȡ����������
                //PWM4OUT2 = 0;
				TIM4CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}



}


/**********************************TIM5******************************/

#if 0
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
static void TIM5_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
#if 1	 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if 0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//CH1	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH2 ���벶��
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
#if 1
		//CH3	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH4 ���벶��
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
#endif
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM5,  TIM_IT_CC2 | TIM_IT_CC4,
			ENABLE);   //����������жϣ�����CC2IE�����ж�
	
 
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����
 

}

void TIM5_IRQHandler(void)   //TIM3�ж�
{
	if ((TIM5CH2_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);		//����жϱ�־λ
			if (TIM5CH2_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM5CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM5);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM5CH2_CAPTURE_DOWNVAL < TIM5CH2_CAPTURE_UPVAL)
				{
					tim5_T2 = ARR_2KHz;
				}
				else
					tim5_T2 = 0;
				
				tem5pup2 = TIM5CH2_CAPTURE_DOWNVAL - TIM5CH2_CAPTURE_UPVAL
						+ tim5_T2;		//�õ��ܵĸߵ�ƽ��ʱ��
				PWM5OUT2 = tem5pup2;		//�ܵĸߵ�ƽ��ʱ��
				TIM5CH2_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM5CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM5);		//��ȡ����������
                PWM5OUT2 = 0;
				TIM5CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}
#if 1
	if ((TIM5CH4_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);		//����жϱ�־λ
			if (TIM5CH4_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM5CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM5);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM5CH4_CAPTURE_DOWNVAL < TIM5CH4_CAPTURE_UPVAL)
				{
					tim5_T4 = ARR_2KHz;
				}
				else
					tim5_T4 = 0;
				
				tem5pup4 = TIM5CH4_CAPTURE_DOWNVAL - TIM5CH4_CAPTURE_UPVAL
						+ tim5_T4;		//�õ��ܵĸߵ�ƽ��ʱ��
				PWM5OUT4 = tem5pup4;		//�ܵĸߵ�ƽ��ʱ��
				TIM5CH4_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC4PolarityConfig(TIM5, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM5CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM5);		//��ȡ����������
                PWM5OUT4 = 0;
				TIM5CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC4PolarityConfig(TIM5, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
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
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
   GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //Timer3��ȫ��ӳ��  TIM3_CH1->PC6 CH2->PC7 CH3->PC8 CH4->PC9																			//����TIM3��CH2�����PWMͨ����LED��ʾ
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ; //TIM_CH2
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����    
   
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; //TIM_CH2
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //����
   GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif   
	
 #if 1
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
#if 0
	//CH1	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH2 ���벶��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
#endif
	
	//CH3	PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	//CH4 ���벶��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ������Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM2, /*TIM_IT_CC2 | */TIM_IT_CC4 ,
			ENABLE);   //����������жϣ�����CC2IE�����ж�
	
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
 

}
void TIM2_IRQHandler(void)   //TIM4�ж�
{
#if 0
	if ((TIM2CH2_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);		//����жϱ�־λ
			if (TIM2CH2_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM2CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM2);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM2CH2_CAPTURE_DOWNVAL < TIM2CH2_CAPTURE_UPVAL)
				{
					tim2_T2 = ARR_1KHz;
				}
				else
					tim2_T2 = 0;
				
				tem2pup2 = TIM2CH2_CAPTURE_DOWNVAL - TIM2CH2_CAPTURE_UPVAL
						+ tim2_T2;		//�õ��ܵĸߵ�ƽ��ʱ��
				PWM2OUT2 = tem2pup2;		//�ܵĸߵ�ƽ��ʱ��
				TIM2CH2_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM2CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM2);		//��ȡ����������
                //PWM2OUT2 = 0;
				TIM2CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}
	#endif
if ((TIM2CH4_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) 	//����2���������¼�
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);		//����жϱ�־λ
		if (TIM2CH4_CAPTURE_STA & 0X40) 	//����һ���½���
		{
			TIM2CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM2);//��¼�´�ʱ�Ķ�ʱ������ֵ
			if (TIM2CH4_CAPTURE_DOWNVAL < TIM2CH4_CAPTURE_UPVAL)
			{
				tim2_T4 = ARR_2KHz;
			}
			else
				tim2_T4 = 0;
			
			tem2pup4 = TIM2CH4_CAPTURE_DOWNVAL - TIM2CH4_CAPTURE_UPVAL
					+ tim2_T4;		//�õ��ܵĸߵ�ƽ��ʱ��
			PWM2OUT4 = tem2pup4;		//�ܵĸߵ�ƽ��ʱ��
			TIM2CH4_CAPTURE_STA = 0;		//�����־λ����
			TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising); //����Ϊ�����ز���				  
		}
		else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
		{
			TIM2CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM2);		//��ȡ����������
			//PWM2OUT4 = 0;
			TIM2CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
			TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
		}
	}
}



}
#endif
#endif

#if 0

//��ʱ��5ͨ��1���벶������

TIM_ICInitTypeDef  TIM5_ICInitStructure;

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
   	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
   


}

u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
 
}

#endif

void TIM1_EncoderInit(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure; 
//	NVIC_InitTypeDef NVIC_InitStructure;
	/*��ȫ��ӳ��*/
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);                                                                     	 //����TIM3��CH2�����PWMͨ����LED��ʾ
	
#if 1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //����
	GPIO_Init(GPIOE, &GPIO_InitStructure);
#endif
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 0;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM1, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3

//	TIM_ARRPreloadConfig(TIM4, ENABLE);//ʹ��Ԥװ��
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);//���TIM3�ĸ��±�־λ
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);//���и����ж�

	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1, ENABLE);	
}


void TIM_INIT(void)
{

	TIM6_Init(999, 71);// 1kHz
	
	#if STM32_BOARD
	
		TIM1_EncoderInit(EncoderArr, EncoderPSC);
	
		TIM2_AngleP_Init(ARR_1KHz, PSC_1KHz);	

		TIM3_Torque_Init(ARR_2KHz, PSC_2KHz); 		//����Ƶ��PWMƵ��=72 000 000/(35+1)(9999+1)=200hz

		//	TIM3_Torque_Init(4999,71);  //����Ƶ��PWMƵ��=72000000/72=1MHz
		//	TIM4_Init(0XFFFF,72-1); //��1Mhz��Ƶ�ʼ��� 
		//	TIM_SetCompare1(TIM3,10000);		   
		 //   TIM_SetCompare2(TIM3,0x8000); 	   
		//	TIM_SetCompare3(TIM3,0xA000);		   
		//	TIM_SetCompare4(TIM3,0xF000);		   
		TIM4_AngleS_Init(ARR_200Hz, PSC_200Hz); 	 //����Ƶ��PWMƵ��=72 000 000/(0+1)(35999+1)=2Khz
			//TIM_SetCompare1(TIM4,20000);		
		//TIM5_PWM_Init(ARR_2KHz, PSC_2KHz); 	 //����Ƶ��PWMƵ��=72 000 000/(1+1)(35999+1)=1Khz
			//TIM_SetCompare1(TIM5,5000); 		
			//TIM_SetCompare3(TIM5,30000);
	#endif
}
