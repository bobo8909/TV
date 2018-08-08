#include "driver_ad5734r.h"

//u16 g_DABuf[4] = {0};

/*******************************
 *函数名:
 *函数功能:
 *参数:
 *返回值:
********************************/
void AD5734R_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1|GPIO_Pin_4);
	GPIO_SetBits(GPIOA, GPIO_Pin_3|GPIO_Pin_2);
    
    SPI1_Init();  //初始化SPI1

    CLR = 0;
	delay_us(1);
	CLR = 1;

    /////////////////////////////通道ABCD//////////////////////////////输出范围选择寄存器(0-5V)
    SYNC = 1;        //SYNC低电平写入数据 
    LDAC = 1;
		
    delay_us(1);                
	SYNC = 0;   
	SPI_I2S_SendData(SPI1, 0x0C); //选择四个通道
	delay_us(5);
	SPI_I2S_SendData(SPI1, 0x00);
	delay_us(5);
	SPI_I2S_SendData(SPI1, 0x00); //设置电压范围为0-5V
	   
	delay_us(8);
	SYNC = 1;    //锁存	
	delay_ms(1);

	/////////////////////////////通道ABCD///////////////////////////////电源控制寄存器
	SYNC = 1;        //SYNC低电平写入数据
	LDAC = 1;
		
	delay_us(1);                      
	SYNC = 0;   
	SPI_I2S_SendData(SPI1, 0x10);
	delay_us(5);
	SPI_I2S_SendData(SPI1, 0x00);
	delay_us(5);
	SPI_I2S_SendData(SPI1, 0x0F);
	   
	delay_us(8);
	SYNC = 1;    //锁存

	////////////////////////////通道A///////////////写数据
//	SYNC = 1;        //SYNC低电平写入数据
//	LDAC = 1;
//		
//	delay_us(1);                     
//	SYNC = 0;   
//	SPI_I2S_SendData(SPI1, 0x00);
//	delay_us(5);
//	SPI_I2S_SendData(SPI1, 0xFF);
//	delay_us(5);
//	SPI_I2S_SendData(SPI1, 0xFC);
//	delay_us(8);
//	SYNC = 1;    //锁存
//	delay_ms(1);
//		
//	////////////////////////////通道B///////////////写数据
//	delay_us(1);                     
//	SYNC = 0;   
//	SPI_I2S_SendData(SPI1, 0x01);
//	delay_us(5);
//	SPI_I2S_SendData(SPI1, 0x28);
//	delay_us(5);
//	SPI_I2S_SendData(SPI1, 0xF0);
//	   
//	delay_us(8);
//	SYNC = 1;    //锁存
//	delay_ms(1);
		
	LDAC = 0;    
    
    
		
}


/*******************************
 *函数名:
 *函数功能:接受CAN消息，发出控制命令
 *参数:
 *返回值:
********************************/
void AD5734R_Control(u8 RegDataBuf[8])
{
//    u8 i;
    u8 u8Temp = 0;

//    g_CANRX0Buff[0] = 0xFC;  //测试
//    g_CANRX0Buff[1] = 0xFF;  //测试  
        
	if(1/*g_CANReceiveFlag1 == 1*/)
	{
		
		////////////////////////////通道A///////////////写数据
		SYNC = 1;        //SYNC低电平写入数据
		LDAC = 1;
#if 1		
		delay_us(1);                     
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x00);
		delay_us(5);
		u8Temp = ((RegDataBuf[1] << 2) | (RegDataBuf[0] >> 6));
		SPI_I2S_SendData(SPI1, u8Temp);
		delay_us(5);
		u8Temp = (RegDataBuf[0] << 2);
		SPI_I2S_SendData(SPI1,u8Temp);
		delay_us(8);
		SYNC = 1;    //锁存
		delay_ms(1);
		
		////////////////////////////通道B///////////////写数据
		delay_us(1);                     
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x01);
		delay_us(5);
		u8Temp = ((RegDataBuf[3] << 2) | (RegDataBuf[2] >> 6));
		SPI_I2S_SendData(SPI1, u8Temp);
		delay_us(5);
		u8Temp = (RegDataBuf[2] << 2);
		SPI_I2S_SendData(SPI1, u8Temp);
	   
		delay_us(8);
		SYNC = 1;    //锁存
		delay_ms(1);
		
