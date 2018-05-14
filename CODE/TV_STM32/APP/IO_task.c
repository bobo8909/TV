#include "IO_task.h"



DRIVING_MODE g_DrivingMode;

/* ====================================================================================================================
**  Function Name    :    driving mode detect
**  Process Timing   :    Any Time
**  Summary          :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    
**  Return Value     :    driving mode // 0:manned  1:automatic
==================================================================================================================== */
DRIVING_MODE CheckDrivingMode(void)
{
	DRIVING_MODE DrivingMode;
	if (READ_DRIVING_MODE == 1) 	   //automatic driving switch detect
	{		
		DrivingMode = AUTOMATIC_DRIVING;
	}
	else
	{
		DrivingMode = MANNED_DRIVING;
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
	if(g_DrivingMode  == MANNED_DRIVING)
	{
		
	}
	
	/*************************************	
		���ﻹ��һ��ת�Ǵ������쳣���ж�
	*************************************/
		
	if (READ_KLAXON_SWITCH == 0x00)       //�����ȿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.KlaxonSignal = 0x01;
	}

	if (READ_LIGHT_TURN_RIGHT == 0x00)   //��ת�ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnRightSignal = 0x01;
	}
	
	if (READ_LIGHT_TURN_LEFT == 0x00)    //��ת�ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.TurnLeftSignal = 0x01;
	}

	if (READ_BEAN_LIGHT == 0x00)    //Զ��ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.BeamlightSignal = 0x01;
	}

	if (READ_DIPPED_HEADLIGHT == 0x00)     //����ƿ��ؼ��
	{
		g_BCM1SendVal.LIGHTSTATUS.bits.DippedLightSignal = 0x01;
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

	/*��������ģʽ��ʱ���ɼ�������ļ����ź�ʱ��������Ӧ�Ŀ��ƶ���*/
	if(g_DrivingMode == AUTOMATIC_DRIVING)
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
	if (g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x00)
	{
		R_GEAR = OFF;
		D_GEAR = OFF;
	}
	else if(g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x01)
	{
		D_GEAR = ON;
		R_GEAR = OFF;
	}
	else if(g_BCM1SendVal.DRIVINGSTATUS.bits.GearStatusSignal == 0x02)
	{
		D_GEAR = OFF;
		R_GEAR = ON;
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
			D_GEAR = OFF;
			R_GEAR = OFF;

			//printf("neutral gear\r\n");
		}
		else if(g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x01)//forward gear ǰ����
	    {
	    	D_GEAR = ON;
			R_GEAR = OFF;   
			printf("forward gear\r\n");
		}
		else if(g_VCU2RecvVal.CONTROLSIGNAL.bits.b_GearsControlSig == 0x02)//reserve gear ���˵�
		{
	    	D_GEAR = OFF;
			R_GEAR = ON;   
			printf("reserve gear\r\n");
		}
		else
		{
			/*INVALID*/
			printf("INVALID gear\r\n");
		}

		if (g_VCU5RecvVal.LIGHTSTATUS.bits.b_DippedHeadlight == 0x01)
		{
			DIPPED_HEADLIGHT = ON;
			printf("DIPPED_HEADLIGHT\r\n");
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

	CanControl();
}
