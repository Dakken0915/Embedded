#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CAN.h"
#include "Key.h"

CanTxMsg TxMsgArray[] = {
/*  StdId     ExtId         IDE            RTR        DLC        Data   */
	{0x555, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x12345678, CAN_Id_Extended, CAN_RTR_Data, 4, {0xAA, 0xBB, 0xCC, 0xDD}},
	{0x555, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x00, 0x00, 0x00, 0x00}},
	{0x555, 0x0789ABCD, CAN_Id_Extended, CAN_RTR_Remote, 4, {0x00, 0x00, 0x00, 0x00}}
};

CanRxMsg RxMsg;

uint8_t Msg_index;
uint8_t KetNum;

int main(void)
{
	OLED_Init();
	Key_Init();
	MyCAN_Init();

	OLED_ShowString(1, 1, " RX :");
	OLED_ShowString(2, 1, "RXID:");
	OLED_ShowString(3, 1, "Len :");
	OLED_ShowString(4, 1, "Data:");
	
	while(1)
	{
		KetNum = Key_GetNum();
		if(KetNum == 1)
		{
			CAN_TX(&TxMsgArray[Msg_index++]);
			if(Msg_index >= sizeof(TxMsgArray) / sizeof(TxMsgArray[0]))
				Msg_index = 0;
		}
		
		if(CAN_ReceiveCheck())
			CAN_RX(&RxMsg);
		
		if(RxMsg.IDE == CAN_Id_Standard)
		{
			OLED_ShowString(1, 6, "Std");
			OLED_ShowHexNum(2, 6, RxMsg.StdId, 8);
		}
		else if(RxMsg.IDE == CAN_Id_Extended)
		{
			OLED_ShowString(1, 6, "Ext");
			OLED_ShowHexNum(2, 6, RxMsg.ExtId, 8);
		}

		if(RxMsg.RTR == CAN_RTR_Data)
		{
			OLED_ShowString(1, 10, "Data  ");
			OLED_ShowHexNum(3, 6, RxMsg.DLC, 1);
			OLED_ShowHexNum(4, 6, RxMsg.Data[0], 2);
			OLED_ShowHexNum(4, 9, RxMsg.Data[1], 2);
			OLED_ShowHexNum(4, 12, RxMsg.Data[2], 2);
			OLED_ShowHexNum(4, 15, RxMsg.Data[3], 2);
		}
		else if(RxMsg.RTR == CAN_RTR_Remote)
		{
			OLED_ShowString(1, 10, "Remote");
			OLED_ShowHexNum(3, 6, RxMsg.DLC, 1);
			OLED_ShowHexNum(4, 6, 0x00, 2);
			OLED_ShowHexNum(4, 9, 0x00, 2);
			OLED_ShowHexNum(4, 12, 0x00, 2);
			OLED_ShowHexNum(4, 15, 0x00, 2);
		}

	}
}
