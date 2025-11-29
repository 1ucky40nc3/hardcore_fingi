#ifndef ABSTRACT_MEASUREMENT_SERVICE_H
#define ABSTRACT_MEASUREMENT_SERVICE_H

// Interface for reading the current sensor value.
class AbstractMeasurementService
{
public:
    virtual ~AbstractMeasurementService() = default;

    // Returns the latest measured value (raw * factor).
    virtual float getLatestMeasurement() const = 0;

    // Logic to simulate or read new raw data.
    virtual void updateRawMeasurement() = 0;
};

#endif // ABSTRACT_MEASUREMENT_SERVICE_H