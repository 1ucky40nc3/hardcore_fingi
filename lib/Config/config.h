/**
 * @brief The baud rate for serial communication.
 *
 * This constant sets the data transfer rate for the serial port,
 */
const unsigned long SERIAL_BAUD = 115200;

/**
 * @brief The delay in milliseconds after serial communication is initialized.
 *
 * This delay provides a brief pause after setting up the serial port,
 * allowing the serial connection to stabilize before any data transmission begins.
 * It can be useful to ensure that the connected device is ready to receive commands or data.
 */
const unsigned long DELAY_AFTER_SERIAL_INITIALIZED = 100;

/**
 * @brief The name of the BLE server/device.
 *
 * This name is displayed when scanning for BLE devices.
 */
const char *BLE_SERVER_NAME = "ESP32-WROVER-E";

/**
 * @brief A GATT service with a characteristic that can publish time series data.
 */
const char *TIME_SERIES_SERVICE_UUID = "fe0fadc2-a9dc-4566-96f5-dd8a4934dac2";

/**
 * @brief A GATT service with a characteristic that can publish time series data.
 */
const char *TIME_SERIES_CHARACTERISTIC_UUID = "cba1d466-344c-4be3-ab3f-189f80dd7518";