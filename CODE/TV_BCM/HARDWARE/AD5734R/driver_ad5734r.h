
#ifndef __DRIVER_AD5734R_H
#define __DRIVER_AD5734R_H

#include "sys.h"  


#define SYNC PAout(4)
#define LDAC PAout(1)
#define CLR  PAout(3)

/***************** AD5374R�ļĴ�����ַ ******************/
#define DAC_REG           0x00   //DAC�Ĵ���
#define OUTPUT_RANGE_REG  0x01   //�����Χѡ��Ĵ���
#define POWER_REG         0x02   //��Դ���ƼĴ���
#define CONTROL_REG       0x03   //���ƼĴ���

/***************** AD5374R��ͨ����ַ ******************/
#define DAC_A             0x00   //DAC��Aͨ��
#define DAC_B             0x01   //DAC��Bͨ��
#define DAC_C             0x02   //DAC��Cͨ��
#define DAC_D             0x03   //DAC��Dͨ��
#define DAC_ABCD          0x04   //DAC��ABCDͨ��


//#define BIN_2sCOMP PAout(2) 

//extern u16 g_DABuf[4];



void AD5734R_Init(void);
void AD5734R_Control(u8 RegDataBuf[8]);
//void AD5734R_Write(u8 reg, u8 ChannelAddr, u16 data);
void AD5734R_Write(u8 *data);







#endif  /* __AD5734R_H */





