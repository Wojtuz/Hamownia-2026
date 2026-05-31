#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

void FDCAN_Config(FDCAN_HandleTypeDef *hfdcan);
void TransmitOverCan(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len);

#endif /* INC_CAN_H_ */