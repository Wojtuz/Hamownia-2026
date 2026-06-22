#include <stdint.h>
#include "regulator.h"

float regP = 5.0f;
float regI = 0.2f;

volatile float integral = 0;

void regulateTorquePI(float torqueSetPoint, float torqueValue, float * brakingAmps)
{
    float error = torqueSetPoint - torqueValue;
    
    float output = regP * error + regI * integral;

    if (!(output > 100.0f))
        integral += error;

    if (output < 0.0f)
        output = 0.0f;

    *brakingAmps = output;
}

void resetRegulatorState()
{
    integral = 0;
}