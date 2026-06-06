#ifndef INC_REGULATOR_H_
#define INC_REGULATOR_H_
#include "main.h"

void regulateTorquePI(float torqueSetPoint, float torqueValue, float * brakingAmps);

#endif /* INC_REGULATOR_H_ */