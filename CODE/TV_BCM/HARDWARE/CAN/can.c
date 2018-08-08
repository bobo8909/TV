#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

u8 u8_StartOutput = 0;//������ECU���������־λ 0����ͣ����� 1���������
CANRXBUF CANRecvDataBuf[3] = {0, {0}, 0};
CANRXBUF CANRecvBCMBuf = {0, {0}, 0};
CANRXBUF CANRecvEBSBuf = {0, {0}, 0};

STRUCT_VCU_2 g_VCU2RecvVal = {0};
STRUCT_VCU_3 g_VCU3RecvVal = {0};
STRUCT_VCU_5 g_VCU5RecvVal = {0};
STRUCT_BCM_1 g_RecvBCM1Val = {0};

STRUCT_BCM_1 g_BCM1SendVal = {0};
STRUCT_BCM_2 g_BCM2SendVal = {0};
STRUCT_BCM_3 g_BCM3SendVal = {0};
STRUCT_EBS_1 g_EBS1RecvVal = {0};

STRUCT_CAN_ERR_FLAG g_CANErrFlag = {0};



									  
//////////////////////////////////////////////////////////////////////////////////
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��;


u8 CanModeInit(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��IO
	  
 	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM = DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
	CAN_InitStructure.CAN_ABOM = DISABLE;						 //����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM = DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART = DISABLE;						 	//�����Զ����� //
  	CAN_InitStructure.CAN_RFLM = DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP = DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode = mode;	         //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //
  	//���ò�����
  	CAN_InitStructure.CAN_SJW = tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1 = tbs1; //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2 = tbs2;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler = brp;            //��Ƶϵ��(Fdiv)Ϊbrp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // ��ʼ��CAN1 

	CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //������0
   	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;//CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)CANID_VCU_2 << 3) >> 16) & 0xFFFF;
  	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)CANID_VCU_2 << 3) & 0xFFFF) | CAN_Id_Extended | CAN_RTR_Data;
  	//CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((u32)CANID_VCU_3 << 3) >> 16) & 0xFFFF;
  	//CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((u32)CANID_VCU_3 << 3) & 0xFFFF) | CAN_Id_Extended | CAN_RTR_Data;;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��


	CAN_FilterInitStructure.CAN_FilterNumber = 1;	  //������1
   	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;//CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)CANID_VCU_5 << 3) >> 16) & 0xFFFF;
  	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)CANID_VCU_5 << 3) & 0xFFFF) | CAN_Id_Extended | CAN_RTR_Data;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((u32)CANID_EBS_1 << 3) >> 16) & 0xFFFF;
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((u32)CANID_EBS_1 << 3) & 0xFFFF) | CAN_Id_Extended | CAN_RTR_Data;;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������1������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //���������1
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
	
#if CAN_RX0_INT_ENABLE

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.	
	
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


	CAN_ITConfig(CAN1,CAN_IT_EWG|CAN_IT_EPV|CAN_IT_BOF|CAN_IT_LEC|CAN_IT_ERR,ENABLE);

  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	
  	CAN_Receive(CAN1, 0, &RxMessage);
	
	if(RxMessage.ExtId == CANID_VCU_2)
	{
		memcpy(CANRecvDataBuf[0].Buf,RxMessage.Data,RxMessage.DLC);
		
		CANRecvDataBuf[0].NewDataFlag = 1;
	}
    #if 0
	if(RxMessage.ExtId == CANID_VCU_3)
	{
		memcpy(CANRecvDataBuf[1].Buf,RxMessage.Data,RxMessage.DLC);
		
		CANRecvDataBuf[1].NewDataFlag = 1;
	}
	#endif
	if(RxMessage.ExtId == CANID_VCU_5)
	{
		memcpy(CANRecvDataBuf[2].Buf,RxMessage.Data,RxMessage.DLC);
		
		CANRecvDataBuf[2].NewDataFlag = 1;
	}

	if(RxMessage.ExtId == CANID_EBS_1)
	{
		memcpy(CANRecvEBSBuf.Buf,RxMessage.Data,RxMessage.DLC);
		
		CANRecvEBSBuf.NewDataFlag = 1;
	}
}
#endif

