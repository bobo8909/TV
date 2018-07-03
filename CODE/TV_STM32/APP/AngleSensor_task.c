#include "AngleSensor_task.h"


/******************************************************
*四路PWM的频率：
*	 转角S：TIM4(200Hz)
*	 转角P: TIM2(1KHz)
*    扭矩1：TIM3(2KHz)
*	 扭矩2: TIM3(2KHz)
******************************************************/
#define TIM_SEND_RESOLUTION 10000
/**
  * @brief  处理四路PWM输入输出的任务，把从IO口读取的波形PWM占空比按照相同的占空比从相应的IO口输出
  *         
  * @param  None
  *   
  * @retval None
  */
	int DutyCycleTIM4 = 0;
void AngleSensor_task(void)
{
	#if 1
	static u32 PWMVal[4] = {0};
	
	static u32 BackupPWM3OUT2 = 0;
	static u32 BackupPWM4OUT2 = 0;
	static u32 BackupPWM2OUT4 = 0;
	static u32 BackupPWM3OUT4 = 0;
    static u32 BackupPWM5OUT2 = 0;
//    static u8 i = 0;
	u32 SendVal = 0;

    
    //TIM_SetCompare1(TIM3, 18000); 
    //TIM_SetCompare3(TIM3, 18000); 
/*
*已根据实际的频率进行了修改，把读取的占空比按照CAN协议发送出去
*/
    //printfTIM5();
	//if(g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
	#if 1
    
    if ((BackupPWM2OUT4 != PWM2OUT4) || (BackupPWM4OUT2 != PWM4OUT2))
	{
		BackupPWM2OUT4 = PWM2OUT4;	
		BackupPWM4OUT2 = PWM4OUT2;		
		//PWM2OUT4 = 0;
		//PWM4OUT2 = 0;

		//TIM_SetCompare1(TIM4, BackupPWM4OUT2);	
		//printf("S = %d\r\n", BackupPWM4OUT2);
		printf("S = %d\r\n", (BackupPWM4OUT2 * 10000 / ARR_200Hz));
		
		//TIM_SetCompare3(TIM2, BackupPWM2OUT4);
		
		printf("P = %d\r\n", 10000 - (BackupPWM2OUT4 * 10000 / ARR_1KHz));
        
		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == AUTOMATIC_DRIVING)
		{		
			SendVal = BackupPWM2OUT4 * TIM_SEND_RESOLUTION / ARR_1KHz ;
            SendVal = 10000 - SendVal; 
			g_BCM2SendVal.AngleSensorSignalPHigh = SendVal >> 8;
			g_BCM2SendVal.AngleSensorSignalPLow = (u8)(SendVal & 0xFF);

            
			SendVal = BackupPWM4OUT2 * TIM_SEND_RESOLUTION / ARR_200Hz;
			g_BCM2SendVal.AngleSensorSignalSHigh = SendVal >> 8;
			g_BCM2SendVal.AngleSensorSignalSLow = (u8)(SendVal & 0xFF);
		}
    }
    #else
	/*转角P*/
    if (BackupPWM2OUT4 != PWM2OUT4) 
	{
		BackupPWM2OUT4 = PWM2OUT4;	
		PWM2OUT4 = 0;
		
		//TIM_SetCompare3(TIM2, BackupPWM2OUT4);
		
		printf("P = %d\r\n", 10000 - (BackupPWM2OUT4 * 10000 / ARR_1KHz));
		
		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == AUTOMATIC_DRIVING)
		{		
			SendVal = BackupPWM2OUT4 * TIM_SEND_RESOLUTION / ARR_1KHz ;
            SendVal = 10000 - SendVal; 
			g_BCM2SendVal.AngleSensorSignalPHigh = SendVal >> 8;
			g_BCM2SendVal.AngleSensorSignalPLow = (u8)(SendVal & 0xFF);
		}
	}

	/*转角S*/
	if(BackupPWM4OUT2 != PWM4OUT2)
	{
		BackupPWM4OUT2 = PWM4OUT2;		
		PWM4OUT2 = 0;

		//TIM_SetCompare1(TIM4, BackupPWM4OUT2);	
		//printf("S = %d\r\n", BackupPWM4OUT2);
		printf("S = %d\r\n", (BackupPWM4OUT2 * 10000 / ARR_200Hz));

		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == AUTOMATIC_DRIVING)
		{
			SendVal = BackupPWM4OUT2 * TIM_SEND_RESOLUTION / ARR_200Hz;
			
			g_BCM2SendVal.AngleSensorSignalSHigh = SendVal >> 8;
			g_BCM2SendVal.AngleSensorSignalSLow = (u8)(SendVal & 0xFF);
		}
	}
#endif
	/*扭矩1*/
