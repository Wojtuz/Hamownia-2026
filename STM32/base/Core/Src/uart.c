#include "uart.h"
#include "can.h"
#include "libVescCan/VESC_Consts.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_fdcan.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
uint8_t tx_buffer2[UART2_TX_BUFFER_SIZE];

uint16_t last_rx_pos = 0;
uint8_t rx_buffer[UART_RX_BUFFER_SIZE] = {0};

volatile uint8_t is_transmitting = 0;
volatile uint8_t is_transmitting2 = 0;

extern uint8_t brakeVescID;
extern uint8_t testVescID;

extern volatile float torqueSetpoint;
extern volatile float torqueValue;

extern volatile bool regulatorON;
extern volatile bool brakeCommandActive;
extern volatile uint8_t brakeMotorVescCommand;
extern volatile float brakeMotorVescData;

extern volatile bool testCommandActive;
extern volatile uint8_t testMotorVescCommand;
extern volatile float testMotorVescData;

extern volatile bool uart_started;

static uint16_t getBufferPos(uint16_t position, uint16_t buffer_size)
{
    return position % buffer_size;
}

void UART_logDebug(UART_HandleTypeDef *huart, const char *msg, uint8_t size)
{
    while (is_transmitting); // Czekaj, aż poprzedni transfer się skończy
    for (uint8_t i = 0; i < size; i++)
    {
        tx_buffer2[i] = msg[i];
    }
    is_transmitting = 1;
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
    msg->data[3] = data & 0xFF; // Low byte
    msg->data[2] = (data >> 8) & 0xFF; // High byte
    msg->data[1] = (data >> 16) & 0xFF; // Middle byte
    msg->data[0] = (data >> 24) & 0xFF; // High byte
}

