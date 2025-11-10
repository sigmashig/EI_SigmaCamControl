#include "CamController.h"
#include "Camera1.h"
#include <algorithm>

namespace Sigma::CamControl
{
    CamController *CamController::Create(CamControllerConfig camControllerConfig, Sigma::Logger *loger)
    {
        CamController *camController = nullptr;
        switch (camControllerConfig.type)
        {
        case CamController::CAMERA_TYPE_1:
            camController = new Camera1(camControllerConfig, loger);
            break;
        default:
            camController = nullptr;
            break;
        }
        if (camController != nullptr)
        {
            camController->CreateGimbal();
        }
        return camController;
    }

    CamController::~CamController()
    {
        if (gimbal != nullptr)
        {
            delete gimbal;
            gimbal = nullptr;
        }
        if (Log != nullptr && isLogerSet)
        {
            delete Log;
            Log = nullptr;
        }
    }

    bool CamController::CreateGimbal()
    {
        gimbal = Sigma::Vehicle::Gimbal::Create(camControllerConfig.gimbalConfig);
        return gimbal != nullptr;
    }

    CamController::CameraType CamController::GetCameraType(std::string typeString)
    {
        std::string uTypeString = typeString;
        std::transform(uTypeString.begin(), uTypeString.end(), uTypeString.begin(), ::toupper);
        if (uTypeString == "CAMERA_TYPE_1")
        {
            return CameraType::CAMERA_TYPE_1;
        }
        return CameraType::CAMERA_TYPE_UNDEFINED;
    }

    CamController::CamController(CamControllerConfig camControllerConfig, Sigma::Logger *loger)
    {
        this->camControllerConfig = camControllerConfig;
        this->Log = (loger != nullptr) ? loger : Sigma::GetLogger();
        this->isLogerSet = (loger == nullptr);
        this->gimbal = nullptr;
    }
}