#if 1    
    #if 1
	if (BackupPWM3OUT2 != PWM3OUT2) 
    #else
    if (PWM3OUT2Flag == 1)
    #endif
    {
        PWM3OUT2Flag = 0;
    #if 1
        //if (((PWM3OUT2 - (int)BackupPWM3OUT2) > 10) ||((PWM3OUT2 - (int)BackupPWM3OUT2) < 0))
		{
		//BackupPWM3OUT2 = BackupPWM3OUT2 + 10;		
		//BackupPWM3OUT2 = PWM3OUT2 + 100;		
		BackupPWM3OUT2 = PWM3OUT2;
		PWM3OUT2 = 0;
		printf("T1 = %d\r\n", BackupPWM3OUT2*10000/ARR_2KHz);	
//		printf("T1 = %d\r\n", BackupPWM3OUT2);	

		/*有人模式时输出PWM波，无人模式时把采集的数据发送给VCU*/
		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
		{
		    #if 1
			//TIM_SetCompare1(TIM3, BackupPWM3OUT2);
            //TIM_SetCompare3(TIM3, ARR_2KHz - BackupPWM3OUT2);
            #else
			TIM_SetCompare1(TIM3, ARR_2KHz -BackupPWM3OUT2);
            TIM_SetCompare3(TIM3, BackupPWM3OUT2);

            #endif
		}
		else
		{
			SendVal = BackupPWM3OUT2 * ARR_2KHz / TIM_SEND_RESOLUTION ;
			
			g_BCM2SendVal.EPSMomentalSignal1High = SendVal >> 8;
			g_BCM2SendVal.EPSMomentalSignal1Low = (u8)(SendVal & 0xFF);
		}

            }
#if 0
        else
            {
            //BackupPWM3OUT2 = BackupPWM3OUT2 - 100;            
            printf("T11 = %d\r\n", BackupPWM3OUT2);  
			TIM_SetCompare1(TIM3, BackupPWM3OUT2-10 );
            TIM_SetCompare3(TIM3, ARR_2KHz - BackupPWM3OUT2+10 );

        }
        #endif
       #endif
	}
//	else
//	{
//		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
//		{
//			//TIM_SetCompare1(TIM3, 18000);	
//            //TIM_SetCompare3(TIM3, 18000);	
//			//TIM_SetCompare1(TIM3, 21600);//60%
//		}
//	}
#endif
#if 1
	/*扭矩2*/
//	if(BackupPWM3OUT4 != PWM3OUT4)
    if(BackupPWM5OUT2 != PWM5OUT2)
	{
	    #if 0
		//BackupPWM3OUT4 = PWM3OUT4 + 900;	
		BackupPWM3OUT4 = PWM3OUT4;
		PWM3OUT4 = 0;
		printf("T2 = %d\r\n", BackupPWM3OUT4*10000/ARR_2KHz);
//		printf("T2 = %d\r\n", BackupPWM3OUT4);
		#else
		BackupPWM5OUT2 = PWM5OUT2;
		PWM5OUT2 = 0;
//        printf("T2 = %d\r\n", BackupPWM5OUT2);
		printf("T2 = %d\r\n", BackupPWM5OUT2*10000/ARR_2140Hz);

        #endif
		/*有人模式时输出PWM波，无人模式时把采集的数据发送给VCU*/
		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
		{
			//TIM_SetCompare3(TIM3, BackupPWM3OUT4);	
		}
		else
		{
			SendVal = BackupPWM3OUT4 * ARR_2KHz / TIM_SEND_RESOLUTION;
			
			g_BCM2SendVal.EPSMomentalSignal2High = SendVal >> 8;
			g_BCM2SendVal.EPSMomentalSignal2Low = (u8)(SendVal & 0xFF);
		}
	}
//	else
//	{		
//		if (g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == MANNED)
//		{
//	//		TIM_SetCompare3(TIM3, 18000);
//			//TIM_SetCompare3(TIM3, 14400);//40%
//		}
//	}
#endif
	
	if(g_VCU5RecvVal.DRIVING_MODE.bits.b_DrivingMode == AUTOMATIC_DRIVING)
	{
		//PWMVal[0] = ((u32)g_VCU3RecvVal.AngleSensorSigSHigh << 8 | g_VCU3RecvVal.AngleSensorSigSLow) * ARR_200Hz / TIM_SEND_RESOLUTION;
		//PWMVal[1] = ((u32)g_VCU3RecvVal.AngleSensorSigPHigh << 8 | g_VCU3RecvVal.AngleSensorSigPLow) * ARR_1KHz / TIM_SEND_RESOLUTION  ;
		PWMVal[2] = ((u32)g_VCU3RecvVal.EPSMomentalSig1HIGH << 8 | g_VCU3RecvVal.EPSMomentalSig1LOW) * ARR_2KHz / TIM_SEND_RESOLUTION;
		PWMVal[3] = ((u32)g_VCU3RecvVal.EPSMomentalSig2HIGH << 8 | g_VCU3RecvVal.EPSMomentalSig2LOW) * ARR_2KHz / TIM_SEND_RESOLUTION;
		
		//TIM_SetCompare1(TIM4, PWMVal[0]);		   
		//TIM_SetCompare3(TIM2, PWMVal[1]);		   
		TIM_SetCompare1(TIM3, PWMVal[2]);		   
		TIM_SetCompare3(TIM3, PWMVal[3]);	
		
		//printf("PWMVal0 = %d\r\n", PWMVal[0]);
		//printf("PWMVal1 = %d\r\n", PWMVal[1]);
		printf("PWMVal2 = %d\r\n", PWMVal[2]);
		printf("PWMVal3 = %d\r\n", PWMVal[3]);
	}
#endif

	#if 0
	if(TIM4Flag == 1)
	{
		//DutyCycleTIM4 = iHeightCountTIM4 * 100 / iPeriodTIM4;
		//printf("PE7 : %d\n", GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
		printf("iHeightCount:%d\r\n",iHeightCountTIM4+1);
		printf("iPeriod:%d\r\n",iPeriodTIM4+1);
		printf("DutyCycle : %02d.%02d%%\r\n", (iHeightCountTIM4+1) * 100 / (iPeriodTIM4+1), (iHeightCountTIM4+1) * 10000 / (iPeriodTIM4+1) %100);
		DutyCycleTIM4 = 0;
		iHeightCountTIM4 = 0;
		iPeriodTIM4 = 0;
		TIM4Flag = 0;
	}
    #endif
	//delay_ms(100);
}

