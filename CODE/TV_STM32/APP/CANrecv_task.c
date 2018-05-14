#include "CANrecv_task.h"



void CANrecv_task(void)
{

	if(CANRecvDataBuf[0].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU2RecvVal,CANRecvDataBuf[0].Buf,8);
		
		CANRecvDataBuf[0].NewDataFlag = 0;		
//		g_StructGlobalFlag.bits.RecvVCU2Flag = 1;
	}

	if(CANRecvDataBuf[1].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU3RecvVal,CANRecvDataBuf[1].Buf,8);
		
		CANRecvDataBuf[1].NewDataFlag = 0;
//		g_StructGlobalFlag.bits.RecvVCU3Flag = 1;
	}

	if(CANRecvDataBuf[2].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU5RecvVal,CANRecvDataBuf[2].Buf,8);
		
		CANRecvDataBuf[2].NewDataFlag = 0;
//		g_StructGlobalFlag.bits.RecvVCU5Flag = 1;
	}
	
}


