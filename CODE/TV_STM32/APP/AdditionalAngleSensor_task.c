#include "AdditionalAngleSensor_task.h"

/*���ӽǶȴ���������ʹ���ⲿ�ж�����ȡ����*/
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
