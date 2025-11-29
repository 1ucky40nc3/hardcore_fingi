#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "secrets.h"
#include "WebController.h"
#include "SensorService.h"
#include "AbstractSensorService.h"

// Your network credentials
const char *ssid = "YourWiFiSSID";
const char *password = "YourWiFiPassword";

AsyncWebServer server(80);
WebController *controller;

// Get the concrete service instance (Singleton)
SensorService &sensorService = SensorService::getInstance();

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

  // 2. Initialize Controller using the AbstractSensor interface (Dependency Injection)
  // The controller receives the concrete SensorService instance but only sees its AbstractSensor methods.
  controller = new WebController(server, sensorService);

  // 3. Start Server
  server.begin();
  Serial.println("REST API server started.");
}

void loop()
{
  // The main loop calls the service's periodic logic
  sensorService.updateSensorData();
  // Print the current state for debugging
  Serial.printf("Status: Measurement=%.2f, Battery=%d%%, Factor=%.2f\n",
                sensorService.getLatestMeasurement(),
                sensorService.getBatteryCharge(),
                sensorService.getCalibrationFactor());

  delay(5000);
}