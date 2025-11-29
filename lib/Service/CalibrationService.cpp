#include "CalibrationService.h"

float CalibrationService::getCalibrationFactor() const
{
    return calibration_factor_;
}

void CalibrationService::setCalibrationFactor(float factor)
{
    // Basic validation could be added here
    if (factor > 0)
    {
        calibration_factor_ = factor;
        Serial.printf("CalibrationService: Factor updated to %.2f\n", factor);
    }
    else
    {
        Serial.println("CalibrationService: Attempted to set invalid factor (must be > 0).");
    }
}