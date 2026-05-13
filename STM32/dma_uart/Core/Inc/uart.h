#include "enums.h"
#include "main.h"
#include "stm32l0xx_hal_uart.h"
#include <sys/types.h>

extern UART_HandleTypeDef huart5;

void TransmitMessageDMA(struct Message *msg);