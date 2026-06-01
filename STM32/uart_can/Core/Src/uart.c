#include "uart.h"
#include <stdint.h>

uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
uint8_t tx_buffer2[UART2_TX_BUFFER_SIZE];

volatile uint8_t is_transmitting = 0;
volatile uint8_t is_transmitting2 = 0;

static uint16_t getBufferPos(uint16_t position, uint16_t buffer_size)
{
    return position % buffer_size;
}

void UART_logDebug(UART_HandleTypeDef *huart, const char *msg, uint8_t size)
{
    while (is_transmitting2); // Czekaj, aż poprzedni transfer się skończy
    for (uint8_t i = 0; i < size; i++)
    {
        tx_buffer2[i] = msg[i];
    }
    is_transmitting2 = 1;
    HAL_UART_Transmit_DMA(huart, (uint8_t *)msg, size);
}

void UART_TransmitMessageDMA(UART_HandleTypeDef *huart, struct Message *msg)
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
    HAL_UART_Transmit_DMA(huart, tx_buffer, total_size);
}

// Callback wywoływany po zakończeniu transmisji DMA
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        is_transmitting = 0; // Transmisja zakończona
    }
    if (huart->Instance == USART2)
    {
        is_transmitting2 = 0; // Transmisja zakończona
    }
}

void UART_CreateMessage(struct Message *msg, MsgID id, uint8_t *data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    for (uint8_t i = 0; i < msg->size; i++)
    {
        msg->data[i] = data[i];
    }
}

void UART_CreateMessage32(struct Message *msg, MsgID id, uint32_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[0] = data & 0xFF; // Low byte
    msg->data[1] = (data >> 8) & 0xFF; // High byte
    msg->data[2] = (data >> 16) & 0xFF; // Middle byte
    msg->data[3] = (data >> 24) & 0xFF; // High byte
}

void UART_CreateMessage16(struct Message *msg, MsgID id, uint16_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[0] = data & 0xFF; // Low byte
    msg->data[1] = (data >> 8) & 0xFF; // High byte
}

void UART_CreateMessage8(struct Message *msg, MsgID id, uint8_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[0] = data;
}


uint8_t getBufferPosToWrite(uint8_t wannaWrite)
{
    return wannaWrite % UART_TX_BUFFER_SIZE;
}

void UART_HandleIncomingMessage(UART_HandleTypeDef *logHuart, struct Message *msg)
{
    UART_logDebug(logHuart, "Received message with ID: ", 25);
    UART_logDebug(logHuart, (char *)&msg->ID, 1);
    UART_logDebug(logHuart, " and value: ", 11);
    UART_logDebug(logHuart, (char *)msg->data, msg->size);
    UART_logDebug(logHuart, "\r\n", 2);
    UART_TransmitMessageDMA(logHuart, msg);
}

void UART_ProcessRxDmaBuffer(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx, uint8_t *rx_buffer, uint16_t rx_buffer_size, uint16_t *last_rx_pos)
{
    if ((huart == NULL) || (hdma_rx == NULL) || (rx_buffer == NULL) || (last_rx_pos == NULL) || (rx_buffer_size == 0U))
    {
        return;
    }

    struct Message msg;
    uint16_t currentPos = rx_buffer_size - __HAL_DMA_GET_COUNTER(hdma_rx);
    currentPos = getBufferPos(currentPos, rx_buffer_size);

    if (currentPos != *last_rx_pos)
    {
        for (uint16_t i = *last_rx_pos; i != currentPos; i = getBufferPos(i + 1U, rx_buffer_size))
        {
            HAL_UART_Transmit(huart, rx_buffer, rx_buffer_size, HAL_MAX_DELAY);
            HAL_Delay(10);

            if (rx_buffer[i] == 0xAA)
            {
                uint16_t data_index = getBufferPos(i + 1U, rx_buffer_size);
                msg.ID = rx_buffer[data_index];
                rx_buffer[data_index] = 0;
                msg.size = getMessageSize(msg.ID);

                for (uint8_t j = 0; j < msg.size; j++)
                {
                    data_index = getBufferPos(i + 2U + j, rx_buffer_size);
                    msg.data[j] = rx_buffer[data_index];
                    rx_buffer[data_index] = 0;
                }

                rx_buffer[i] = 0;
                i = getBufferPos(i + 1U + msg.size, rx_buffer_size);
                UART_HandleIncomingMessage(huart, &msg);
                HAL_Delay(5);
            }
            else
            {
                rx_buffer[i] = 0;
            }
        }

        *last_rx_pos = currentPos;
    }
}