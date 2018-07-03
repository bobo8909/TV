#include "driver_IO.h"

void IO_init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;	
 #if STM32_EVALUATION
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;	    		 // ???��????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ2MHz
	GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
	
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD 0,1,2,3,4,5,6,7

	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
 #else
    /*GPIO�����ʼ��*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //���������IO���ٶ�Ϊ2MHz
    GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                   //���������IO���ٶ�Ϊ2MHz
    GPIO_ResetBits(GPIOE,GPIO_Pin_14|GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);                   //���������IO���ٶ�Ϊ2MHz
    GPIO_ResetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);                   //���������IO���ٶ�Ϊ2MHz
    GPIO_ResetBits(GPIOG,GPIO_Pin_5|GPIO_Pin_6);

    /*GPIO�����ʼ��*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE 2,3,4,5,6,

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3| GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOF 0,1,2,3,4

 #endif
		
}

/**
  * @brief  �ٶ��������ص�GPIO��ʼ��
  *        
  * @param  None
  *   
  * @retval None
  */
void SpeedGPIOInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  ���ӽǶȴ������������ص�GPIO��ʼ��
  *        
  * @param  None
  *   
  * @retval None
  */
void AngleSensorGPIOInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

