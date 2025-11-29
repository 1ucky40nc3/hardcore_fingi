#include "WebController.h"
#include <ArduinoJson.h>

#define JSON_DOC_SIZE 128

// The constructor sets up all the API routes
WebController::WebController(AsyncWebServer &server, AbstractSensorService &sensorService)
    : sensorService_(sensorService) // Store the service reference
{
    // GET /measurement
    server.on("/measurement", HTTP_GET, std::bind(&WebController::handleMeasurement, this, std::placeholders::_1));

    // GET /battery
    server.on("/battery", HTTP_GET, std::bind(&WebController::handleBattery, this, std::placeholders::_1));

    // GET /calibration
    server.on("/calibration", HTTP_GET, std::bind(&WebController::handleGetCalibration, this, std::placeholders::_1));

    // PATCH /calibration
    server.on("/calibration", HTTP_PATCH,
              std::bind(&WebController::handleGetCalibration, this, std::placeholders::_1), // Ignore upload
              NULL,                                                                         // Ignore file upload
              std::bind(&WebController::handlePatchCalibration, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
}

// GET /measurement - Returns JSON { "value": 12.34 }
void WebController::handleMeasurement(AsyncWebServerRequest *request)
{
    float value = sensorService_.getLatestMeasurement(); // Call through service

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    doc["value"] = value;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

// GET /battery - Returns JSON { "charge_percent": 85 }
void WebController::handleBattery(AsyncWebServerRequest *request)
{
    int charge = sensorService_.getBatteryCharge(); // Call through service

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    doc["charge_percent"] = charge;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

// GET /calibration - Returns JSON { "calibration_factor": 1.05 }
void WebController::handleGetCalibration(AsyncWebServerRequest *request)
{
    float factor = sensorService_.getCalibrationFactor(); // Call through service

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    doc["calibration_factor"] = factor;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

// PATCH /calibration - Expects JSON { "calibration_factor": 1.2 }
void WebController::handlePatchCalibration(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == total - len)
    {
        StaticJsonDocument<JSON_DOC_SIZE> doc;
        DeserializationError error = deserializeJson(doc, (const char *)data);

        if (error)
        {
            request->send(400, "text/plain", "Invalid JSON or Body Too Large");
            return;
        }

        if (doc.containsKey("calibration_factor") && doc["calibration_factor"].is<float>())
        {
            float newFactor = doc["calibration_factor"].as<float>();

            // 1. Controller calls Service to update the state
            sensorService_.setCalibrationFactor(newFactor);

            // 2. Respond with the new state
            float updatedFactor = sensorService_.getCalibrationFactor();

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