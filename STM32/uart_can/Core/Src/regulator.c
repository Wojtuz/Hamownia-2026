#include <stdint.h>
#include "regulator.h"

float regP = 1.0f;
float regI = 1.0f;

volatile float integral;

void regulateTorquePI(float torqueSetPoint, float torqueValue, float * brakingAmps)
{
    float error = torqueSetPoint - torqueValue;
    integral += error;
    float output = regP * error + regI * integral;

    if (output < 0.0f)
        output = 0.0f;

    *brakingAmps = output;
}