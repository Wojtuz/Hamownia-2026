#include "enums.h"
#include "main.h"
#include "stm32l0xx_hal_uart.h"
#include <sys/types.h>

extern UART_HandleTypeDef huart5;

void TransmitMessageDMA(struct Message *msg);
void CreateMessage(struct Message *msg, MsgID id, uint8_t *data);
void CreateMessage8(struct Message *msg, MsgID id, uint8_t data);
void CreateMessage16(struct Message *msg, MsgID id, uint16_t data);
void CreateMessage32(struct Message *msg, MsgID id, uint32_t data);
