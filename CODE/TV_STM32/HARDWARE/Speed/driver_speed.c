#include "driver_speed.h"

/**
  * @brief  �ٶ��������ص�GPIO��ʼ��
  *        
  * @param  None
  *   
  * @retval None
  */
void SpeedGPIOInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

