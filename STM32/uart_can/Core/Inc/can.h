#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

void FDCAN_Config(FDCAN_HandleTypeDef *hfdcan);
void TransmitOverCan(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len);

void TransmitVescCommand(FDCAN_HandleTypeDef *hfdcan, VESC_Id_t vescID, VESC_Command_t command, float value);

#endif /* INC_CAN_H_ */