#include "AdditionalAngleSensor_task.h"

/*附加角度传感器任务，使用外部中断来获取脉冲*/
#if 0
void AdditionalAngleSensor_task(void)
{
//	u16 AngSenorA = 0;
//	u16 AngSenorB = 0;
	
	if(g_StructExtiFlag.bits.ExtiAngSensorACalFlag == 1)
	{
		g_StructExtiFlag.bits.ExtiAngSensorACalFlag = 0;
//		AngSenorA = APluseFunc();
		//printf("AngSenorA = %d");
	}

	
	if(g_StructExtiFlag.bits.ExtiAngSensorBCalFlag == 1)
	{
		g_StructExtiFlag.bits.ExtiAngSensorBCalFlag = 0;
//		AngSenorB = BPluseFunc();
		//printf("AngSenorA = %d");
	}
}
#endif
