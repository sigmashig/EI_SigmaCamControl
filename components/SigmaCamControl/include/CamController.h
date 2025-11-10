#pragma once

#include "Gimbal.h"
#include "Logger.h"
#include "lwip/ip_addr.h"

namespace Sigma::CamControl
{
	class CamController
	{
	public:
		typedef enum
		{
			CAMERA_TYPE_UNDEFINED,
			CAMERA_TYPE_1,
		} CameraType;

		typedef struct
		{
			CameraType type;
			bool isValid;
			bool ptzOverIP;
			bool ptzOverSerial;
			Sigma::Vehicle::Gimbal::GimbalConfig gimbalConfig;
			struct
			{
				ip_addr_t ipAddress;
				std::string userId;
				std::string password;
				std::string ptzURLSuffix;
			} ipPTZConfig;
			struct
			{
				Sigma::IO::PinN rxPin;
				Sigma::IO::PinN txPin;
				uint16_t baudRate;
			} serialPTZConfig;
		} CamControllerConfig;

		static CamController *Create(CamControllerConfig camControllerConfig, Sigma::Logger *_loger = nullptr);
		virtual ~CamController();

		virtual bool Begin() = 0;
		virtual bool SetYawAngle(uint16_t angle) { return gimbal != nullptr && gimbal->SetYawAngle(angle); };
		virtual bool SetYawMotion(int8_t motion) { return gimbal != nullptr && gimbal->SetYawMotion(motion); };
		virtual bool SetPitchAngle(uint16_t angle) { return gimbal != nullptr && gimbal->SetPitchAngle(angle); };
		virtual bool SetPitchMotion(int8_t motion) { return gimbal != nullptr && gimbal->SetPitchMotion(motion); };
		virtual bool SetRollAngle(uint16_t angle) { return gimbal != nullptr && gimbal->SetRollAngle(angle); };
		virtual bool SetRollMotion(int8_t motion) { return gimbal != nullptr && gimbal->SetRollMotion(motion); };
		virtual bool SetZoomTele() { return SetZoom(1); };
		virtual bool SetZoomWide() { return SetZoom(-1); };
		virtual bool SetZoom(int zoom) { return false; };
		virtual bool SetFocus(int focus) { return false; };
		virtual bool SetFocusNear() { return SetFocus(1); };
		virtual bool SetFocusFar() { return SetFocus(-1); };
		virtual bool SetStop() { return false; };
		virtual bool SetIris(int iris) { return false; };
		virtual bool SetIrisOpen() { return SetIris(1); };
		virtual bool SetIrisClose() { return SetIris(-1); };

		enum class Direction : int
		{
			Negative = -1,
			Positive = 1
		};
		bool SetZoom(Direction d) { return SetZoom(static_cast<int>(d)); }
		bool SetFocus(Direction d) { return SetFocus(static_cast<int>(d)); }
		bool SetIris(Direction d) { return SetIris(static_cast<int>(d)); }

		virtual bool CreateGimbal();

		static CameraType GetCameraType(std::string typeString);

	protected:
		Sigma::Logger *Log = nullptr;
		bool isLogerSet = false;
		CamControllerConfig camControllerConfig;
		CamController(CamControllerConfig camControllerConfig, Sigma::Logger *_loger);
		Sigma::Vehicle::Gimbal *gimbal = nullptr;

	private:
	};
}