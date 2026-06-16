#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"
#include <stdint.h>
#include "libVescCan/VESC_Convert.h"

void FDCAN_Config(FDCAN_HandleTypeDef *hfdcan);
void CAN_TransmitOverCan(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len);

void CAN_TransmitVescCommand(FDCAN_HandleTypeDef *hfdcan, VESC_Id_t vescID, VESC_Command_t command, float value);

#endif /* INC_CAN_H_ */