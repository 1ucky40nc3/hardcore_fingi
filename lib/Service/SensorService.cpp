#include "SensorService.h"

// Implementation of data accessors
float SensorService::getLatestMeasurement() const
{
    // In a real device, this would return the last known measurement
    return latest_measurement_ * calibration_factor_; // Apply calibration factor
}

int SensorService::getBatteryCharge() const
{
    return battery_charge_;
}

float SensorService::getCalibrationFactor() const
{
    return calibration_factor_;
}

// Implementation of data mutators
void SensorService::setCalibrationFactor(float factor)
{
    calibration_factor_ = factor;
    Serial.printf("Service: Calibration factor updated to %.2f\n", factor);
}

// Implementation of simulated business logic (run in loop())
void SensorService::updateSensorData()
{
    // Simulate reading a raw sensor value and discharging the battery
    float rawReading = random(100, 500) / 100.0f;
    latest_measurement_ = rawReading;
    battery_charge_ = constrain(battery_charge_ - 1, 0, 100);
}