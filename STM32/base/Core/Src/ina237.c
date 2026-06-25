#include "main.h"

extern I2C_HandleTypeDef hi2c1;
#define INA237_ADDR1            (0x40 << 1) // Przypuszczalny adres I2C dla układu 1 (pamiętaj o przesunięciu bitowym w lewo dla HAL!)
#define INA237_ADDR2            (0x41 << 1) // Przypuszczalny adres I2C dla układu 2

#define INA237_REG_CONFIG       0x00
#define INA237_REG_ADC_CONFIG   0x01
#define INA237_REG_SHUNT_CAL    0x02
#define INA237_REG_VBUS         0x05 // Napięcie na szynie (Bus Voltage)
#define INA237_REG_DIAG_ALRT    0x0B
#define INA237_REG_MANUFACTURER 0x3E // Register, który czytasz w swoim kodzie (powinien zwrócić 0x5449)

/* Niskopoziomowy odczyt 16-bitowego rejestru (INA237 wysyła MSB jako pierwszy) */
uint16_t INA237_ReadRegister(uint16_t devAddr, uint8_t regAddr)
{
    return 0;
    uint8_t buf[2];
    if (HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, buf, 2, 100) == HAL_OK)
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
    HAL_I2C_Mem_Write(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
}

void INA237_InitDevice(uint16_t devAddr) {
    return;
    devAddr = devAddr << 1;
    // 1. Sprawdź ID producenta (opcjonalny test komunikacji)
    uint16_t id = INA237_ReadRegister(devAddr, INA237_REG_MANUFACTURER);
    if(id != 0x5449) {
        // Błąd komunikacji z tym adresem! Możesz tu dać np. zapalenie czerwonej diody LED_ER
        HAL_GPIO_WritePin(LED_ER_GPIO_Port, LED_ER_Pin, GPIO_PIN_SET);
        return;
    }

    // 2. Podstawowa konfiguracja ADC (np. tryb ciągły dla Shunt i Bus voltage)
    // 0xFBAC to przykładowa wartość domyślna z datasheetu dla trybu Continuous
    INA237_WriteRegister(devAddr, INA237_REG_ADC_CONFIG, 0xFBAC);
    
    // 3. TUTAJ należy wyliczyć i wpisać SHUNT_CAL, jeśli chcesz mieć odczyt prądu w Amperach.
    // INA237_WriteRegister(devAddr, INA237_REG_SHUNT_CAL, twoja_wyliczona_wartosc);
}