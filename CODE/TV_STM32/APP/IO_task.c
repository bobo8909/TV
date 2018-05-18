#include "IO_task.h"




/* ====================================================================================================================
**  Function Name    :    driving mode detect
**  Process Timing   :    Any Time
**  Summary          :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    
**  Return Value     :    driving mode // 0:manned  1:automatic
==================================================================================================================== */
u8 CheckDrivingMode(void)
{
	u8 DrivingMode;
	
	if (READ_DRIVING_MODE == 1) 	   //automatic driving switch detect
	{	
		DrivingMode = AUTOMATIC_DRIVING;
	}
	else
	{
		DrivingMode = MANNED;
	}
	
	return DrivingMode;
}


/* ====================================================================================================================
**  Function Name    :    Signal Detection 
**  Process Timing   :    Any Time
**  Summary          :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */

void CheckIo(void)
{
	if(g_StructGlobalFlag.bits.DrivingModeSwitchFlag == 0)
	{
		g_StructBCMStatus.DrivingMode = CheckDrivingMode();

		g_BCM1SendVal.DRIVINGSTATUS.bits.DrivingModeStatus = g_StructBCMStatus.DrivingMode;
	}
	else
	{
		if (MANNED == CheckDrivingMode())
		{
			g_StructGlobalFlag.bits.DrivingModeSwitchFlag = 0;
		}
	}
	
	if (g_StructBCMStatus.DrivingMode  == MANNED)
	{
		BRAKE_CONTROL = OFF;
	}
	else
	{
		if (READ_BRAKE_CONTROL == 0x00)
		{
			if (g_BCM1SendVal.SpeedVal == 0x00)
			{
				g_StructBCMStatus.DrivingMode = MANNED;
				g_StructGlobalFlag.bits.DrivingModeSwitchFlag = 1;
			}
			else
			{
				BRAKE_CONTROL = ON;
				D_GEAR = OFF;
				R_GEAR = OFF;
				AcceleratorControl(OFF);
            }
		}
	}
	
	/*************************************	
		这里还差一个转角传感器异常的判断
	*************************************/
		
	if (READ_KLAXON_SWITCH == 0x01)       //电喇叭开关检测
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x00;
	}
	
	if (READ_LIGHT_TURN_RIGHT == 0x01)   //右转灯开关检测
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x01;
	}
	else 
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x00;
	}
	
	if (READ_LIGHT_TURN_LEFT == 0x01)    //左转灯开关检测
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x00;
	}
	
	if (READ_BEAN_LIGHT == 0x01)    //远光灯开关检测
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x00;
	}
	
	if (READ_DIPPED_HEADLIGHT == 0x01)     //近光灯开关检测
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x01;
	}
	else
	{	
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x00;
	}
	
	if ((READ_D_GEAR == 0) &&(READ_R_GEAR == 0)) //挡位检测
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x00;
	}
	else if((READ_D_GEAR == 1) &&(READ_R_GEAR == 0))
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x01;
	}
	else if((READ_D_GEAR == 0) &&(READ_R_GEAR == 1))
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x02;
	}
	else if((READ_D_GEAR == 1) &&(READ_R_GEAR == 1))
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x03;
		/*invalid*/
	}    

}



/* ====================================================================================================================
**  Function Name    :    Signal output 
**  Process Timing   :    Any Time
**  Summary          :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    
**  Return Value     :    -
==================================================================================================================== */

void IOControl(void)
{
	/*电喇叭控制*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal == 0x01)
	{		
		KLAXON_SWITCH = ON;
	}
	else
	{
		KLAXON_SWITCH = OFF;
	}

	/*刹车灯控制*/
	if(g_StructGlobalFlag.bits.StoplightFlag == 0x01)
	{
		STOPLIGHT = ON;
	}
	else
	{
		STOPLIGHT = OFF;
	}

	
	/*当在无人模式下时，采集到下面的几个信号时不能做相应的控制动作*/
	if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING)
	{
		return;
	}

	/*右转灯控制*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal == 0x01)
	{
		TURN_RIGHT_LIGHT = ON;
	}
	else
	{
		TURN_RIGHT_LIGHT = OFF;
	}

	/*左转灯控制*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal == 0x01)
	{
		TURN_LEFT_LIGHT = ON;	
	}
	else
	{
		TURN_LEFT_LIGHT = OFF;
	}

	/*远光灯控制*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal == 0x01)
	{
		BEAN_LIGHT = ON;
	}
	else
	{
		BEAN_LIGHT = OFF;
	}
	
	/*近光灯控制*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal == 0x01)
	{
		DIPPED_HEADLIGHT = ON;
	}
	else
	{
		DIPPED_HEADLIGHT = OFF;
	}
	
	/*挡位控制*/
	if (g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x00)//空挡
	{
		#if 1
		R_GEAR = OFF;
		D_GEAR = OFF;
		#else
		R_GEAR = ON;
		D_GEAR = ON;
		#endif	
		REVERSING_LIGHT = OFF;
	}
	else if(g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x01)//D挡
	{
		#if 1
		D_GEAR = ON;
		R_GEAR = OFF;
		#else
		D_GEAR = OFF;
		R_GEAR = ON;
		#endif
		REVERSING_LIGHT = OFF;
	}
	else if(g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x02)//R档
	{
		#if 1
		D_GEAR = OFF;
		R_GEAR = ON;
		#else
		D_GEAR = ON;
		R_GEAR = OFF;
		#endif
		REVERSING_LIGHT = ON;
	}
	
	/*油门使能信号*/
	if (READ_ACCELERATOR == 0x01)
	{
		ACCELERATOR_ENABLE = ON;
	}
	else
	{
		ACCELERATOR_ENABLE = OFF;
	}
}



void IO_task(void)
{	
	CheckIo();
	
	IOControl();
	
}
