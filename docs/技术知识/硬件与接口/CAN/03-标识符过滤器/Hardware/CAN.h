#ifndef __CAN_H
#define __CAN_H

void MyCAN_Init(void);
void CAN_TX(CanTxMsg *TxMessage);
uint8_t CAN_ReceiveCheck(void);
void CAN_RX(CanRxMsg *RxMessage);

#endif
