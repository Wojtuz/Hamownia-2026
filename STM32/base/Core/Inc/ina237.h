#ifndef INC_INA237_H_
#define INC_INA237_H_

#include "main.h"
#include <stdint.h>

uint16_t INA237_ReadRegister(uint16_t devAddr, uint8_t regAddr);
void INA237_WriteRegister(uint16_t devAddr, uint8_t regAddr, uint16_t value);
void INA237_InitDevice(uint16_t devAddr);

#endif /* INC_INA237_H_ */