#include <stdint.h>
#include "regulator.h"

float regP = 0.02f;
float regI = 0.001f;

volatile float integral = 0;

void regulateTorquePI(float torqueSetPoint, float torqueValue, float * brakingAmps)
{
    float error = torqueSetPoint - torqueValue;
    
    float output = regP * error + regI * integral;

    if (!(error > 0 && integral > 100.0f))
        integral += error;
    
    if (!(error < 0 && integral < -100.0f))
        integral += error;

    *brakingAmps = output;
}

void resetRegulatorState()
{
    integral = 0;
}