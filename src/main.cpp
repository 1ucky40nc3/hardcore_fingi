#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "secrets.h"
#include "WebController.h"
#include "MeasurementService.h"
#include "CalibrationService.h"
#include "BatteryService.h"

// Your network credentials
const char *ssid = "YourWiFiSSID";
const char *password = "YourWiFiPassword";

AsyncWebServer server(80);
WebController *controller;

// Get the concrete service instance (Singleton)
MeasurementService &measurementService = MeasurementService::getInstance();
CalibrationService &calibrationService = CalibrationService::getInstance();
BatteryService &batteryService = BatteryService::getInstance();

void setup()
{
  Serial.begin(115200);
  delay(2000);

  if (WiFi.softAP(WIFI_SSID, WIFI_PASSWORD))
  {
    IPAddress ip = WiFi.softAPIP();
    Serial.printf("AP Started! IP Address: %s\n", ip.toString().c_str());
  }
  else
  {
    Serial.println("AP Failed to start!");
  }

  // 2. Initialize Controller using the three Abstract Service interfaces
  controller = new WebController(
      server,
      measurementService, // Passed as AbstractMeasurementService&
      calibrationService, // Passed as AbstractCalibrationService&
      batteryService      // Passed as AbstractBatteryService&
  );
  // 3. Start Server
  server.begin();
  Serial.println("REST API server started.");
}

void loop()
{
  // The main loop coordinates the periodic updates for each service
  measurementService.updateRawMeasurement();
  batteryService.updateBatteryCharge();

  // Print the current state for debugging
  Serial.printf("Status: Measurement=%.2f, Battery=%d%%, Factor=%.2f\n",
                measurementService.getLatestMeasurement(),
                batteryService.getBatteryCharge(),
                calibrationService.getCalibrationFactor());

  delay(5000);
}