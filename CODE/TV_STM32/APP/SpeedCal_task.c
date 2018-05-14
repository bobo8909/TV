#include "SpeedCal_task.h"

#define PI 3.1415f

#define R 0.268f

#define SPEED_RESOLUTION 0.337f

#define Speed(N) (N)*2*PI*R*36/8/12.49f 

static void SpeedCal(void)
{
	static u16 SpeedN = 0;
	static float SpeedV = 0;
	static u16 SpeedCANVal = 0;
	if(g_StructExtiFlag.bits.SpeedCalFlag == 1)
	{
		g_StructExtiFlag.bits.SpeedCalFlag = 0;
		
#if STM32_BOARD 
		SpeedN = (u16)SpeedPluseFunc();
		printf("SpeedN = %d\r\n",SpeedN);
		SpeedV = Speed(SpeedN);
		SpeedCANVal = SpeedV / SPEED_RESOLUTION;


		g_BCM1SendVal.SpeedVal = SpeedCANVal;
		printf("g_BCM1SendVal.SpeedVal = 0x%2x\r\n",g_BCM1SendVal.SpeedVal);

		//g_BCM1SendVal.SpeedVal = 0x22;
		//g_BCM2SendVal.AngleSensorSignalPHigh = 0x11;
		//g_BCM3SendVal.FrontSpeedHigh = 0x33;
#endif

	}
}

void SpeedCal_task(void)
{
	SpeedCal();
}
