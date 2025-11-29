#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <ESPAsyncWebServer.h>
#include "../Service/AbstractMeasurementService.h"
#include "../Service/AbstractCalibrationService.h"
#include "../Service/AbstractBatteryService.h"

class WebController
{
public:
    // Uses Dependency Injection for all three specialized services
    WebController(
        AsyncWebServer &server,
        AbstractMeasurementService &measurementService,
        AbstractCalibrationService &calibrationService,
        AbstractBatteryService &batteryService);

private:
    // References to the abstract interfaces
    AbstractMeasurementService &measurementService_;
    AbstractCalibrationService &calibrationService_;
    AbstractBatteryService &batteryService_;

    // Handlers for the REST API endpoints
    void handleMeasurement(AsyncWebServerRequest *request);
    void handleBattery(AsyncWebServerRequest *request);
    void handleGetCalibration(AsyncWebServerRequest *request);
    void handlePatchCalibration(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
};

#endif // WEB_CONTROLLER_H