#ifndef __CAN_H
#define __CAN_H

void MyCAN_Init(void);
void CAN_TX(uint32_t ID, uint8_t length, uint8_t *Data);
uint8_t CAN_ReceiveCheck(void);
void CAN_RX(uint32_t *ID, uint8_t *length, uint8_t *Data);

#endif