//can����һ������
//u32CanId : ����֡��can id;
//u8len:���ݳ���(���Ϊ8);				     
//ptrMsg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CanSendMsg(uint32_t u32CanId, u8* ptrMsg, u8 u8Len)
{	
  u8 mbox;
  u16 i = 0;
  CanTxMsg TxMessage;
  TxMessage.StdId = 0;					 // ��׼��ʶ�� 
  TxMessage.ExtId = u32CanId;				   // ������չ��ʾ�� 
  TxMessage.IDE = CAN_Id_Extended; // ��չ֡
  TxMessage.RTR = CAN_RTR_Data;		 // ����֡
  TxMessage.DLC = u8Len;						// Ҫ���͵����ݳ���
  for(i = 0; i < u8Len; i++)
  TxMessage.Data[i] = ptrMsg[i];			          
  mbox = CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed)&&(i<0XFFF)) //�ȴ����ͽ���
  {
	  i++;	
  }
  if(i>=0XFFF)return 1;
  return 0;		

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CanReceiveMsg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

void CAN1_SCE_IRQHandler(void)
{
	
	unsigned char Rx_Error_Cnt;
	unsigned char Tx_Error_Cnt;
	unsigned char Last_Error;
	
	if(CAN_GetITStatus(CAN1, CAN_IT_EWG) == SET) 	   //���󾯸��ж�
	{
		Tx_Error_Cnt = CAN_GetLSBTransmitErrorCounter(CAN1);
		Rx_Error_Cnt = CAN_GetReceiveErrorCounter(CAN1);
		
		if((Tx_Error_Cnt >= 96) || (Rx_Error_Cnt >= 96))
		{
			g_CANErrFlag.Byte1.bits.CAN_ErrorWarning_Flag = 1;
		}

		CAN_ClearITPendingBit(CAN1, CAN_IT_EWG);
	}
	
	if(CAN_GetITStatus(CAN1, CAN_IT_EPV) == SET) 	   //���󱻶��ж�
	{
		Tx_Error_Cnt = CAN_GetLSBTransmitErrorCounter(CAN1);
		Rx_Error_Cnt = CAN_GetReceiveErrorCounter(CAN1);
		
		if((Tx_Error_Cnt > 127)||(Rx_Error_Cnt > 127))
		{
			g_CANErrFlag.Byte1.bits.CAN_ErrorPassive_Flag = 1;
		}
		CAN_ClearITPendingBit(CAN1, CAN_IT_EPV); 	   
	}
	
	if(CAN_GetITStatus(CAN1, CAN_IT_BOF) == SET) 	   //�����ж�
	{
		g_CANErrFlag.Byte1.bits.CAN_ErrorBusoff_Flag = 1;
		
		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);
	}
	
	if(CAN_GetITStatus(CAN1, CAN_IT_LEC) == SET) 	   //�ϴδ�����ж�
	{
		Last_Error = CAN_GetLastErrorCode(CAN1);
		if(Last_Error == 0x1) //λ������
		{
			g_CANErrFlag.Byte0.bits.CAN_ErrorCode_StuffErr_Flag = 0x01;
		}
		if(Last_Error == 0x2) //��ʽ����
		{
			g_CANErrFlag.Byte0.bits.CAN_ErrorCode_FormErr_Flag = 0x01;
		}
		if(Last_Error == 0x3) //ȷ��(ACK)����
		{
			g_CANErrFlag.Byte0.bits.CAN_ErrorCode_ACKErr_Flag = 0x01;
		}
		if(Last_Error == 0x4) //����λ����
		{
			g_CANErrFlag.Byte0.bits.CAN_ErrorCode_BitRecessiveErr_Flag = 0x01;
		}
		if(Last_Error == 0x5) //����λ����
		{
			g_CANErrFlag.Byte0.bits.CAN_ErrorCode_BitDominantErr_Flag = 0x01;
		}
		if(Last_Error == 0x6) //CRC����
		{
			g_CANErrFlag.Byte0.bits.CAN_ErrorCode_CRCErr_Flag = 0x01;
		}
		CAN_ClearITPendingBit(CAN1, CAN_IT_LEC);
	}

	/*���ﻹ��ȷ��ʲôʱ���ܹ�����*/
	//if(CAN_GetITStatus(CAN1, CAN_IT_ERR) == SET) 	   //�����ж�
	//{
	//	CanModeInit(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 24, CAN_Mode_Normal); //CAN�ڵ�����
	//	CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);
	//}
}


