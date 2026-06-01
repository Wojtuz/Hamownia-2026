/*
 * enums.h
 *
 *  Created on: 29 kwi 2026
 *      Author: Wojtu
 */

#ifndef INC_ENUMS_H_
#define INC_ENUMS_H_

#include <stdint.h>

typedef enum
{
    /* Sensor board (0x0X) */
    FEEDBACK_SENSOR_TORQUE               = 0x01,
    FEEDBACK_SENSOR_BATTERY_DATA         = 0x02,
    FEEDBACK_SENSOR_MCU_TEMPERATURE      = 0x05,

    /* Sensor commands */
    SET_SENSOR_MCU_TORQUE_RESET          = 0x21,

    /* Brake motor - VESC (0x4X) */
    FEEDBACK_BRAKE_MOTOR_CURRENT         = 0x41,
    FEEDBACK_BRAKE_MOTOR_VOLTAGE         = 0x42,
    FEEDBACK_BRAKE_MOTOR_POWER           = 0x43,
    FEEDBACK_BRAKE_MOTOR_SPEED           = 0x44,
    FEEDBACK_BRAKE_MOTOR_TEMPERATURE     = 0x45,
    FEEDBACK_BRAKE_MOTOR_POSITION        = 0x48,

    /* Brake motor commands */
    SET_BRAKE_MOTOR_BRAKE_CURRENT        = 0x61,
    SET_BRAKE_MOTOR_BRAKE_TORQUE         = 0x62,
    SET_BRAKE_MOTOR_DRIVE_CURRENT        = 0x63,
    SET_BRAKE_MOTOR_SPEED                = 0x64,
    SET_BRAKE_MOTOR_DUTY                 = 0x65,

    /* Test motor feedback (0x8X) */
    FEEDBACK_TEST_MOTOR_CURRENT          = 0x81,
    FEEDBACK_TEST_MOTOR_VOLTAGE          = 0x82,
    FEEDBACK_TEST_MOTOR_POWER            = 0x83,
    FEEDBACK_TEST_MOTOR_SPEED            = 0x84,
    FEEDBACK_TEST_MOTOR_TEMPERATURE      = 0x85,

    /* Test motor commands */
    SET_TEST_MOTOR_BRAKE_CURRENT         = 0xA1,
    SET_TEST_MOTOR_BRAKE_TORQUE          = 0xA2,
    SET_TEST_MOTOR_DRIVE_CURRENT         = 0xA3,
    SET_TEST_MOTOR_SPEED                 = 0xA4,

    /* General */
    GENERAL_WEB_STOP                     = 0xC1,
    GENERAL_BASE_STOP                    = 0xC2,

    /* Configuration */
    CONFIG_TEST_MOTOR_FRAMES             = 0xF0,
    CONFIG_TEST_MOTOR_CAN_ID             = 0xF1

} MsgID;

static inline uint8_t getMessageSize(MsgID id)
{
    switch (id)
    {
        /* Sensor board */
        case FEEDBACK_SENSOR_TORQUE:              return 2;
        case FEEDBACK_SENSOR_BATTERY_DATA:        return 1;
        case FEEDBACK_SENSOR_MCU_TEMPERATURE:     return 1;
        case SET_SENSOR_MCU_TORQUE_RESET:         return 0;

        /* Brake motor feedback */
        case FEEDBACK_BRAKE_MOTOR_CURRENT:        return 2;
        case FEEDBACK_BRAKE_MOTOR_VOLTAGE:        return 2;
        case FEEDBACK_BRAKE_MOTOR_POWER:          return 2;
        case FEEDBACK_BRAKE_MOTOR_SPEED:          return 2;
        case FEEDBACK_BRAKE_MOTOR_TEMPERATURE:    return 1;
        case FEEDBACK_BRAKE_MOTOR_POSITION:       return 2;

        /* Brake motor commands */
        case SET_BRAKE_MOTOR_BRAKE_CURRENT:       return 2;
        case SET_BRAKE_MOTOR_BRAKE_TORQUE:        return 2;
        case SET_BRAKE_MOTOR_DRIVE_CURRENT:       return 2;
        case SET_BRAKE_MOTOR_SPEED:               return 2;
        case SET_BRAKE_MOTOR_DUTY:                return 1;

        /* Test motor feedback */
        case FEEDBACK_TEST_MOTOR_CURRENT:         return 2;
        case FEEDBACK_TEST_MOTOR_VOLTAGE:         return 2;
        case FEEDBACK_TEST_MOTOR_POWER:           return 2;
        case FEEDBACK_TEST_MOTOR_SPEED:           return 2;
        case FEEDBACK_TEST_MOTOR_TEMPERATURE:     return 1;

        /* Test motor commands */
        case SET_TEST_MOTOR_BRAKE_CURRENT:        return 2;
        case SET_TEST_MOTOR_BRAKE_TORQUE:         return 2;
        case SET_TEST_MOTOR_DRIVE_CURRENT:        return 2;
        case SET_TEST_MOTOR_SPEED:                return 2;

        /* General */
        case GENERAL_WEB_STOP:                    return 1;
        case GENERAL_BASE_STOP:                   return 1;

        /* Config */
        case CONFIG_TEST_MOTOR_FRAMES:            return 2;
        case CONFIG_TEST_MOTOR_CAN_ID:            return 1;

        default:
            return 0;
    }
}

struct Message
{
    MsgID ID;
    uint8_t size;
    uint8_t data[4];
};

#endif /* INC_ENUMS_H_ */