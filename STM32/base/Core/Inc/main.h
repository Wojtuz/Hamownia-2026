/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IMU_INT1_Pin GPIO_PIN_13
#define IMU_INT1_GPIO_Port GPIOC
#define IMU_INT2_Pin GPIO_PIN_14
#define IMU_INT2_GPIO_Port GPIOC
#define EEP_WP_Pin GPIO_PIN_2
#define EEP_WP_GPIO_Port GPIOC
#define LIN_EN_Pin GPIO_PIN_1
#define LIN_EN_GPIO_Port GPIOA
#define LIN_TX_Pin GPIO_PIN_2
#define LIN_TX_GPIO_Port GPIOA
#define LIN_RX_Pin GPIO_PIN_3
#define LIN_RX_GPIO_Port GPIOA
#define SPI1_CS1_Pin GPIO_PIN_4
#define SPI1_CS1_GPIO_Port GPIOA
#define ESP_TX_Pin GPIO_PIN_4
#define ESP_TX_GPIO_Port GPIOC
#define ESP_RX_Pin GPIO_PIN_5
#define ESP_RX_GPIO_Port GPIOC
#define SPI1_CSS2_Pin GPIO_PIN_0
#define SPI1_CSS2_GPIO_Port GPIOB
#define USB_TX_Pin GPIO_PIN_10
#define USB_TX_GPIO_Port GPIOB
#define USB_RX_Pin GPIO_PIN_11
#define USB_RX_GPIO_Port GPIOB
#define NTC2_ADC_Pin GPIO_PIN_12
#define NTC2_ADC_GPIO_Port GPIOB
#define NTC1_ADC_Pin GPIO_PIN_13
#define NTC1_ADC_GPIO_Port GPIOB
#define INDICATOR_GREEN_Pin GPIO_PIN_15
#define INDICATOR_GREEN_GPIO_Port GPIOB
#define INDICATOR_YELLOW_Pin GPIO_PIN_6
#define INDICATOR_YELLOW_GPIO_Port GPIOC
#define INDICATOR_RED_Pin GPIO_PIN_7
#define INDICATOR_RED_GPIO_Port GPIOC
#define Chopper__OV_Pin GPIO_PIN_8
#define Chopper__OV_GPIO_Port GPIOC
#define Chopper_PWM_Pin GPIO_PIN_9
#define Chopper_PWM_GPIO_Port GPIOC
#define Chopper_ADC_Pin GPIO_PIN_8
#define Chopper_ADC_GPIO_Port GPIOA
#define LED_ER_Pin GPIO_PIN_9
#define LED_ER_GPIO_Port GPIOA
#define LED_OK_Pin GPIO_PIN_10
#define LED_OK_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_10
#define DEBUG_TX_GPIO_Port GPIOC
#define DEBUG_RX_Pin GPIO_PIN_11
#define DEBUG_RX_GPIO_Port GPIOC
#define RS485_TX_Pin GPIO_PIN_12
#define RS485_TX_GPIO_Port GPIOC
#define RS485_RX_Pin GPIO_PIN_2
#define RS485_RX_GPIO_Port GPIOD
#define RS485_RX_TX_Pin GPIO_PIN_5
#define RS485_RX_TX_GPIO_Port GPIOB
#define INA2_ALERT_Pin GPIO_PIN_6
#define INA2_ALERT_GPIO_Port GPIOB
#define INA1_ALERT_Pin GPIO_PIN_9
#define INA1_ALERT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
