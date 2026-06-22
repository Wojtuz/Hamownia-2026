#include "main.h"

extern I2C_HandleTypeDef hi2c1;


/* Niskopoziomowy odczyt 16-bitowego rejestru (INA237 wysyła MSB jako pierwszy) */
uint16_t INA237_ReadRegister(uint16_t devAddr, uint8_t regAddr)
{
    return 0;
    uint8_t buf[2];
    if (HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY) == HAL_OK)
    {
        return (uint16_t)((buf[0] << 8) | buf[1]);
    }
    return 0;
}

/* Niskopoziomowy zapis 16-bitowego rejestru */
void INA237_WriteRegister(uint16_t devAddr, uint8_t regAddr, uint16_t value)
{
    return;
    uint8_t buf[2];
    buf[0] = (value >> 8) & 0xFF;
    buf[1] = value & 0xFF;
    HAL_I2C_Mem_Write(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);
}

/* Podstawowa inicjalizacja (ustawienie trybu ciągłego ADC itp.) */
void INA237_InitDevice(uint16_t devAddr)
{
    // Przykład: Konfiguracja domyślna ADC (Continuous VBUS, VSHUNT, Temp)
    // Wartość 0xFBBC to przykładowe ustawienie czasu konwersji i uśredniania
    //INA237_WriteRegister(devAddr, INA237_REG_ADC_CONFIG, 0xFBBC);
    
    // Jeśli używasz rejestru CURRENT, musisz tutaj wpisać też wartość do rejestru SHUNT_CALIBRATION!
    // INA237_WriteRegister(devAddr, INA237_REG_SHUNT_CAL, ...);
}