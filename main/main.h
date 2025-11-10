#pragma once

#include "testGimbal.h"

#define MOTOR_PITCH 32
#define MOTOR_YAW 33

typedef enum
{
    TEST_UNKNOWN = 0,
    TEST_GIMBAL,
} TestType;

void runApplication();
bool CleanupTest(TestType testType);
