#include "CANrecv_task.h"



void CANrecv_task(void)
{

	if (CANRecvDataBuf[0].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU2RecvVal,CANRecvDataBuf[0].Buf,8);

		if ((g_VCU2RecvVal.Accelerator1HIGH != 0x00) && (g_VCU2RecvVal.Accelerator1LOW != 0x00))
		{
			g_TIMFlag.bits.DACSendFlag = 1;
		}
		CANRecvDataBuf[0].NewDataFlag = 0;		
//		g_StructGlobalFlag.bits.RecvVCU2Flag = 1;
	}

	if (CANRecvDataBuf[1].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU3RecvVal,CANRecvDataBuf[1].Buf,8);
		
		CANRecvDataBuf[1].NewDataFlag = 0;
//		g_StructGlobalFlag.bits.RecvVCU3Flag = 1;
	}

	if (CANRecvDataBuf[2].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU5RecvVal,CANRecvDataBuf[2].Buf,8);
		
		CANRecvDataBuf[2].NewDataFlag = 0;
//		g_StructGlobalFlag.bits.RecvVCU5Flag = 1;
	}
	#if 0
		if(CANRecvBCMBuf.NewDataFlag == 1)
		{
		
			g_StructBCMStatus.DrivingMode = CANRecvBCMBuf.Buf[1] & 0x04;
			
			CANRecvBCMBuf.NewDataFlag = 0;
	//		g_StructGlobalFlag.bits.RecvVCU5Flag = 1;
		}
	#else
		if (CANRecvEBSBuf.NewDataFlag == 1)
		{
			memcpy(&g_EBS1RecvVal,CANRecvEBSBuf.Buf,8);
			CANRecvEBSBuf.NewDataFlag = 0;
		
			if ((g_EBS1RecvVal.FrontBrakePressure != 0) || (g_EBS1RecvVal.RearBrakePressure != 0))
			{
				g_StructGlobalFlag.bits.StoplightFlag = 1;
			}
			if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING)
			{
				if(g_EBS1RecvVal.MainCylinderBrakePressure >= 55)
				{
					BRAKE_CONTROL = 0;
				}
				else
				{
					BRAKE_CONTROL = 1;
				}
			}
		}

	#endif
}