//		LDAC = 0;

        ////////////////////////////通道C///////////////写数据
		delay_us(1);                     
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x02);
		delay_us(5);
		u8Temp = ((RegDataBuf[5] << 2) | (RegDataBuf[4] >> 6));
		SPI_I2S_SendData(SPI1, u8Temp);
		delay_us(5);
		u8Temp = (RegDataBuf[4] << 2);
		SPI_I2S_SendData(SPI1, u8Temp);
	   
		delay_us(8);
		SYNC = 1;    //锁存
		delay_ms(1);
#endif		

		
		////////////////////////////通道D///////////////写数据
		delay_us(1);                     
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x03);
		delay_us(5);
		u8Temp = ((RegDataBuf[7] << 2) | (RegDataBuf[6] >> 6));
		SPI_I2S_SendData(SPI1, u8Temp);
		delay_us(5);
		u8Temp = (RegDataBuf[6] << 2);
		SPI_I2S_SendData(SPI1, u8Temp);
	   
		delay_us(8);
		SYNC = 1;    //锁存
		delay_ms(1);
		
		LDAC = 0;
      		
	}
    else
    {
#if 0    
      /////////////////////////////通道ABCD//////////////////////////////输出范围选择寄存器
		SYNC = 1;        //SYNC低电平写入数据 
		LDAC = 1;
		
		delay_us(1);                
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x0C);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0x00);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0x04);
	   
		delay_us(8);
		SYNC = 1;    //锁存	
		delay_ms(1);

		/////////////////////////////通道ABCD///////////////////////////////电源控制寄存器
		SYNC = 1;        //SYNC低电平写入数据
		LDAC = 1;
		
		delay_us(1);                      
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x10);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0x00);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0x0F);
	   
		delay_us(8);
		SYNC = 1;    //锁存

		////////////////////////////通道A///////////////写数据
		SYNC = 1;        //SYNC低电平写入数据
		LDAC = 1;
		
		delay_us(1);                     
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x00);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0xFF);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0xFC);
		delay_us(8);
		SYNC = 1;    //锁存
		delay_ms(1);
		
		////////////////////////////通道B///////////////写数据
		delay_us(1);                     
		SYNC = 0;   
		SPI_I2S_SendData(SPI1, 0x01);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0x28);
		delay_us(5);
		SPI_I2S_SendData(SPI1, 0xF0);
	   
		delay_us(8);
		SYNC = 1;    //锁存
		delay_ms(1);
		
		LDAC = 0;
        
#endif

    }
	
 #if 0       
	if(g_CANReceiveFlag2 == 1)  //预留 
	{
		
        
	}
	else
	{
        
        
	}
	#endif
}


#if 0
/*******************************
 *函数名:
 *函数功能:AD5734R的写数据函数
 *参数:[IN]reg:DAC寄存器地址
       [IN]ChannelAddr:DAC通道
       [IN]data:写入的数据
 *返回值:none
********************************/
void AD5734R_Write(u8 reg, u8 ChannelAddr, u16 data)
{
    u8 u8Temp = 0;

    data = data << 2;  //左移2位
    
    delay_us(1);                     
	SYNC = 0;   
    u8Temp = (reg << 3) | ChannelAddr;     
	SPI_I2S_SendData(SPI1, u8Temp);
	delay_us(5);
	u8Temp = (u8)(data >> 8);    //高字节
	SPI_I2S_SendData(SPI1, u8Temp);
	delay_us(5);
	u8Temp = (u8)(data);         //低字节
	SPI_I2S_SendData(SPI1, u8Temp);
	   
	delay_us(8);
	SYNC = 1;    //锁存
	delay_ms(1);
	LDAC = 0;
    
}
#endif

void AD5734R_Write(u8 *data)
{
    u8 u8Temp = 0;
    
    SYNC = 1;        //SYNC低电平写入数据
    LDAC = 1;
		
	delay_us(1);                     
	SYNC = 0;   
	SPI_I2S_SendData(SPI1, 0x00);
	delay_us(5);
	u8Temp = (data[1] << 2) | (data[0] >> 6);
	SPI_I2S_SendData(SPI1, u8Temp);
	delay_us(5);
	u8Temp = (data[0] << 2);
	SPI_I2S_SendData(SPI1, u8Temp);
	delay_us(8);

    LDAC = 0;
	SYNC = 1;    //锁存

    
}

void AD5734R_Read(u8 *arr)
{
    
}





