#ifndef __DRIVER_IO_H
#define __DRIVER_IO_H

#include "sys.h"

#define KLAXON_SWITCH		PBout(5)// 喇叭继电器
#define TURN_RIGHT_LIGHT		PBout(6)// 右转灯间歇继电器  
#define TURN_LEFT_LIGHT		PBout(7)// 左转灯开关间歇继电器  
#define DIPPED_HEADLIGHT   	PBout(8)// 近光灯继电器
#define BEAN_LIGHT      	PBout(9)// 远光灯继电器
#define STOPLIGHT     		PBout(10)// 刹车灯继电器
#define REVERSING_LIGHT    	PBout(11)// 倒车灯继电器
#define D_GEAR   			PBout(12)// 挡位D
#define R_GEAR              PBout(13)// 挡位R
#define BRAKE_CONTROL		PBout(14)// 主动制动控制
#define LED					PBout(15)// LED灯

#define READ_DRIVING_MODE    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)   //自动驾驶模式切换开关
#define READ_KLAXON_SWITCH     GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)   //电喇叭开关
#define READ_LIGHT_TURN_RIGHT  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //右转灯开关
#define READ_LIGHT_TURN_LEFT   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)  //左转灯开关
#define READ_BEAN_LIGHT   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //远光灯开关
#define READ_DIPPED_HEADLIGHT    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)   //近光灯开关
#define READ_D_GEAR        GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)      //D挡开关
#define READ_R_GEAR       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)      //R挡开关

void IO_init(void);
void SpeedGPIOInit(void);
void AngleSensorGPIOInit(void);

#endif
