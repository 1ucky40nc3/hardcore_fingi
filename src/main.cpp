#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "config.h"

BLECharacteristic timeSeriesCharacteristic(TIME_SERIES_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

bool deviceConnected = false;

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
}

void loop()
{
  timeSeriesCharacteristic.setValue("100"); // For now we only output a constant value
  timeSeriesCharacteristic.notify();
  delay(2000);
}
