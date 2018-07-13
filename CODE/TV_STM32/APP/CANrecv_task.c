#include "CANrecv_task.h"



/* ====================================================================================================================
**  Function Name    :    Anysis can and control The light
**  Process Timing   :    Any  time
**  Summary          :    -
**  Arguments[Input] :    
**  Arguments[Output]:    
**  Return Value     :    -
==================================================================================================================== */

static void CanControl(void)
{		
#if 0
	//电喇叭控制
	if(g_VCU5RecvVal.LIGHTSTATUS.bits.b_Klaxon == 0x01)   
	{
		KLAXON_SWITCH = ON;
        //printf("klaxon on\r\n");
	}
	else
	{
		KLAXON_SWITCH = OFF;
        //printf("klaxon off\r\n");
	}
#endif
	//挡位控制
	if (g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x00) //neutral gear 空挡
	{
		#if 1
		D_GEAR = OFF;
		R_GEAR = OFF;
		#else
		D_GEAR = ON;
		R_GEAR = ON;
		#endif

		REVERSING_LIGHT = OFF;			
		//printf("neutral gear\r\n");
	}
	else if(g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x01)//forward gear 前进挡
    {
    	#if 1
    	D_GEAR = ON;
		R_GEAR = OFF;   
		#else
    	D_GEAR = OFF;
		R_GEAR = ON;   
		#endif

		REVERSING_LIGHT = OFF;
		//printf("forward gear\r\n");
	}
	else if(g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x02)//reserve gear 后退挡
	{
		#if 1
    	D_GEAR = OFF;
		R_GEAR = ON;  
		#else
    	D_GEAR = ON;
		R_GEAR = OFF;  
		#endif
		
		REVERSING_LIGHT = ON;
		//printf("reserve gear\r\n");
	}
	else
	{
		/*INVALID*/
		//printf("INVALID gear\r\n");
	}

	/*油门使能*/
	if (g_VCU2RecvVal.CONTROLSIGNAL.bits.b_AcceleratorEnable == 0x01)
	{
		ACCELERATOR_ENABLE = ON;
	}
	else
	{
		ACCELERATOR_ENABLE = OFF;
	}

	/*近光灯*/
	if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_DippedHeadlight == 0x01)
	{
		DIPPED_HEADLIGHT = ON;
		//printf("DIPPED_HEADLIGHT\r\n");
	}
	else 
	{
		DIPPED_HEADLIGHT = OFF;
		//printf("INVALID gear\r\n");
	}

	/*远光灯*/
	if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_BeamLight == 0x01)
	{
		BEAN_LIGHT = ON;		
	}
	else 
	{
		BEAN_LIGHT = OFF;		
	}

	/*左转灯*/
	if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_TurnLeftLight == 0x01)
	{
		TURN_LEFT_LIGHT = ON;
	}
	else
	{
		TURN_LEFT_LIGHT = OFF;
	}

	/*右转灯*/
	if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_TurnRightLight == 0x01)
	{
		TURN_RIGHT_LIGHT = ON;
	}
	else
	{
		TURN_RIGHT_LIGHT = OFF;
	}

}


void CANrecv_task(void)
{
//	u8 busrec = 0;
    /*如果收不到CAN数据*/
    if((CANRecvDataBuf[0].NewDataFlag == 0)&&(CANRecvDataBuf[1].NewDataFlag == 0)
        &&(CANRecvDataBuf[2].NewDataFlag == 0))
    {
        g_StructGlobalFlag.bits.bCANConnectionError = 1;
    }

    /*正常接收CAN数据*/
	if (CANRecvDataBuf[0].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU2RecvVal,CANRecvDataBuf[0].Buf,8);
        
        g_StructGlobalFlag.bits.bCANConnectionError = 0;

		CANRecvDataBuf[0].NewDataFlag = 0;		
	}

	if (CANRecvDataBuf[1].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU3RecvVal,CANRecvDataBuf[1].Buf,8);
		
		CANRecvDataBuf[1].NewDataFlag = 0;

        g_StructGlobalFlag.bits.bCANConnectionError = 0;
	}

	if (CANRecvDataBuf[2].NewDataFlag == 1)
	{
	
		memcpy(&g_VCU5RecvVal,CANRecvDataBuf[2].Buf,8);
		
		CANRecvDataBuf[2].NewDataFlag = 0;

		if(g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
		{
			g_StructBCMStatus.DrivingMode = MANNED;
		}
		else
		{
			g_StructBCMStatus.DrivingMode = AUTOMATIC_DRIVING;
		}

        g_StructGlobalFlag.bits.bCANConnectionError = 0;
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
		else
		{
			g_StructGlobalFlag.bits.StoplightFlag = 0;				
		}
		
		if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING)
		{
			//if(g_EBS1RecvVal.MainCylinderBrakePressure >= 55)
			//{
			//	BRAKE_CONTROL = 0;
			//}
			//else
			//{
			//	BRAKE_CONTROL = 1;
			//}
		}
	}
#endif

	
	if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING)
	{
		CanControl();
	}

//	if(CAN_GetReceiveErrorCounter(CAN1) != 0)
//	{
//		busrec = CAN_GetReceiveErrorCounter(CAN1) ;
//		printf("rec=%d\r\n",busrec);
//	}
#if 0
	if(CAN_GetReceiveErrorCounter(CAN1) != 0)
	{
		busrec = CAN_GetReceiveErrorCounter(CAN1) ;
		printf("grec=%d\r\n",busrec);
	}
	if (CAN_GetLSBTransmitErrorCounter(CAN1) != 0)
	{
		busrec = CAN_GetLSBTransmitErrorCounter(CAN1) ;
		printf("gtec=%d\r\n",busrec);	
	}
	if(CAN_GetITStatus(CAN1,CAN_IT_LEC) == SET)
	{
		busrec = CAN_GetLSBTransmitErrorCounter(CAN1) ;
		printf("gcan lec\r\n");
		//CanModeInit(CAN_SJW_1tq,CAN_BS2_8tq, CAN_BS1_9tq, 8, CAN_Mode_Normal);//CAN普通模式初始化, 波特率250Kbps 	
	}
	
	if(CAN_GetITStatus(CAN1,CAN_IT_BOF) == SET)
	{
		busrec = CAN_GetLSBTransmitErrorCounter(CAN1) ;
		printf("gcan busoff\r\n");
		//CanModeInit(CAN_SJW_1tq,CAN_BS2_8tq, CAN_BS1_9tq, 8, CAN_Mode_Normal);//CAN普通模式初始化, 波特率250Kbps 	
	}

	
#endif

}


