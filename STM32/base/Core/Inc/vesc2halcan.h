#ifndef vesc2halcan_h_
#define vesc2halcan_h_

#include "libVescCan/VESC.h"

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_fdcan.h"

#define VESC2HALCAN_OK 0				// all good.
#define VESC2HALCAN_ERR_TXDATA 1		// txData array is too small to hold all data from vescRawFrame!
#define VESC2HALCAN_ERR_RXDATA 2		// vescRawFrame rawData size is too small!
#define VESC2HALCAN_ERR_DLC 3			// vescRawFrame cannot be converted due to non standard data length!

uint8_t vesc2halcan(FDCAN_TxHeaderTypeDef* txHeader, uint8_t* txData, size_t txDataSize, const VESC_RawFrame* vescRawFrame);
uint8_t halcan2vesc(VESC_RawFrame* vescRawFrame, const FDCAN_RxHeaderTypeDef* rxHeader, const uint8_t* rxData);

#endif //vesc2halcan_h_