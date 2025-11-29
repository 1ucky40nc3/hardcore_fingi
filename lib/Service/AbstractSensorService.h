#ifndef ABSTRACT_SENSOR_H
#define ABSTRACT_SENSOR_H

// The abstract base class defines the interface (contract) for any sensor service
class AbstractSensorService
{
public:
    // Virtual destructor is mandatory for abstract classes
    virtual ~AbstractSensorService() = default;

    // --- Data Accessors (GET endpoints) ---
    virtual float getLatestMeasurement() const = 0;
    virtual int getBatteryCharge() const = 0;
    virtual float getCalibrationFactor() const = 0;

    // --- Data Mutators (PATCH endpoint) ---
    virtual void setCalibrationFactor(float factor) = 0;

    // --- Business/Update Logic ---
    virtual void updateSensorData() = 0;
};

#endif // ABSTRACT_SENSOR_H