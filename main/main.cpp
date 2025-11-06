#include <esp_event.h>
#include <SigmaLogger.h>
#include <SigmaIO.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "main.h"

SigmaLogger Log(120);
void timerCallback(TimerHandle_t xTimer)
{
    
    Log.Append("Main timer").Debug();
}


extern "C" void app_main(void)
{
    printf("\n ==========================\n Start Application\n==========================\n");

    esp_event_loop_create_default();

    SigmaIO::SetLogger(&Log);
    printf("\n ==========================\n End Application\n==========================\n");

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
