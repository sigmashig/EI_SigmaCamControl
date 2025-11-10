#pragma once

#include "CamController.h"

namespace Sigma::CamControl
{
	class Camera1 : public CamController
	{
	public:
		Camera1(CamControllerConfig camControllerConfig, Sigma::Logger *loger);
		~Camera1();

		bool Begin() override;
		bool SetZoom(int zoom) override;
		bool SetFocus(int focus) override;
		bool SetStop() override;
		bool SetIris(int iris) override;

	private:
		static constexpr const char *ptzBody = "<?xml version=\"1.0\"?>"
											   "<soap:Envelope xmlns:soap=\"http://www.w3.org/2001/12/soap-envelope\">"
											   "<soap:Header>"
											   "<userid>%USERID%</userid>"
											   "<passwd>%PASSWORD%</passwd>"
											   "</soap:Header>"
											   "<soap:Body>"
											   "<xml>"
											   "<cmd>%CMD%</cmd>"
											   "</xml>"
											   "</soap:Body>"
											   "</soap:Envelope>";

		static constexpr const char *cmdStop = "stop";
		static constexpr const char *cmdZoomTele = "zoomtele";
		static constexpr const char *cmdZoomWide = "zoomwide";
		static constexpr const char *cmdFocusNear = "FocusNearAutoOff";
		static constexpr const char *cmdFocusFar = "FocusFarAutoOff";
		static constexpr const char *cmdIrisOpen = "IrisOpenAutoOff";
		static constexpr const char *cmdIrisClose = "IrisCloseAutoOff";

		void replacePTZBody(std::string &body, const char *cmd);

		bool setPTZOverIP(const char *cmd);

		bool setZoomOverIP(int zoomDirection);
		bool setFocusOverIP(int focusDirection);
		bool setFocusOverSerial(int focusDirection);
		bool setZoomOverSerial(int zoomDirection);
		bool setStopOverIP();
		bool setIrisOverIP(int irisDirection);
		bool setStopOverSerial();
		bool setIrisOverSerial(int irisDirection);
	};
}