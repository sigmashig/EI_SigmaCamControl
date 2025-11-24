#include "main.h"
#include "testGimbal.h"
#include "SigmaIO/IOMgr.h"

bool testGimbal()
{
    Sigma::IO::IOError err = Sigma::IO::SIGMAIO_SUCCESS;

    Sigma::IO::PinDefinition motorPin;

    motorPin.pinType = Sigma::IO::PinType::SIGMAIO_PIN_TYPE_PWM;
    motorPin.pinValue.pin = MOTOR_PITCH;
    motorPin.pinParams.pwmPin.frequency = 50;
    motorPin.pinParams.pwmPin.resolution = 12;

    err = Sigma::IO::IOMgr::RegisterPin(motorPin);
    if (err != Sigma::IO::SIGMAIO_SUCCESS)
    {
        printf("Error registering PITCH motor pin: (%d) (%s)\n", err, Sigma::IO::IOErrorToString(err).c_str());
        return false;
    }

    motorPin.pinValue.pin = MOTOR_YAW;
    err = Sigma::IO::IOMgr::RegisterPin(motorPin);
    if (err != Sigma::IO::SIGMAIO_SUCCESS)
    {
        printf("Error registering YAW motor pin: (%d) (%s)\n", err, Sigma::IO::IOErrorToString(err).c_str());
        return false;
    }

    // Rotate PITCH motor
    

    return true;
}