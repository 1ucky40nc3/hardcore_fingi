#ifndef BATTERY_SERVICE_H
#define BATTERY_SERVICE_H

#include <Arduino.h>
#include "AbstractBatteryService.h"

class BatteryService : public AbstractBatteryService
{
public:
    static BatteryService &getInstance()
    {
        static BatteryService instance;
        return instance;
    }

    // --- Implemented Interface Methods ---
    int getBatteryCharge() const override;
    void updateBatteryCharge() override;

private:
    BatteryService() : battery_charge_(100) {}

    int battery_charge_;

    BatteryService(const BatteryService &) = delete;
    BatteryService &operator=(const BatteryService &) = delete;
};

#endif // BATTERY_SERVICE_H