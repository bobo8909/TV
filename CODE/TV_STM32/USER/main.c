#include "sys.h"



int main(void)
{		

	System_init();

    while(1)
    {
    	IWDG_Feed();  //ι��

		Led_task();
		
		/*������յ���CAN����*/
		CANrecv_task();
		
	    #if STM32_BOARD
	    	/*PWM�������������*/		
			IO_task();
	        PWM_task();
			ADC_task();
			DAC_task();
			SpeedCal_task();
			AngSensor_task();
			Encoder_task();
		#endif

		#if JSDG_BOARD
			IO_task();
		#endif

		#if ZXKZ_BOARD
			DAC_task();
		#endif

		/*CAN��������*/
		CANsend_task();
        
    }
}


