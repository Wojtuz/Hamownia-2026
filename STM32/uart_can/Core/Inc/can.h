#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

void FDCAN_Config(FDCAN_HandleTypeDef *hfdcan);
void CAN_tx();

#endif /* INC_CAN_H_ */