void UART_CreateMessage16(struct Message *msg, MsgID id, uint16_t data)
{
    msg->ID = id;
    msg->size = getMessageSize(id);
    msg->data[1] = data & 0xFF; // Low byte
    msg->data[0] = (data >> 8) & 0xFF; // High byte
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

void updateBrakeCommand(uint8_t BrakeMotorVescCommand, float value)
{
    brakeMotorVescCommand = BrakeMotorVescCommand;
    brakeMotorVescData = value;
    brakeCommandActive = true;
}

void updateTestCommand(uint8_t TestMotorVescCommand, float value)
{
    testMotorVescCommand = TestMotorVescCommand;
    testMotorVescData = value;
    testCommandActive = true;
}

void UART_HandleIncomingMessage(UART_HandleTypeDef *huart, FDCAN_HandleTypeDef *hfdcan, struct Message *msg)
{
    uart_started = true;
    switch (msg->ID)
    {
        /* =========================
         * SENSOR FEEDBACK
         * ========================= */
        case FEEDBACK_SENSOR_TORQUE:
        {
            float torque = (int16_t)((msg->data[0] << 8) | msg->data[1]) / 100.0f;

            torqueValue = torque;
            break;
        }

        /* =========================
         * BRAKE MOTOR SETPOINTS
         * ========================= */
        case SET_BRAKE_MOTOR_BRAKE_TORQUE:
        {
            float torque = (int32_t)((msg->data[0] << 8) | msg->data[1]) / 100.0f;
            torqueSetpoint = torque;
            regulatorON = true;
            break;
        }

        case SET_BRAKE_MOTOR_BRAKE_CURRENT: 
        {
            float current = (int32_t)((msg->data[0] << 8) | msg->data[1]) / 100.0f; // to libVescCan scaling
            updateBrakeCommand(VESC_COMMAND_SET_CURRENT_BRAKE, current);
            regulatorON = false;
            break;
        }    

        case SET_BRAKE_MOTOR_DRIVE_CURRENT:
        {
            float current = (int32_t)((msg->data[0] << 8) | msg->data[1]) / 100.0f;
            updateBrakeCommand(VESC_COMMAND_SET_CURRENT, current);
            regulatorON = false;
            break;
        }

        case SET_BRAKE_MOTOR_SPEED:
        {
            float rpm = (int32_t)((msg->data[0] << 8) | msg->data[1]) / 1.0f;
            updateBrakeCommand(VESC_COMMAND_SET_RPM, rpm);
            regulatorON = false;
            break;
        }

        case SET_BRAKE_MOTOR_DUTY:
        {
            float duty = msg->data[0] / 100.0f;
            updateBrakeCommand(VESC_COMMAND_SET_DUTY, duty);
            regulatorON = false;
            break;
        }

        /* =========================
         * TEST MOTOR SETPOINTS
         * ========================= */
        case SET_TEST_MOTOR_BRAKE_TORQUE:
        {
            //int16_t torque = (int16_t)((msg->data[0] << 8) | msg->data[1]);
            /// UNSUPPORTED
            break;
        }

        case SET_TEST_MOTOR_BRAKE_CURRENT: 
        {
            float current = (int16_t)((msg->data[0] << 8) | msg->data[1]) / 100.0f;
            updateTestCommand(VESC_COMMAND_SET_CURRENT_BRAKE, current);
            break;
        }    

        case SET_TEST_MOTOR_DRIVE_CURRENT:
        {
            float current = (int32_t)((msg->data[0] << 8) | msg->data[1]) / 100.0f;
            updateTestCommand(VESC_COMMAND_SET_CURRENT, current);
            break;
        }

        case SET_TEST_MOTOR_SPEED:
        {
            float rpm = (int32_t)((msg->data[0] << 8) | msg->data[1]);
            updateTestCommand(VESC_COMMAND_SET_RPM, rpm);
            break;
        }

        /* =========================
         * CONFIG
         * ========================= */
        case CONFIG_TEST_MOTOR_FRAMES:
        {
            //uint16_t mask = (msg->data[0] << 8) | msg->data[1];
            ///UNSUPPORTED YET
            break;
        }

        case CONFIG_TEST_MOTOR_CAN_ID:
        {
            uint8_t can_id = msg->data[0];
            testVescID = can_id;
            break;
        }

        default:
            // unknown frame
            break;
    }
}

void UART_StartReceiveDMA(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx)
{
    HAL_StatusTypeDef status;

    status = HAL_UART_Receive_DMA(
        huart,
        rx_buffer,
        UART_RX_BUFFER_SIZE
    );

    if(status != HAL_OK)
    {
        Error_Handler();
    }
}

void UART_ProcessRxDmaBuffer(UART_HandleTypeDef *huart, FDCAN_HandleTypeDef *hfdcan, DMA_HandleTypeDef *hdma_rx)
{
    struct Message msg;
    uint16_t currentPos = UART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(hdma_rx);
    currentPos = getBufferPos(currentPos, UART_RX_BUFFER_SIZE);

    if (currentPos != last_rx_pos)
    {
        for (uint16_t i = last_rx_pos; i != currentPos; i = getBufferPos(i + 1U, UART_RX_BUFFER_SIZE))
        {
            //HAL_UART_Transmit(huart, rx_buffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
            //HAL_Delay(10);

            if (rx_buffer[i] == 0xAA)
            {
                uint16_t data_index = getBufferPos(i + 1U, UART_RX_BUFFER_SIZE);
                msg.ID = rx_buffer[data_index];
                rx_buffer[data_index] = 0;
                msg.size = getMessageSize(msg.ID);

                for (uint8_t j = 0; j < msg.size; j++)
                {
                    data_index = getBufferPos(i + 2U + j, UART_RX_BUFFER_SIZE);
                    msg.data[j] = rx_buffer[data_index];
                    rx_buffer[data_index] = 0;
                }

                rx_buffer[i] = 0;
                i = getBufferPos(i + 1U + msg.size, UART_RX_BUFFER_SIZE);
                UART_HandleIncomingMessage(huart, hfdcan, &msg);
                //HAL_Delay(5);
                //HAL_UART_Transmit(huart, (uint8_t *)"\n", 1, HAL_MAX_DELAY);    
            }
            else
            {
                rx_buffer[i] = 0;
            }
        }

        last_rx_pos = currentPos;
    }
}