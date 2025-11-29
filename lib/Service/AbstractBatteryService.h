#ifndef ABSTRACT_BATTERY_SERVICE_H
#define ABSTRACT_BATTERY_SERVICE_H

// Interface for reading the battery status.
class AbstractBatteryService
{
public:
    virtual ~AbstractBatteryService() = default;

    // Data Accessor (GET endpoint)
    virtual int getBatteryCharge() const = 0;

    // Logic to simulate or read battery discharge.
    virtual void updateBatteryCharge() = 0;
};

#endif // ABSTRACT_BATTERY_SERVICE_H