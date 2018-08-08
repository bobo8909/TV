#include "driver_speed.h"

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

