#ifndef __DRIVER_IO_H
#define __DRIVER_IO_H

#include "sys.h"

#define KLAXON_SWITCH		PFout(0)// 喇叭继电器
#define TURN_RIGHT_LIGHT	PFout(1)// 右转灯间歇继电器  
#define TURN_LEFT_LIGHT		PFout(2)// 左转灯开关间歇继电器  
#define DIPPED_HEADLIGHT   	PFout(3)// 近光灯继电器
#define BEAN_LIGHT      	PFout(4)// 远光灯继电器
#define STOPLIGHT     		PFout(5)// 刹车灯继电器
#define REVERSING_LIGHT    	PFout(6)// 倒车灯继电器
#define D_GEAR   			PFout(7)// 挡位D
#define R_GEAR              PFout(9)// 挡位R
#define BRAKE_CONTROL		PFout(10)// 主动制动控制
#define ACCELERATOR_ENABLE	PFout(11)//油门使能


#define READ_DRIVING_MODE    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)   //自动驾驶模式切换开关
#define READ_KLAXON_SWITCH     GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)   //电喇叭开关
#define READ_LIGHT_TURN_RIGHT  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //右转灯开关
#define READ_LIGHT_TURN_LEFT   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)  //左转灯开关
#define READ_BEAN_LIGHT   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //远光灯开关
#define READ_DIPPED_HEADLIGHT    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)   //近光灯开关
#define READ_D_GEAR        GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)      //D挡开关
#define READ_R_GEAR       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)      //R挡开关
#define READ_BRAKE_CONTROL GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)		//制动控制
#define READ_ACCELERATOR	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)		//油门使能


void IO_init(void);
void SpeedGPIOInit(void);
void AngleSensorGPIOInit(void);

#endif
