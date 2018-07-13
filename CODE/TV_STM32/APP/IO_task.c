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
	static u8 flag1= 0,flag2= 0;
	if (READ_DRIVING_MODE == 0) 	   //automatic driving switch detect
	{	
		DrivingMode = AUTOMATIC_DRIVING;
        if(flag1 == 0)
        {
            printf("auto\r\n");
            flag1 = 1;
            flag2 = 0;
            }
	}
	else
	{
		DrivingMode = MANNED;
        if(flag2 ==0 )
            {
        printf("manned\r\n");
        flag1 = 0;
        flag2 = 1;
            }
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
	#if 0
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
				D_GEAR = ON;
				R_GEAR = ON;
				AcceleratorControl(OFF);
            }
		}
        #endif
	}
	
	/*************************************	
		���ﻹ��һ��ת�Ǵ������쳣���ж�
	*************************************/
		
	if (READ_KLAXON_SWITCH == INPUT_ON)       //�����ȿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x00;
	}
	
	if (READ_LIGHT_TURN_RIGHT == INPUT_ON)   //��ת�ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x01;
	}
	else 
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x00;
	}
	
	if (READ_LIGHT_TURN_LEFT == INPUT_ON)    //��ת�ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x00;
	}
	
	if (READ_BEAN_LIGHT == INPUT_ON)    //Զ��ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x01;
        //printf("yuanguang kai\r\n");
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x00;
        //printf("yuanguang guan\r\n");
	}
	
	if (READ_DIPPED_HEADLIGHT == INPUT_ON)     //����ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x01;
        //printf("jinguang kai\r\n");
	}
	else
	{	
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x00;
        //printf("jinguang guan\r\n");
	}
	
	if ((READ_D_GEAR == INPUT_OFF) &&(READ_R_GEAR == INPUT_OFF)) //��λ���
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x00;
	}
	else if((READ_D_GEAR == INPUT_ON) &&(READ_R_GEAR == INPUT_OFF))
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x01;
	}
	else if((READ_D_GEAR == INPUT_OFF) &&(READ_R_GEAR == INPUT_ON))
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x02;
	}
	else if((READ_D_GEAR == INPUT_ON) &&(READ_R_GEAR == INPUT_ON))
	{
		g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal = 0x03;
		/*invalid*/
	}    

	if (READ_BRAKE_CONTROL == INPUT_OFF)     //ɲ����
	{
		g_StructGlobalFlag.bits.StoplightFlag = 0x01;
        //printf("brake 1\r\n");
	}
	else
	{	
		g_StructGlobalFlag.bits.StoplightFlag = 0x00;
        //printf("brake 0\r\n");
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

	/*�����ȿ���*/
	if ((g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal == 0x01) ||(g_VCU5RecvVal.LIGHTSTATUS.bits.b_Klaxon == 0x01))
	{		
		KLAXON_SWITCH = ON;
	}
	else
	{
		KLAXON_SWITCH = OFF;
	}

	/*ɲ���ƿ���*/
	if(g_StructGlobalFlag.bits.StoplightFlag == 0x01)
	{
		STOPLIGHT = ON;
	}
	else
	{
		STOPLIGHT = OFF;
	}

	
	/*��������ģʽ��ʱ���ɼ�������ļ����ź�ʱ��������Ӧ�Ŀ��ƶ���*/
	if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING)
	{
		return;
	}

	/*��ת�ƿ���*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal == 0x01)
	{
		TURN_RIGHT_LIGHT = ON;
	}
	else
	{
		TURN_RIGHT_LIGHT = OFF;
	}

	/*��ת�ƿ���*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal == 0x01)
	{
		TURN_LEFT_LIGHT = ON;	
	}
	else
	{
		TURN_LEFT_LIGHT = OFF;
	}

	/*Զ��ƿ���*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal == 0x01)
	{
		BEAN_LIGHT = ON;
	}
	else
	{
		BEAN_LIGHT = OFF;
	}
	
	/*����ƿ���*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal == 0x01)
	{
		DIPPED_HEADLIGHT = ON;
	}
	else
	{
		DIPPED_HEADLIGHT = OFF;
	}
	
	/*��λ����*/
	if (g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x00)//�յ�
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
	else if(g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x01)//D��
	{
		#if 1
		D_GEAR = ON;
		R_GEAR = OFF;
		#else
		D_GEAR = OFF;
		R_GEAR = ON;
		#endif
		REVERSING_LIGHT = OFF;
//        printf("D\r\n");
	}
	else if(g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x02)//R��
	{
		#if 1
		D_GEAR = OFF;
		R_GEAR = ON;
		#else
		D_GEAR = ON;
		R_GEAR = OFF;
		#endif
//        printf("R\r\n");
		REVERSING_LIGHT = ON;
        //printf("r dang\r\n");
	}
	
	/*����ʹ���ź�*/
	if (READ_ACCELERATOR == 0x00)
	{
		ACCELERATOR_ENABLE = ON;
        //printf("acc on\r\n");
	}
	else
	{
		ACCELERATOR_ENABLE = OFF;
        //printf("acc off\r\n");
	}
}



void IO_task(void)
{	
	CheckIo();
	
	IOControl();
	
}
