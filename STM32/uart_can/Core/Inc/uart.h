#include "enums.h"
#include "main.h"
#include "stm32g4xx_hal_uart.h"
#include <sys/types.h>

#define UART_RX_BUFFER_SIZE 16
#define UART_TX_BUFFER_SIZE 16
#define UART2_TX_BUFFER_SIZE 255

void UART_logDebug(UART_HandleTypeDef *huart, const char *msg, uint8_t size);

/// Message creation and transmission functions
void UART_TransmitMessageDMA(UART_HandleTypeDef *huart, struct Message *msg);
void UART_CreateMessage(struct Message *msg, MsgID id, uint8_t *data);
void UART_CreateMessage8(struct Message *msg, MsgID id, uint8_t data);
void UART_CreateMessage16(struct Message *msg, MsgID id, uint16_t data);
void UART_CreateMessage32(struct Message *msg, MsgID id, uint32_t data);

/// Message receive callback
uint8_t getBufferPosToWrite(uint8_t wannaWrite);
void UART_HandleIncomingMessage(UART_HandleTypeDef *huart, FDCAN_HandleTypeDef *hfdcan, struct Message *msg);
void UART_StartReceiveDMA(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx);
void UART_ProcessRxDmaBuffer(UART_HandleTypeDef *huart, FDCAN_HandleTypeDef *hfdcan, DMA_HandleTypeDef *hdma_rx);
