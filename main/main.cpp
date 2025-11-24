#include <freertos/FreeRTOS.h>
#include <esp_event.h>
#include "main.h"
#include "SigmaLogger/LoggerMgr.h"
#include "SigmaIO/IOMgr.h"

TestType activeTest = TEST_UNKNOWN;

TestType testList[] = {

    TEST_GIMBAL,
};

bool CleanupTest(TestType testType)
{
    Sigma::IO::IOError err = Sigma::IO::SIGMAIO_SUCCESS;
    bool result = true;
    err = Sigma::IO::IOMgr::Cleanup();
    if (err != Sigma::IO::SIGMAIO_SUCCESS)
    {
        Sigma::GetLogger().Printf("Failed to cleanup SigmaIO:(%d) %s", (int16_t)err, Sigma::IO::IOErrorToString(err).c_str()).Error();
        result = false;
    }
    return result;
}

bool runTest(TestType testType)
{
    bool testResult = false;
    activeTest = testType;
   /*
    esp_event_handler_instance_t handlerInstance;
    err = IOMgr::RegisterEventHandlerInstance(ESP_EVENT_ANY_ID, eventHandler, NULL, &handlerInstance);
    if (err != SIGMAIO_SUCCESS)
    {
        printf("Failed to register event handler: %d\n", err);
        return false;
    }
*/
    switch (testType)
    {
    case TEST_GIMBAL:
    {
        testResult = testGimbal();
        break;
    }
    default:
    {
        Sigma::GetLogger().Printf("Unknown test type").Error();
        testResult = false;
        break;
    }
    }
    if (!CleanupTest(activeTest))
    {
        Sigma::GetLogger().Printf("⚠️ Cleanup test failed").Error();
    }
    activeTest = TEST_UNKNOWN;

    return testResult;
}

void runApplication()
{
    Sigma::GetLogger().Printf("----------------------------------- Hello world! ----------------------------------------").Info();
    int successCount = 0;
    int failureCount = 0;

    vTaskDelay(pdMS_TO_TICKS(1000));
    for (TestType testType : testList)
    {
        Sigma::GetLogger().Printf("=====Running test: %d =====", testType).Info();
        bool testResult = runTest(testType);
        if (testResult)
        {
            successCount++;
            Sigma::GetLogger().Printf("🟢[PASSED!]Test passed").Info();
        }
        else
        {
            failureCount++;
            Sigma::GetLogger().Printf("🔴[FAILED!]Test failed").Error();
        }
    }
    Sigma::GetLogger().Printf("===========================================").Info();
    Sigma::GetLogger().Printf("Total tests: %d, 🟢 Success: %d, 🔴 Failure: %d", successCount + failureCount, successCount, failureCount).Info();
    Sigma::GetLogger().Printf("===========================================").Info();
}

extern "C" void app_main(void)
{
    printf("\n==========================================\n");
    printf("Starting application\n");
    printf("==========================================\n");

    // Create default event loop
    esp_err_t ret;
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
    {
        printf("Failed to create default event loop: %s\n", esp_err_to_name(ret));
        return;
    }

    // IOMgr::PostEvent(pdLedPin);

    runApplication();
    printf("==========================================\n");
    printf("==========================================\n");
    printf("==========================================\n");
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
