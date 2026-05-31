#include "uart.h"
#include <stdint.h>

uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
uint8_t tx_buffer2[UART2_TX_BUFFER_SIZE];

volatile uint8_t is_transmitting = 0;
volatile uint8_t is_transmitting2 = 0;

void logDebug(const char *msg, uint8_t size)
{
    while (is_transmitting2); // Czekaj, aż poprzedni transfer się skończy
    for (uint8_t i = 0; i < size; i++)
    {
        tx_buffer2[i] = msg[i];
    }
    is_transmitting2 = 1;
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)msg, size);
}

void TransmitMessageDMA(struct Message *msg)
{
    // Czekaj, aż poprzedni transfer się skończy
    while (is_transmitting);
    
    // Sprawdź czy rozmiar wiadomości się zmieści
    uint8_t total_size = 2 + msg->size; // 1 byte start, 1 byte type, size bytes data
    if (total_size > UART_TX_BUFFER_SIZE)
    {
        return; // Bufor za mały
    }

    // Przygotuj dane w buforze statycznym
    tx_buffer[0] = 0xAA; // Start byte
    tx_buffer[1] = msg->ID; // Message ID
    for (uint8_t i = msg->size; i > 0; i--)
    {
        tx_buffer[1 + i] = msg->data[i-1];
    }

    // Oznacz, że transmisja się rozpoczyna
    is_transmitting = 1;
    
    // Wyślij przez DMA
    HAL_UART_Transmit_DMA(&huart5, tx_buffer, total_size);
}

// Callback wywoływany po zakończeniu transmisji DMA
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART5)
    {
        is_transmitting = 0; // Transmisja zakończona
    }
    if (huart->Instance == USART2)
    {
        is_transmitting2 = 0; // Transmisja zakończona
    }
}

void CreateMessage(struct Message *msg, MsgID id, uint8_t *data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    for (uint8_t i = 0; i < msg->size; i++)
    {
        msg->data[i] = data[i];
    }
}

void CreateMessage32(struct Message *msg, MsgID id, uint32_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[0] = data & 0xFF; // Low byte
    msg->data[1] = (data >> 8) & 0xFF; // High byte
    msg->data[2] = (data >> 16) & 0xFF; // Middle byte
    msg->data[3] = (data >> 24) & 0xFF; // High byte
}

void CreateMessage16(struct Message *msg, MsgID id, uint16_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[0] = data & 0xFF; // Low byte
    msg->data[1] = (data >> 8) & 0xFF; // High byte
}

void CreateMessage8(struct Message *msg, MsgID id, uint8_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[0] = data;
}


uint8_t getBufferPosToWrite(uint8_t wannaWrite)
{
    return wannaWrite % UART_TX_BUFFER_SIZE;
}

void HandleIncomingMessage(struct Message *msg)
{
    logDebug("Received message with ID: ", 25);
    logDebug((char *)&msg->ID, 1);
    logDebug(" and value: ", 11);
    logDebug((char *)msg->data, msg->size);
    logDebug("\r\n", 2);
    TransmitMessageDMA(msg);

}