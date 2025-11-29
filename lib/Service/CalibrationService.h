#ifndef CALIBRATION_SERVICE_H
#define CALIBRATION_SERVICE_H

#include <Arduino.h>
#include "AbstractCalibrationService.h"

class CalibrationService : public AbstractCalibrationService
{
public:
    // Singleton pattern
    static CalibrationService &getInstance()
    {
        static CalibrationService instance;
        return instance;
    }

    // --- Implemented Interface Methods ---
    float getCalibrationFactor() const override;
    void setCalibrationFactor(float factor) override;

private:
    CalibrationService() : calibration_factor_(1.0f) {}

    float calibration_factor_;

    CalibrationService(const CalibrationService &) = delete;
    CalibrationService &operator=(const CalibrationService &) = delete;
};

#endif // CALIBRATION_SERVICE_H