void CANErrPrintf(void)
{
	if(g_CANErrFlag.Byte1.bits.CAN_ErrorWarning_Flag == 0x01)
	{
		g_CANErrFlag.Byte1.bits.CAN_ErrorWarning_Flag = 0;
		printf("CAN_IT_EWG\r\n");	
	}

	if(g_CANErrFlag.Byte1.bits.CAN_ErrorPassive_Flag == 0x01)
	{
		g_CANErrFlag.Byte1.bits.CAN_ErrorPassive_Flag  = 0;
		printf("CAN_IT_EPV\r\n"); 	 
	}

	if(g_CANErrFlag.Byte1.bits.CAN_ErrorBusoff_Flag == 0x01)
	{
		g_CANErrFlag.Byte1.bits.CAN_ErrorBusoff_Flag = 0;
		
		CanModeInit(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 24, CAN_Mode_Normal);//CAN�ڵ�����
		printf("busoff\r\n"); 
	}


	if(g_CANErrFlag.Byte0.bits.CAN_ErrorCode_StuffErr_Flag == 0x01) //λ������
	{
		g_CANErrFlag.Byte0.bits.CAN_ErrorCode_StuffErr_Flag = 0;
		printf("CAN_ErrorCode_StuffErr_Flag\r\n");
	}
	if(g_CANErrFlag.Byte0.bits.CAN_ErrorCode_FormErr_Flag == 0x01) //��ʽ����
	{
		g_CANErrFlag.Byte0.bits.CAN_ErrorCode_FormErr_Flag = 0;
		printf("CAN_ErrorCode_FormErr\r\n");
	}
	if(g_CANErrFlag.Byte0.bits.CAN_ErrorCode_ACKErr_Flag == 0x01) //ȷ��(ACK)����
	{
		g_CANErrFlag.Byte0.bits.CAN_ErrorCode_ACKErr_Flag = 0;
		printf("CAN_ErrorCode_ACKErr\r\n");
	}
	if(g_CANErrFlag.Byte0.bits.CAN_ErrorCode_BitRecessiveErr_Flag == 0x01) //����λ����
	{
		g_CANErrFlag.Byte0.bits.CAN_ErrorCode_BitRecessiveErr_Flag = 0;
		printf("CAN_ErrorCode_BitRecessiveErr\r\n");
	}
	if(g_CANErrFlag.Byte0.bits.CAN_ErrorCode_BitDominantErr_Flag == 0x01) //����λ����
	{
		g_CANErrFlag.Byte0.bits.CAN_ErrorCode_BitDominantErr_Flag = 0;
		printf("CAN_ErrorCode_BitDominantErr\r\n");
	}
	if(g_CANErrFlag.Byte0.bits.CAN_ErrorCode_CRCErr_Flag == 0x01) //CRC����
	{
		g_CANErrFlag.Byte0.bits.CAN_ErrorCode_CRCErr_Flag = 0;
		printf("CAN_ErrorCode_CRCErr\r\n");
	}


}
