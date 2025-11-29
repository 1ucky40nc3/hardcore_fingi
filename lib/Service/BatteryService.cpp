#include "BatteryService.h"

int BatteryService::getBatteryCharge() const
{
    return battery_charge_;
}

void BatteryService::updateBatteryCharge()
{
    // Simulate battery discharge
    battery_charge_ = constrain(battery_charge_ - 1, 0, 100);
    // In a real device, this would involve reading the voltage and calculating the percentage
}