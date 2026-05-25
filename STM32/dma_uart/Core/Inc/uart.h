#include "enums.h"
#include "main.h"
#include "stm32l0xx_hal_uart.h"
#include <sys/types.h>

#define UART_RX_BUFFER_SIZE 16
#define UART_TX_BUFFER_SIZE 16
#define UART2_TX_BUFFER_SIZE 255
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart2;

void logDebug(const char *msg, uint8_t size);

/// Message creation and transmission functions
void TransmitMessageDMA(struct Message *msg);
void CreateMessage(struct Message *msg, MsgID id, uint8_t *data);
void CreateMessage8(struct Message *msg, MsgID id, uint8_t data);
void CreateMessage16(struct Message *msg, MsgID id, uint16_t data);
void CreateMessage32(struct Message *msg, MsgID id, uint32_t data);

/// Message receive callback
uint8_t getBufferPosToWrite(uint8_t wannaWrite);
void HandleIncomingMessage(struct Message *msg);
