#include "SigmaCamControl/Camera1.h"
#include <esp_http_client.h>
#include <SigmaLogger/LoggerMgr.h>

namespace Sigma::CamControl
{
    Camera1::Camera1(CamControllerConfig camControllerConfig) : CamController(camControllerConfig)
    {
    }

    Camera1::~Camera1()
    {
    }

    bool Camera1::Begin()
    {
        return true;
    }

    bool Camera1::SetZoom(int zoom)
    {

        if (zoom == 0)
        {
            return false;
        }
        if (camControllerConfig.ptzOverIP)
        {
            return setZoomOverIP(zoom);
        }
        else if (camControllerConfig.ptzOverSerial)
        {
            return setZoomOverSerial(zoom);
        }
        else
        {
            return false;
        }
    }
    bool Camera1::SetFocus(int focus)
    {
        if (focus == 0)
        {
            return false;
        }
        if (camControllerConfig.ptzOverIP)
        {
            return setFocusOverIP(focus);
        }
        else if (camControllerConfig.ptzOverSerial)
        {
            return setFocusOverSerial(focus);
        }
        else
        {
            return false;
        }
    }

    bool Camera1::SetStop()
    {
        if (camControllerConfig.ptzOverIP)
        {
            return setStopOverIP();
        }
        else if (camControllerConfig.ptzOverSerial)
        {
            return setStopOverSerial();
        }
        else
        {
            return false;
        }
    }
    bool Camera1::SetIris(int iris)
    {
        if (iris == 0)
        {
            return false;
        }
        if (camControllerConfig.ptzOverIP)
        {
            return setIrisOverIP(iris);
        }
        else if (camControllerConfig.ptzOverSerial)
        {
            return setIrisOverSerial(iris);
        }
        else
        {
            return false;
        }
    }

    bool Camera1::setZoomOverSerial(int zoomDirection)
    {
        return false;
    }

    bool Camera1::setStopOverIP()
    {
        return setPTZOverIP(cmdStop);
    }

    bool Camera1::setIrisOverIP(int irisDirection)
    {
        return setPTZOverIP(irisDirection == 1 ? cmdIrisOpen : cmdIrisClose);
    }

    bool Camera1::setStopOverSerial()
    {
        return false;
    }

    bool Camera1::setIrisOverSerial(int irisDirection)
    {
        return false;
    }

    bool Camera1::setFocusOverSerial(int focusDirection)
    {
        return false;
    }

    void Camera1::replacePTZBody(std::string &body, const char *cmd)
    {
        std::string replaceTpl = "%USERID%";
        int n;

        n = body.find(replaceTpl);
        if (n != std::string::npos)
        {
            body.replace(n, replaceTpl.length(), camControllerConfig.ipPTZConfig.userId);
        }
        replaceTpl = "%PASSWORD%";
        n = body.find(replaceTpl);
        if (n != std::string::npos)
        {
            body.replace(n, replaceTpl.length(), camControllerConfig.ipPTZConfig.password);
        }
        replaceTpl = "%CMD%";
        n = body.find(replaceTpl);
        if (n != std::string::npos)
        {
            body.replace(n, replaceTpl.length(), cmd);
        }
    }
    bool Camera1::setPTZOverIP(const char *cmd)
    {
        char ipAddress[16];
        ip4addr_ntoa_r((const ip4_addr_t *)&camControllerConfig.ipPTZConfig.ipAddress, ipAddress, sizeof(ipAddress));
        std::string url = "http://" + std::string(ipAddress) + camControllerConfig.ipPTZConfig.ptzURLSuffix;
        std::string body = ptzBody;
        replacePTZBody(body, cmd);
        std::string response = "";
        int httpCode = 0;
        esp_err_t err = ESP_OK;

        esp_http_client_config_t config = {};
        config.url = url.c_str();
        config.timeout_ms = 3000;
        config.method = HTTP_METHOD_POST;

        esp_http_client_handle_t client = esp_http_client_init(&config);
        if (client != nullptr)
        {
            err = esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
            if (err == ESP_OK)
            {
                err = esp_http_client_set_post_field(client, body.c_str(), body.length());
                if (err == ESP_OK)
                {

                    err = esp_http_client_perform(client);
                    if (err == ESP_OK)
                    {
                        httpCode = esp_http_client_get_status_code(client);
                        int contentLength = esp_http_client_get_content_length(client);
                        if (contentLength > 0)
                        {
                            char *buffer = new char[contentLength + 1];
                            int dataRead = 0;
                            int totalRead = 0;
                            while (totalRead < contentLength)
                            {
                                dataRead = esp_http_client_read(client, buffer + totalRead, contentLength - totalRead);
                                if (dataRead <= 0)
                                {
                                    break;
                                }
                                totalRead += dataRead;
                            }
                            if (totalRead > 0)
                            {
                                buffer[totalRead] = '\0';
                                response = std::string(buffer);
                            }
                            delete[] buffer;
                        }
                        else
                        {
                            constexpr int BufferSize = 512;
                            char buffer[BufferSize];
                            int dataRead = 0;
                            while ((dataRead = esp_http_client_read(client, buffer, BufferSize - 1)) > 0)
                            {
                                buffer[dataRead] = '\0';
                                response += std::string(buffer);
                            }
                        }
                    }
                }
            }
        }
        esp_http_client_cleanup(client);
        Sigma::GetLogger(ccTag).Printf("Response: (%d)%s", httpCode, response.c_str()).Error();
        return (httpCode == 200);
    }

    bool Camera1::setZoomOverIP(int zoomDirection)
    {
        return setPTZOverIP(zoomDirection == 1 ? cmdZoomTele : cmdZoomWide);
    }

    bool Camera1::setFocusOverIP(int focusDirection)
    {
        return setPTZOverIP(focusDirection == 1 ? cmdFocusNear : cmdFocusFar);
    }
}