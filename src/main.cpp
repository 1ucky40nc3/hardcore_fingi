#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <HX711.h>
#include "config.h"
#include "utils.h"

BLECharacteristic timeSeriesCharacteristic(TIME_SERIES_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
bool deviceConnected = false;

HX711 scale;
float calibration_factor = 33; // this calibration factor is adjusted according to my load cell

// BLE Callbacks
class MyBLEServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("BLE connected");
  };
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("BLE disconnected");
  }
};

void setup()
{
  // Initialize the serial connection
  Serial.begin(SERIAL_BAUD);
  // // wait for Serial to come online (only for testing/development purposes)
  // while (!Serial)
  //   ;
  delay(DELAY_AFTER_SERIAL_INITIALIZED);
  Serial.println("Serial start");

  // Create BLE Device
  BLEDevice::init(BLE_SERVER_NAME);

  // Create BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyBLEServerCallbacks());

  BLEService *pService = pServer->createService(TIME_SERIES_SERVICE_UUID);
  // Add characteristic and descriptors
  pService->addCharacteristic(&timeSeriesCharacteristic);
  timeSeriesCharacteristic.addDescriptor(new BLE2902());
  // Start the service
  pService->start();
  // Configure the advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(TIME_SERIES_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // Show the calibration message on the serial output
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  // Begin the scale calibration
  scale.begin(33, 32);
  scale.set_scale();
  scale.tare();                            // Reset the scale to 0
  long zero_factor = scale.read_average(); // Get a baseline reading
  Serial.print("Zero factor: ");           // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop()
{
  scale.set_scale(calibration_factor);          // Adjust to this calibration factor
  float measurement = scale.get_units() / 1000; // Get the scale readings in gram and convert to kg
  String measurementString = formatFloat(measurement);
  Serial.println(measurementString.c_str());

  timeSeriesCharacteristic.setValue(measurementString.c_str()); // For now we only output a constant value
  timeSeriesCharacteristic.notify();
  delay(100);
}
