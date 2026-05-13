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
    /* Sensor feedback (ESP C -> ESP B -> Webserver) */
    FEEDBACK_SENSOR_TORQUE                = 0x01, // to base board, for torque control loop
    FEEDBACK_SENSOR_BATTERY_DATA          = 0x02,
    FEEDBACK_SENSOR_MOTOR_TEMPERATURE     = 0x03,
    FEEDBACK_SENSOR_MOTOR_SPEED_DATA      = 0x04, // IF encoder is present on the sensor board
    FEEDBACK_SENSOR_MCU_TEMPERATURE       = 0x05,

    /* Brake motor feedback (VESC chain) */
    FEEDBACK_BRAKE_MOTOR_CURRENT          = 0x41,
    FEEDBACK_BRAKE_MOTOR_VOLTAGE          = 0x42,
    FEEDBACK_BRAKE_MOTOR_POWER            = 0x43,
    FEEDBACK_BRAKE_MOTOR_SPEED            = 0x44,
    FEEDBACK_BRAKE_MOTOR_TEMPERATURE      = 0x45,

    /* Brake motor setpoints from webserver board to base board */
    SET_BRAKE_MOTOR_CURRENT               = 0x46,
    SET_BRAKE_MOTOR_TORQUE                = 0x47,

    /* Test motor feedback from base board to webserver board */
    FEEDBACK_TEST_MOTOR_CURRENT           = 0x81,
    FEEDBACK_TEST_MOTOR_VOLTAGE           = 0x82,
    FEEDBACK_TEST_MOTOR_POWER             = 0x83,
    FEEDBACK_TEST_MOTOR_SPEED             = 0x84,
    FEEDBACK_TEST_MOTOR_TEMPERATURE       = 0x85,

    /* Config */
    CONFIG_TEST_MOTOR_CAN_ID              = 0xff

} MsgID;

uint8_t getMessageSize(MsgID id)
{
    switch (id)
    {
        /* Sensor feedback */
        case FEEDBACK_SENSOR_TORQUE:               return 2;
        case FEEDBACK_SENSOR_BATTERY_DATA:         return 1;
        case FEEDBACK_SENSOR_MOTOR_TEMPERATURE:    return 1;
        case FEEDBACK_SENSOR_MOTOR_SPEED_DATA:     return 2;
        case FEEDBACK_SENSOR_MCU_TEMPERATURE:      return 1;

        /* Brake motor feedback */
        case FEEDBACK_BRAKE_MOTOR_CURRENT:         return 2;
        case FEEDBACK_BRAKE_MOTOR_VOLTAGE:         return 2;
        case FEEDBACK_BRAKE_MOTOR_POWER:           return 2;
        case FEEDBACK_BRAKE_MOTOR_SPEED:           return 2;
        case FEEDBACK_BRAKE_MOTOR_TEMPERATURE:     return 1;

        /* Brake motor control */
        case SET_BRAKE_MOTOR_CURRENT:              return 2;
        case SET_BRAKE_MOTOR_TORQUE:               return 2;

        /* Test motor feedback */
        case FEEDBACK_TEST_MOTOR_CURRENT:          return 2;
        case FEEDBACK_TEST_MOTOR_VOLTAGE:          return 2;
        case FEEDBACK_TEST_MOTOR_POWER:            return 2;
        case FEEDBACK_TEST_MOTOR_SPEED:            return 2;
        case FEEDBACK_TEST_MOTOR_TEMPERATURE:      return 1;

        /* Config */
        case CONFIG_TEST_MOTOR_CAN_ID:             return 1;

        default:
            return 0;
    }
}


#endif /* INC_ENUMS_H_ */