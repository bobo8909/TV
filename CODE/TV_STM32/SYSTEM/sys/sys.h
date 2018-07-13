#ifndef __SYS_H
#define __SYS_H	

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_iwdg.h"

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "can.h"
#include "driver_speed.h"
#include "driver_IO.h"
#include <string.h>
#include "driver_spi.h"
#include "driver_ad5734r.h"
#include "iwdg.h"
#include "adc.h"
#include "dac.h"
#include "exti.h"


#include "AngleSensor_task.h"
#include "CANsend_task.h"
#include "CANrecv_task.h"
#include "IO_task.h"
#include "DAC_task.h"
#include "ADC_task.h"
#include "SpeedCal_task.h"
#include "AdditionalAngleSensor_task.h"
#include "Encoder_task.h"



#define STM32_BOARD 1
#define JSDG_BOARD 0 
#define ZXKZ_BOARD 0

#define  MANNED 0
#define  AUTOMATIC_DRIVING 1

typedef struct _BCM_STATUS
{
	u8 DrivingMode;
}STRUCT_BCM_STATUS;

extern STRUCT_BCM_STATUS g_StructBCMStatus;


typedef union
{
		u8 byte;
		struct 
		{
			u8 RecvVCU2Flag : 1;
			u8 RecvVCU3Flag : 1;
			u8 RecvVCU5Flag : 1;
			u8 AngSensorACalFlag : 1;
			u8 AngSensorBCalFlag : 1;
			u8 StoplightFlag : 1;
			u8 DrivingModeSwitchFlag : 1;
			u8 bCANConnectionError : 1;
		}bits;
}STRUCT_GLOBAL_FLAG;
extern STRUCT_GLOBAL_FLAG g_StructGlobalFlag;

typedef union
{
		u8 byte;
		struct 
		{
			u8 SpeedCalFlag : 1;
			u8 SpeedStartFlag : 1;
			u8 SpeedCalTimeoutFlag : 1;
			u8 b3 : 1;
			u8 b4 : 1;
			u8 b5 : 1;
			u8 b6 : 1;
			u8 b7 : 1;
		}bits;
}STRUCT_EXTI_FLAG;
extern STRUCT_EXTI_FLAG g_StructExtiFlag;

extern CanRxMsg CANSendBuf[3] ;
																	    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif


void System_init(void);

