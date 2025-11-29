#include "WebController.h"
#include <ArduinoJson.h>

#define JSON_DOC_SIZE 128

// The constructor sets up all the API routes
WebController::WebController(AsyncWebServer &server, AbstractSensorService &sensorService)
    : sensorService_(sensorService) // Store the service reference
{
    // GET /measurement - Use Lambda for unambiguous binding
    server.on("/measurement", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleMeasurement(request); });

    // GET /battery
    server.on("/battery", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleBattery(request); });

    // GET /calibration
    server.on("/calibration", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleGetCalibration(request); });

    // --------------------------------------------------------------------------
    // FIX: Use separate calls for the Request Handler and the Body Handler to
    // avoid the ambiguous overload error (C/C++(308)).
    // --------------------------------------------------------------------------
    server.on("/calibration", HTTP_PATCH, [this](AsyncWebServerRequest *request)
              {
                  // This is the main request handler (executed if no body handler is attached,
                  // or after the body handler finishes).
                  // Since we handle the response fully in onBody, this simply sends a success.
                  // It must be defined, but we ensure the actual work happens in onBody.
              })
        // Attach the dedicated Body Handler function
        .onBody([this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                { this->handlePatchCalibration(request, data, len, index, total); });
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
    // Note: The body handler is called multiple times for large bodies.
    // We only process and respond when the total data has been received (index + len == total).
    if (index + len == total)
    {
        StaticJsonDocument<JSON_DOC_SIZE> doc;
        // The data buffer is only valid during the scope of this function.
        DeserializationError error = deserializeJson(doc, (const char *)data, len);

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