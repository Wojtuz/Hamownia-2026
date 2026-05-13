#include "uart.h"
#include <stdint.h>

#define UART_TX_BUFFER_SIZE 10
static uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
static uint8_t is_transmitting = 0;

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
    
    uint8_t temp;

    // Przygotuj dane w buforze statycznym
    tx_buffer[0] = 0xAA; // Start byte
    tx_buffer[1] = msg->message_type;
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
}

void CreateMessage(struct Message *msg, MsgID id, uint8_t *data)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        msg->data[i] = 0;
    }
    msg->message_type = id;
    msg->size = getMessageSize(id);
    for (uint8_t i = 0; i < msg->size; i++)
    {
        msg->data[i] = data[i];
    }
}