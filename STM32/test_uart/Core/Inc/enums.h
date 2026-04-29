/*
 * enums.h
 *
 *  Created on: 29 kwi 2026
 *      Author: Wojtu
 */

#ifndef INC_ENUMS_H_
#define INC_ENUMS_H_

#include <stdint.h>

typedef enum {
    FEEDBACK_SENSOR_TORQUE = 0x01,
    FEEDBACK_SENSOR_BATTERY_DATA = 0x02,
    FEEDBACK_SENSOR_MOTOR_TEMPERATURE = 0x03,
    FEEDBACK_SENSOR_MOTOR_SPEED_DATA = 0x04,
} MsgID;

uint8_t getMessageSize(MsgID id)
{
    switch (id)
    {
        case FEEDBACK_SENSOR_TORQUE: return 2;
        case FEEDBACK_SENSOR_BATTERY_DATA: return 1;
        case FEEDBACK_SENSOR_MOTOR_TEMPERATURE: return 1;
        case FEEDBACK_SENSOR_MOTOR_SPEED_DATA: return 2;
        default: return 0;
    }
}


#endif /* INC_ENUMS_H_ */
