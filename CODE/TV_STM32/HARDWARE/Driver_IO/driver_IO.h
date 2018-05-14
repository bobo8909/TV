#ifndef __DRIVER_IO_H
#define __DRIVER_IO_H

#include "sys.h"

#define KLAXON_SWITCH		PBout(5)// ���ȼ̵���
#define TURN_RIGHT_LIGHT		PBout(6)// ��ת�Ƽ�Ъ�̵���  
#define TURN_LEFT_LIGHT		PBout(7)// ��ת�ƿ��ؼ�Ъ�̵���  
#define DIPPED_HEADLIGHT   	PBout(8)// ����Ƽ̵���
#define BEAN_LIGHT      	PBout(9)// Զ��Ƽ̵���
#define STOPLIGHT     		PBout(10)// ɲ���Ƽ̵���
#define REVERSING_LIGHT    	PBout(11)// �����Ƽ̵���
#define D_GEAR   			PBout(12)// ��λD
#define R_GEAR              PBout(13)// ��λR
#define BRAKE_CONTROL		PBout(14)// �����ƶ�����
#define LED					PBout(15)// LED��

#define READ_DRIVING_MODE    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)   //�Զ���ʻģʽ�л�����
#define READ_KLAXON_SWITCH     GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)   //�����ȿ���
#define READ_LIGHT_TURN_RIGHT  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //��ת�ƿ���
#define READ_LIGHT_TURN_LEFT   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)  //��ת�ƿ���
#define READ_BEAN_LIGHT   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //Զ��ƿ���
#define READ_DIPPED_HEADLIGHT    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)   //����ƿ���
#define READ_D_GEAR        GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)      //D������
#define READ_R_GEAR       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)      //R������

void IO_init(void);
void SpeedGPIOInit(void);
void AngleSensorGPIOInit(void);

#endif
