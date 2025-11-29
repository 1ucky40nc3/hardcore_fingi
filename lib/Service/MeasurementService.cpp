#include "MeasurementService.h"

float MeasurementService::getLatestMeasurement() const
{
    // The core logic of the measurement service: apply the factor from the CalibrationService
    float factor = CalibrationService::getInstance().getCalibrationFactor();
    return raw_measurement_ * factor;
}

void MeasurementService::updateRawMeasurement()
{
    // Simulate reading from a physical sensor
    raw_measurement_ = random(100, 500) / 100.0f;
}