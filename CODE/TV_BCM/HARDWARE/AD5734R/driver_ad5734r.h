
#ifndef __DRIVER_AD5734R_H
#define __DRIVER_AD5734R_H

#include "sys.h"  


#define SYNC PAout(4)
#define LDAC PAout(1)
#define CLR  PAout(3)

/***************** AD5374R的寄存器地址 ******************/
#define DAC_REG           0x00   //DAC寄存器
#define OUTPUT_RANGE_REG  0x01   //输出范围选择寄存器
#define POWER_REG         0x02   //电源控制寄存器
#define CONTROL_REG       0x03   //控制寄存器

/***************** AD5374R的通道地址 ******************/
#define DAC_A             0x00   //DAC的A通道
#define DAC_B             0x01   //DAC的B通道
#define DAC_C             0x02   //DAC的C通道
#define DAC_D             0x03   //DAC的D通道
#define DAC_ABCD          0x04   //DAC的ABCD通道


//#define BIN_2sCOMP PAout(2) 

//extern u16 g_DABuf[4];



void AD5734R_Init(void);
void AD5734R_Control(u8 RegDataBuf[8]);
//void AD5734R_Write(u8 reg, u8 ChannelAddr, u16 data);
void AD5734R_Write(u8 *data);







#endif  /* __AD5734R_H */





