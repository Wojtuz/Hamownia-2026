#include "can.h"
#include "libVescCan/VESC_Convert.h"
#include "stm32g4xx_hal_fdcan.h"
#include "vesc2halcan.h"

void FDCAN_Config(FDCAN_HandleTypeDef *hfdcan)
{
	FDCAN_FilterTypeDef sFilterConfig;

	/* Configure Rx filter */
	sFilterConfig.IdType = FDCAN_EXTENDED_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;

	sFilterConfig.FilterID1 = 0xFF;			
	sFilterConfig.FilterID2 = 0x1FFF0000;

	if (HAL_FDCAN_ConfigFilter(hfdcan, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	/* Configure global filter:
	 Filter all remote frames with STD and EXT ID
	 Reject non matching frames with STD ID and EXT ID */
	if (HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK) {
		Error_Handler();
	}

	/* Start the FDCAN module */
	if (HAL_FDCAN_Start(hfdcan) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		Error_Handler();
	}
}

void TransmitOverCan(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	/* Prepare Tx header */
	TxHeader.Identifier = id;
	TxHeader.IdType = FDCAN_EXTENDED_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = (len <= 8) ? len : 8; // Ensure length does not exceed 8 bytes
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, data) != HAL_OK) {
		// Handle transmission error
	}
}

void TransmitVescCommand(FDCAN_HandleTypeDef *hfdcan, VESC_Id_t vescID, VESC_Command_t command, float value)
{
	// uint32_t id = ((uint32_t)vescID & 0xFF) | (((uint32_t)command & 0xFF) << 8);

	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];
	VESC_CommandFrame cmd;
	VESC_RawFrame rawFrame;

	cmd.vescID = vescID;
	cmd.command = command;
	cmd.commandData = value;

	VESC_ZeroMemory(&rawFrame, sizeof(rawFrame));
	VESC_ZeroMemory(&TxHeader, sizeof(TxHeader));
	VESC_ZeroMemory(TxData, sizeof(TxData));

	VESC_convertCmdToRaw(&rawFrame, &cmd);
	vesc2halcan(&TxHeader, TxData, sizeof(TxData), &rawFrame);
	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, TxData);
}