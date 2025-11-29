#ifndef MEASUREMENT_SERVICE_H
#define MEASUREMENT_SERVICE_H

#include <Arduino.h>
#include "AbstractMeasurementService.h"
#include "CalibrationService.h" // Dependency on the concrete service

class MeasurementService : public AbstractMeasurementService
{
public:
    static MeasurementService &getInstance()
    {
        static MeasurementService instance;
        return instance;
    }

    // --- Implemented Interface Methods ---
    float getLatestMeasurement() const override;
    void updateRawMeasurement() override;

private:
    MeasurementService() : raw_measurement_(0.0f) {}

    float raw_measurement_;

    MeasurementService(const MeasurementService &) = delete;
    MeasurementService &operator=(const MeasurementService &) = delete;
};

#endif // MEASUREMENT_SERVICE_H