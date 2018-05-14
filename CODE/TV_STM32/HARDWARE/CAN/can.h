#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    


typedef struct 
{
	u8 DLC;
	u8 Buf[8];
	u8 NewDataFlag;
}CANRXBUF;


extern CANRXBUF CANRecvDataBuf[3] ;



typedef struct _VCU_2
{
	u8 Accelerator1LOW;
	u8 Accelerator1HIGH;
	u8 Accelerator2LOW;
	u8 Accelerator2HIGH;
	union 
	{
		u8 Byte;
		struct
		{
			u8 b_AcceleratorEnable : 1;
			u8 b_GearsControlSig : 2;
			u8 : 5;
		}bits;
	}CONTROLSIGNAL;
	u8 b5;
	u8 b6;
	u8 b7;
}STRUCT_VCU_2;

typedef struct _VCU_3
{
	u8 EPSMomentalSig1LOW;
	u8 EPSMomentalSig1HIGH;
	u8 EPSMomentalSig2LOW;
	u8 EPSMomentalSig2HIGH;
	u8 AngleSensorSigPLow;
	u8 AngleSensorSigPHigh;
	u8 AngleSensorSigSLow;
	u8 AngleSensorSigSHigh;
}STRUCT_VCU_3;

typedef struct _VCU_5
{
	union
	{
		u8 Byte;
		struct 
		{
			u8 b_Klaxon : 1;
			u8 b_DippedHeadlight : 1;
			u8 b_BeamLight : 1;
			u8 b_TurnLeftLight : 1;
			u8 b_TurnRightLight : 1;
			u8 b5 : 1;
			u8 b6 : 1;
			u8 b7 : 1;
		}bits;
	}LIGHTSTATUS;

	union
	{
		u8 Byte;
		struct
		{
			u8 b_DrivingMode : 1;
			u8 unused : 7;
		}bits;
	}DRIVING_MODE;
	u8 b2;
	u8 b3;
	u8 b4;
	u8 b5;
	u8 b6;
	u8 CommunicationStatus;
}STRUCT_VCU_5;

typedef struct _BCM_1
{
	union
	{
		u8 Byte;
		struct
		{
			u8 DippedLightSignal : 1;
			u8 BeamlightSignal : 1;
			u8 TurnLeftSignal : 1;
			u8 TurnRightSignal : 1;
			u8 KlaxonSignal : 1;
			u8 BrakingAlarm : 1;
			u8 SteeringAlarm : 1;
			u8 AcceleratorAlarm : 1;
		}bits;
	}LIGHTSTATUS;

	union
	{
		u8 Byte;
		struct 
		{
			u8 GearStatusSignal : 2;
			u8 DrivingModeStatus : 1;
			u8 unused : 5;
		}bits;
	}DRIVINGSTATUS;

	u8 SpeedVal;
	u8 Accelerator1SignalLow;
	u8 Accelerator1SignalHigh;
	u8 Accelerator2SignalLow;
	u8 Accelerator2SignalHigh;
	u8 b7;
}STRUCT_BCM_1;

typedef struct _BCM_2
{
	u8 EPSMomentalSignal1Low;
	u8 EPSMomentalSignal1High;
	u8 EPSMomentalSignal2Low;
	u8 EPSMomentalSignal2High;
	u8 AngleSensorSignalPLow;
	u8 AngleSensorSignalPHigh;
	u8 AngleSensorSignalSLow;
	u8 AngleSensorSignalSHigh;
}STRUCT_BCM_2;

typedef struct _BCM_3
{
	u8 FrontSpeedLow;
	u8 FrontSpeedHigh;
	u8 b2;
	u8 b3;
	u8 b4;
	u8 b5;
	u8 b6;
	u8 b7;
}STRUCT_BCM_3;
//#define CANID_VCU_12  		(0x18DC11F7) //使能命令
//#define CANID_VCU_12  		(0x18F1CCD1) //自检命令
extern STRUCT_VCU_2 g_VCU2RecvVal;
extern STRUCT_VCU_3 g_VCU3RecvVal;
extern STRUCT_VCU_5 g_VCU5RecvVal;
extern STRUCT_BCM_1 g_BCM1SendVal;
extern STRUCT_BCM_2 g_BCM2SendVal;
extern STRUCT_BCM_3 g_BCM3SendVal;



#define CANID_VCU_2 			(0x18F122D1) //动力模块
#define CANID_VCU_3				(0x18F133D1) //转向模块
#define CANID_VCU_5				(0x18F144D1) //灯光喇叭





//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
										 							 				    
u8 CanModeInit(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN初始化
 
u8 CanSendMsg(uint32_t u32CanId, u8* ptrMsg, u8 u8Len);						//发送数据

u8 CanReceiveMsg(u8* buf);							//接收数据
#endif

















