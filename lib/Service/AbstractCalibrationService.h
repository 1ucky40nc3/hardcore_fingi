#ifndef ABSTRACT_CALIBRATION_SERVICE_H
#define ABSTRACT_CALIBRATION_SERVICE_H

// Interface for managing the calibration factor.
class AbstractCalibrationService
{
public:
    virtual ~AbstractCalibrationService() = default;

    // Data Accessor (GET endpoint)
    virtual float getCalibrationFactor() const = 0;

    // Data Mutator (PATCH endpoint)
    virtual void setCalibrationFactor(float factor) = 0;
};

#endif // ABSTRACT_CALIBRATION_SERVICE_H