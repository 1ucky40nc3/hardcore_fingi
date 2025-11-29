#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <ESPAsyncWebServer.h>
#include "../Service/AbstractSensorService.h" // Dependency on the interface

class WebController
{
public:
    // Uses Dependency Injection: takes the server and the abstract service interface
    WebController(AsyncWebServer &server, AbstractSensorService &sensorService);

private:
    AbstractSensorService &sensorService_; // Reference to the service interface

    // Handlers for the REST API endpoints
    void handleMeasurement(AsyncWebServerRequest *request);
    void handleBattery(AsyncWebServerRequest *request);
    void handleGetCalibration(AsyncWebServerRequest *request);
    void handlePatchCalibration(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
};

#endif // WEB_CONTROLLER_H