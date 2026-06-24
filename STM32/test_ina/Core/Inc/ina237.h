#ifndef __INA237_H
#define __INA237_H

#include "main.h"

// Domyślny adres I2C INA237 (A0=GND, A1=GND) przesunięty dla biblioteki HAL (0x40 << 1)
#define INA237_ADDRESS_1            0x80
#define INA237_ADDRESS_2            0x88

// Adresy rejestrów INA237
#define INA237_REG_CONFIG           0x00
#define INA237_REG_ADC_CONFIG       0x01
#define INA237_REG_SHUNT_CAL        0x02
#define INA237_REG_VBUS             0x05
#define INA237_REG_CURRENT          0x07
#define INA237_REG_POWER            0x08
#define INA237_REG_MANUFACTURER_ID  0x3E

typedef struct {
    float    vbus_v;        // Przeliczone napięcie w Woltach [V]
    float    current_a;     // Przeliczony prąd w Amperach [A]
    uint16_t raw_vbus;
    uint16_t raw_current;
} INA237_Data;

// Deklaracje funkcji
void INA237_Read_Data(uint16_t devAddr, INA237_Data *data);
void INA237_WriteRegister(uint16_t devAddr, uint8_t regAddr, uint16_t value);
uint8_t INA237_InitDevice(uint16_t devAddr);
void INA237_I2C_Scanner(void);
uint8_t INA237_InitDevice(uint16_t devAddr);


#endif /* __INA237_H */