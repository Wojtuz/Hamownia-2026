#include "ina237.h"
#include <stdio.h>
#include <string.h>

// Deklaracja zewnętrznych uchwytów peryferiów z pliku main.c
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

// Zewnętrzne struktury zdefiniowane w main.c
extern INA237_Data ina1;
extern INA237_Data ina2;

/* Niskopoziomowy odczyt 16-bitowego rejestru (INA237 wysyła MSB jako pierwszy) */
uint16_t INA237_ReadRegister(uint16_t devAddr, uint8_t regAddr)
{
  uint8_t buf[2];
  
  // Używamy dedykowanej funkcji MEM do odczytu konkretnego rejestru urządzenia I2C
  if (HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, buf, 2, 3) == HAL_OK)
  {
    return (uint16_t)((buf[0] << 8) | buf[1]);
  }
  return 0;
}

/* Niskopoziomowy zapis 16-bitowego rejestru (MSB-First) */
void INA237_WriteRegister(uint16_t devAddr, uint8_t regAddr, uint16_t value)
{
  uint8_t buf[2];
  buf[0] = (value >> 8) & 0xFF; // Starszy bajt
  buf[1] = value & 0xFF;        // Młodszy bajt
  
  HAL_I2C_Mem_Write(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, buf, 2, 3);
}

/* Podstawowa inicjalizacja (ustawienie trybu ciągłego ADC itp.) */
uint8_t INA237_InitDevice(uint16_t devAddr)
{
  uint16_t id = 0;
  
  // Sprawdzenie obecności układu (Manufacturer ID dla INA237 to zawsze 0x5449)
  id = INA237_ReadRegister(devAddr, INA237_REG_MANUFACTURER_ID);
  if (id != 0x5449)
  {
    return HAL_ERROR; // Brak układu lub błąd komunikacji
  }
  
  // 1. Reset i konfiguracja podstawowa CONFIG (Zakres bocznikowy ±163.84 mV)
  INA237_WriteRegister(devAddr, INA237_REG_CONFIG, 0x0000);
  
  // 2. Konfiguracja czasu konwersji i uśredniania (Continuous VBUS, VSHUNT)
  INA237_WriteRegister(devAddr, INA237_REG_ADC_CONFIG, 0xFB58);
  
  // 3. Kalibracja prądu (Przykładowo: bocznik 10mOhm, rozdzielczość 1mA -> SHUNT_CAL = 819)
  INA237_WriteRegister(devAddr, INA237_REG_SHUNT_CAL, 819);
  
  return HAL_OK;
}

/* Bezpieczny odczyt danych i zapis bezpośrednio do wskazanej struktury */
void INA237_Read_Data(uint16_t devAddr, INA237_Data *data)
{
  char uart_buf[128];
  
  if (data == NULL) return; // Zabezpieczenie przed przekazaniem pustego wskaźnika
  
  // Odczyt surowych danych i konwersja na float wewnątrz struktury
  data->raw_vbus = INA237_ReadRegister(devAddr, INA237_REG_VBUS);
  data->vbus_v = (float)data->raw_vbus * 0.003125f; // Rozdzielczość VBUS = 3.125 mV/LSB
  
  data->raw_current = (int16_t)INA237_ReadRegister(devAddr, INA237_REG_CURRENT);
  data->current_a = (float)data->raw_current * 0.001f; // Przy założeniu CURRENT_LSB = 1mA
  
  // Formatowanie float na liczby całkowite dla oszczędności pamięci w printf
  int32_t vbus_int = (int32_t)data->vbus_v;
  int32_t vbus_frac = (int32_t)((data->vbus_v - vbus_int) * 1000.0f);
  if (vbus_frac < 0) vbus_frac = -vbus_frac;

  int32_t curr_int = (int32_t)data->current_a;
  int32_t curr_frac = (int32_t)((data->current_a - curr_int) * 1000.0f);
  if (curr_frac < 0) curr_frac = -curr_frac;
  
  // Wyświetlamy adres (devAddr >> 1), aby log w konsoli pokazywał czytelny adres 7-bitowy (np. 0x40, 0x41)
  sprintf(uart_buf, "INA237 [0x%02X] -> Napiecie: %ld.%03ld V | Prad: %ld.%03ld A\r\n", 
          (devAddr >> 1), vbus_int, vbus_frac, curr_int, curr_frac);
          
  HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
}

/* Skaner I2C do diagnostyki połączenia */
void INA237_I2C_Scanner(void)
{
  char msg[64];
  HAL_StatusTypeDef result;
  uint8_t devices_found = 0;

  sprintf(msg, "\r\n--- Rozpoczynanie skanowania magistrali I2C ---\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  for (uint16_t i = 1; i < 128; i++)
  {
    result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i << 1), 1, 10);
    if (result == HAL_OK)
    {
      devices_found++;
      sprintf(msg, "Znaleziono urzadzenie pod adresem: 0x%02X (Zapis: 0x%02X)\r\n", i, (i << 1));
      HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
  }

  if (devices_found == 0)
  {
    sprintf(msg, "Nie znaleziono zadnych urzadzen I2C.\r\n");
  }
  else
  {
    sprintf(msg, "Skanowanie zakonczone. Liczba urzadzen: %d\r\n", devices_found);
  }
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}