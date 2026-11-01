#include "stm32f10x.h"
#include <string.h>
#include "CAN.h"

void MyCAN_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	// config gpio
	GPIO_InitTypeDef GPIO_Config;
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Config.GPIO_Pin = GPIO_Pin_12;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Config);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Config.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_Config);
	
	// config can periph
	CAN_InitTypeDef CAN_Congfig;
	CAN_Congfig.CAN_ABOM = DISABLE;
	CAN_Congfig.CAN_AWUM = DISABLE;
	CAN_Congfig.CAN_BS1  = CAN_BS1_2tq;
	CAN_Congfig.CAN_BS2  = CAN_BS2_3tq;
	CAN_Congfig.CAN_Mode = CAN_Mode_Normal;
	CAN_Congfig.CAN_NART = DISABLE;
	CAN_Congfig.CAN_Prescaler = 48;     // Baund = 36M / 48 / (1 + 2 + 3) = 125K
	CAN_Congfig.CAN_RFLM = DISABLE;
	CAN_Congfig.CAN_SJW  = CAN_SJW_2tq;
	CAN_Congfig.CAN_TTCM = DISABLE;
	CAN_Congfig.CAN_TXFP = DISABLE;
	CAN_Init(CAN1, &CAN_Congfig);
	
	// config filter -> 接收任意帧
	CAN_FilterInitTypeDef Filter_Config;
	Filter_Config.CAN_FilterActivation = ENABLE;
	Filter_Config.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	Filter_Config.CAN_FilterIdHigh     = 0x0000;
	Filter_Config.CAN_FilterIdLow      = 0x0000;
	Filter_Config.CAN_FilterMaskIdHigh = 0x0000;
	Filter_Config.CAN_FilterMaskIdLow  = 0x0000;
	Filter_Config.CAN_FilterMode	   = CAN_FilterMode_IdMask;  // 屏蔽模式
	Filter_Config.CAN_FilterNumber	   = 0;
	Filter_Config.CAN_FilterScale 	   = CAN_FilterScale_32bit;
	CAN_FilterInit(&Filter_Config);
}

void CAN_TX(CanTxMsg *TxMessage)
{
	uint32_t Timeout = 100000;
	uint8_t mailbox = CAN_Transmit(CAN1, TxMessage);
	
	while(CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok)
	{
		if(--Timeout == 0)
			break;
	}
}

uint8_t CAN_ReceiveCheck(void)
{
	if(CAN_MessagePending(CAN1, CAN_FIFO0) > 0) // 检查挂起消息的数量
		return 1;
	return 0;
}

// 查询接收
void CAN_RX(CanRxMsg *RxMessage)
{
	CAN_Receive(CAN1, CAN_FIFO0, RxMessage);
}
