#include "driver_IO.h"

void IO_init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;	
 #if STM32_EVALUATION
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;	    		 // ???ú????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出，IO口速度为2MHz
	GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
	
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD 0,1,2,3,4,5,6,7

	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
 #else
    /*GPIO输出初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //推挽输出，IO口速度为2MHz
    GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                   //推挽输出，IO口速度为2MHz
    GPIO_ResetBits(GPIOE,GPIO_Pin_14|GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);                   //推挽输出，IO口速度为2MHz
    GPIO_ResetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);                   //推挽输出，IO口速度为2MHz
    GPIO_ResetBits(GPIOG,GPIO_Pin_5|GPIO_Pin_6);

    /*GPIO输入初始化*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 2,3,4,5,6,

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3| GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF 0,1,2,3,4

 #endif
		
}

/**
  * @brief  速度脉冲的相关的GPIO初始化
  *        
  * @param  None
  *   
  * @retval None
  */
void SpeedGPIOInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  附加角度传感器脉冲的相关的GPIO初始化
  *        
  * @param  None
  *   
  * @retval None
  */
void AngleSensorGPIOInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

