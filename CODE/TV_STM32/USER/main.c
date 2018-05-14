#include "sys.h"



int main(void)
{		

	System_init();

    while(1)
    {
    	IWDG_Feed();  //喂狗

		Led_task();
		
		/*处理接收到的CAN数据*/
		CANrecv_task();
		
	    #if STM32_BOARD
	    	/*PWM的输入输出处理*/
	        PWM_task();
			ADC_task();
			DAC_task();
			SpeedCal_task();
			AngSensor_task();
		#endif

		#if JSDG_BOARD
			IO_task();
		#endif

		#if ZXKZ_BOARD
			DAC_task();
		#endif

		/*CAN发送数据*/
		CANsend_task();
        
    }
}


