#include "WebController.h"
#include <ArduinoJson.h>

#define JSON_DOC_SIZE 128

// The constructor now takes all three services
WebController::WebController(
    AsyncWebServer &server,
    AbstractMeasurementService &measurementService,
    AbstractCalibrationService &calibrationService,
    AbstractBatteryService &batteryService) : measurementService_(measurementService),
                                              calibrationService_(calibrationService),
                                              batteryService_(batteryService)
{
    // GET /measurement
    server.on("/measurement", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleMeasurement(request); });

    // GET /battery
    server.on("/battery", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleBattery(request); });

    // GET /calibration
    server.on("/calibration", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleGetCalibration(request); });

    // PATCH /calibration using Lambda + onBody chaining (Fixes C/C++(308) error)
    server.on("/calibration", HTTP_PATCH, [](AsyncWebServerRequest *request)
              {
                  // Request handler is mostly ignored as work is done in onBody
              })
        .onBody([this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                { this->handlePatchCalibration(request, data, len, index, total); });
}

// GET /measurement - Uses Measurement Service
void WebController::handleMeasurement(AsyncWebServerRequest *request)
{
    float value = measurementService_.getLatestMeasurement();

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    doc["value"] = value;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

// GET /battery - Uses Battery Service
void WebController::handleBattery(AsyncWebServerRequest *request)
{
    int charge = batteryService_.getBatteryCharge();

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    doc["charge_percent"] = charge;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

// GET /calibration - Uses Calibration Service
void WebController::handleGetCalibration(AsyncWebServerRequest *request)
{
    float factor = calibrationService_.getCalibrationFactor();

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    doc["calibration_factor"] = factor;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

// PATCH /calibration - Uses Calibration Service
void WebController::handlePatchCalibration(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index + len == total)
    {
        StaticJsonDocument<JSON_DOC_SIZE> doc;
        DeserializationError error = deserializeJson(doc, (const char *)data, len);

        if (error)
        {
            request->send(400, "text/plain", "Invalid JSON or Body Too Large");
            return;
        }

        if (doc.containsKey("calibration_factor") && doc["calibration_factor"].is<float>())
        {
            float newFactor = doc["calibration_factor"].as<float>();

            // 1. Controller calls Calibration Service to update the state
            calibrationService_.setCalibrationFactor(newFactor);

            // 2. Respond with the new state
            float updatedFactor = calibrationService_.getCalibrationFactor();

            StaticJsonDocument<JSON_DOC_SIZE> resp_doc;
            resp_doc["calibration_factor"] = updatedFactor;

            String response;
            serializeJson(resp_doc, response);
            request->send(200, "application/json", response);
        }
        else
        {
            request->send(400, "text/plain", "Missing or invalid 'calibration_factor'");
        }
    }
}