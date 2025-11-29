#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H

#include <Arduino.h>
#include "AbstractSensorService.h"

class SensorService : public AbstractSensorService
{ // Inherits from the abstract interface
public:
    // Singleton pattern for easy global access to the single instance
    static SensorService &getInstance()
    {
        static SensorService instance;
        return instance;
    }

    // --- Implemented Interface Methods ---
    float getLatestMeasurement() const override;
    int getBatteryCharge() const override;
    float getCalibrationFactor() const override;
    void setCalibrationFactor(float factor) override;
    void updateSensorData() override;

private:
    SensorService() : latest_measurement_(0.0f), battery_charge_(100), calibration_factor_(1.0f) {}

    float latest_measurement_;
    int battery_charge_;
    float calibration_factor_;

    // Prevent copy and assignment (for Singleton)
    SensorService(const SensorService &) = delete;
    SensorService &operator=(const SensorService &) = delete;
};

#endif // SENSOR_SERVICE_H