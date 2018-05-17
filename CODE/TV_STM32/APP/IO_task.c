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
**  Arguments[Output]:    ucTemp
**  Return Value     :    -
==================================================================================================================== */

void CheckIo(void)
{
	if(g_StructGlobalFlag.bits.DrivingModeSwitchFlag == 0)
	{
		g_StructBCMStatus.DrivingMode = CheckDrivingMode();
	}
	else
	{
		if (READ_DRIVING_MODE == 0)
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
		���ﻹ��һ��ת�Ǵ������쳣���ж�
	*************************************/
		
	if (READ_KLAXON_SWITCH == 0x01)       //�����ȿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x00;
	}
	
	if (READ_LIGHT_TURN_RIGHT == 0x01)   //��ת�ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x01;
	}
	else 
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x00;
	}
	
	if (READ_LIGHT_TURN_LEFT == 0x01)    //��ת�ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x00;
	}
	
	if (READ_BEAN_LIGHT == 0x01)    //Զ��ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x01;
	}
	else
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x00;
	}
	
	if (READ_DIPPED_HEADLIGHT == 0x01)     //����ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x01;
	}
	else
	{	
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x00;
	}
	
	if ((READ_D_GEAR == 0) &&(READ_R_GEAR == 0)) 
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

	/*����ʹ���ź�*/
	if (READ_ACCELERATOR == 0x01)
	{
		ACCELERATOR_ENABLE = ON;
	}
	else
	{
		ACCELERATOR_ENABLE = OFF;
	}
}



/* ====================================================================================================================
**  Function Name    :    Signal output 
**  Process Timing   :    Any Time
**  Summary          :    -
**  Arguments[Input] :    ValControl
**  Arguments[Output]:    
**  Return Value     :    -
==================================================================================================================== */

void IOControl(void)
{
	/*�����ȿ���*/
	if (g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal == 0x01)
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
		REVERSING_LIGHT = ON;
	}
}



/* ====================================================================================================================
**  Function Name    :    Anysis can and control The light
**  Process Timing   :    Any  time
**  Summary          :    -
**  Arguments[Input] :    
**  Arguments[Output]:    
**  Return Value     :    -
==================================================================================================================== */

void CanControl(void)
{		

	
	//if(g_StructGlobalFlag.bits.RecvVCU5Flag == 1)
	{
		//g_StructGlobalFlag.bits.RecvVCU5Flag = 0;
		//�����ȿ���
		if(g_VCU5RecvVal.LIGHTSTATUS.bits.b_Klaxon == 0x01)   
		{
			KLAXON_SWITCH = ON;
		}
		else
		{
			KLAXON_SWITCH = OFF;
		}

		//��λ����
		if (g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x00) //neutral gear �յ�
		{
			#if 0
			D_GEAR = OFF;
			R_GEAR = OFF;
			#else
			D_GEAR = ON;
			R_GEAR = ON;
			#endif
			//printf("neutral gear\r\n");
		}
		else if(g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x01)//forward gear ǰ����
	    {
	    	#if 0
	    	D_GEAR = ON;
			R_GEAR = OFF;   
			#else
	    	D_GEAR = OFF;
			R_GEAR = ON;   
			#endif
			//printf("forward gear\r\n");
		}
		else if(g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x02)//reserve gear ���˵�
		{
			#if 0
	    	D_GEAR = OFF;
			R_GEAR = ON;  
			#else
	    	D_GEAR = ON;
			R_GEAR = OFF;  

			#endif
			//printf("reserve gear\r\n");
		}
		else
		{
			/*INVALID*/
			printf("INVALID gear\r\n");
		}

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

		if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_BeamLight == 0x01)
		{
			BEAN_LIGHT = ON;		
		}
		else 
		{
			BEAN_LIGHT = OFF;		
		}

		if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_TurnLeftLight == 0x01)
		{
			TURN_LEFT_LIGHT = ON;
		}
		else
		{
			TURN_LEFT_LIGHT = OFF;
		}
		
		if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_TurnRightLight == 0x01)
		{
			TURN_RIGHT_LIGHT = ON;
		}
		else
		{
			TURN_RIGHT_LIGHT = OFF;
		}
	}
}

void IO_task(void)
{	
	CheckIo();
	
	IOControl();
	
	if(g_StructBCMStatus.DrivingMode == AUTOMATIC_DRIVING)
	{
		CanControl();
	}
}
