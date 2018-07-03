#ifndef __DRIVER_IO_H
#define __DRIVER_IO_H

#include "sys.h"
#define STM32_EVALUATION 0

#if STM32_EVALUATION
#define KLAXON_SWITCH		PFout(0)// ���ȼ̵���
#define TURN_RIGHT_LIGHT	PFout(1)// ��ת�Ƽ�Ъ�̵���  
#define TURN_LEFT_LIGHT		PFout(2)// ��ת�ƿ��ؼ�Ъ�̵���  
#define DIPPED_HEADLIGHT   	PFout(3)// ����Ƽ̵���
#define BEAN_LIGHT      	PFout(4)// Զ��Ƽ̵���
#define STOPLIGHT     		PFout(5)// ɲ���Ƽ̵���
#define REVERSING_LIGHT    	PFout(6)// �����Ƽ̵���
#define D_GEAR   			PFout(7)// ��λD
#define R_GEAR              PFout(9)// ��λR
#define BRAKE_CONTROL		PFout(10)// �����ƶ�����
#define ACCELERATOR_ENABLE	PFout(11)//����ʹ��


#define READ_DRIVING_MODE       PDin(0)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)   //�Զ���ʻģʽ�л�����
#define READ_KLAXON_SWITCH      PDin(1)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)   //�����ȿ���
#define READ_LIGHT_TURN_RIGHT   PDin(2)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //��ת�ƿ���
#define READ_LIGHT_TURN_LEFT    PDin(3)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)  //��ת�ƿ���
#define READ_BEAN_LIGHT   	    PDin(4)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //Զ��ƿ���
#define READ_DIPPED_HEADLIGHT	PDin(5)//    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)   //����ƿ���
#define READ_D_GEAR        		PDin(6)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)      //D������
#define READ_R_GEAR       		PDin(7)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)      //R������
#define READ_BRAKE_CONTROL 		PDin(8)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)		//�ƶ�����
#define READ_ACCELERATOR		PDin(9)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)		//����ʹ��
#else
#define KLAXON_SWITCH		PBout(8)// ���ȼ̵���
#define TURN_RIGHT_LIGHT	PBout(9)// ��ת�Ƽ�Ъ�̵���  
#define TURN_LEFT_LIGHT		PEout(13)// ��ת�ƿ��ؼ�Ъ�̵���  
#define DIPPED_HEADLIGHT   	PEout(14)// ����Ƽ̵���
#define BEAN_LIGHT      	PBout(13)// Զ��Ƽ̵���
#define STOPLIGHT     		PBout(12)// ɲ���Ƽ̵���
#define REVERSING_LIGHT    	PGout(6)// �����Ƽ̵���
#define D_GEAR   			PDout(14)// ��λD
#define R_GEAR              PDout(12)// ��λR
#define BRAKE_CONTROL		PGout(5)// �����ƶ�����
#define ACCELERATOR_ENABLE	PDout(13)//����ʹ��


#define READ_DRIVING_MODE       PFin(4)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)   //�Զ���ʻģʽ�л�����
#define READ_KLAXON_SWITCH      PFin(3)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)   //�����ȿ���
#define READ_LIGHT_TURN_RIGHT   PFin(2)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //��ת�ƿ���
#define READ_LIGHT_TURN_LEFT    PFin(1)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)  //��ת�ƿ���
#define READ_BEAN_LIGHT   	    PFin(0)//GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //Զ��ƿ���
#define READ_DIPPED_HEADLIGHT	PEin(6)//    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)   //����ƿ���
#define READ_D_GEAR        		PEin(5)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)      //D������
#define READ_R_GEAR       		PEin(4)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)      //R������
#define READ_BRAKE_CONTROL 		PEin(3)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)		//�ƶ�����
#define READ_ACCELERATOR		PEin(2)//GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)		//����ʹ��

#endif

void IO_init(void);
void SpeedGPIOInit(void);
void AngleSensorGPIOInit(void);

#endif
