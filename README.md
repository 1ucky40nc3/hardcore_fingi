# hardcore_fingi

The Hardware and Microcontroller Programming Part of the [Open Fingi Project].

## Getting Started

This repository contains the hardware assembly instructions and the microcontroler code for the [Open Fingi Project].

### Hardware Assembly

The project currently uses the following hardware:

- [ESP32 ePulse Feather] as a microcontroller
- [Mavin Load Cell - S Type - 200kg] as the sensor for the finger strength measurements
- [Load Cell Amplifier- HX711] as an interface between the microcontroller and the sensor
- [Eye Bolt](https://www.amazon.de/dp/B0BWYTTYPR?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) to be able to mount carabiners on the sensor
- [Nuts](https://www.amazon.de/dp/B09D3CCJRY?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) to lock the eye bolt in place
- some wires

Use the pin and wire layout described below to connect the hardware:

Next connect the [Load Cell Amplifier- HX711] to the [ESP32 ePulse Feather] using the following layout:

| [ESP32 ePulse Feather] | Cable Color | [Load Cell Amplifier- HX711] |
| ---------------------- | ----------- | ---------------------------- |
| GND                    | Black       | GND                          |
| 5V                     | Red         | VCC                          |
| 33                     | White       | DT                           |
| 32                     | Green       | SCL                          |

Lastly connect the [Load Cell Amplifier- HX711] to the [Mavin Load Cell - S Type - 200kg] according to this layout:

| [Load Cell Amplifier- HX711] | [Mavin Load Cell - S Type - 200kg] Cable Color |
| ---------------------------- | ---------------------------------------------- |
| E+                           | Red                                            |
| E-                           | Black                                          |
| A-                           | White                                          |
| A+                           | Green                                          |

The transparent cable of the [Mavin Load Cell - S Type - 200kg] is not connected to anything. You may refer to the [Tutorial to Interface HX711 Balance Module With Load Cell] for guidance.

### Microcontroler

A Bluetooth Low Energy Server (BLE) implemented using [PlatformIO] on the [ESP32 ePulse Feather] hardware. T Here however we implemente the following Generic Attribute Profile (GATT) for the BLE communication:

We implement one service for time serial data. The service has one characteristic with the `notify` and `read` properties. The characteristic has values that may be interpreted as integers type data as part of a time series. The UUID of the service and characteristic can be found in the [configuration](#configuration).

### Install the PlatformIO IDE VSCode Extension

Install the PlatformIO IDE extension from the VSCode extension marketplace (see workspace recommendations). You may be asked to install Python. You may follow the instructions [here](https://docs.platformio.org/en/latest/faq/install-python.html).

### Initialize the Project

In this example a PlatformIO project has already been created. The project configuration can be inspected in the [platformio.ini](./platformio.ini) file.

The `platformio` (or `pio`) command-line interface should be installed along the the extension. For the local development purposes you need to intialize the command-line interface. Execute the following commands in the command-line:

```bash
pio project init
```

Note: The command-line has to be a PlatformIO terminal. You can create a new PlatformIO termincal by pressing the `PlatformIO: New Terminal` buttom in the bottom left of your VSCode window.

Run the `pio check` command to do a static code analysis to check if everything is setup correctly.

### Set up Configuration and Secrets

You can find sections below on how to handle [configuration](#configuration) and [secrets](#secrets) down below. Make to follow the instructions and you should be good to go.

### Build and Upload the Code to the [ESP32 ePulse Feather]

After installation and initialization you may build and upload the code to your device. Connect the Ardunino Nano ESP32 via USB. Then run the following commands (in a PlatformIO terminal):

```bash
# Check if your device can be detected
pio device list
# Build the code
pio run -e esp32-s3-devkitm-1 -t build
# Upload the build
pio run -e esp32-s3-devkitm-1 -t upload
```

As an alternative to the command you can also use the PlatformIO buttons in VSCode. You can find the buttons below. Make sure to select the correct (`esp32-s3-devkitm-1`) environment for the upload.

### Run Tests

```bash
# Run test locally on your desktop
pio test -e desktop
# Run test on the device
pio test -e esp32-s3-devkitm-1
```

#### Debug Tests

If you are using VSCode (which is recommended), use the following steps to debug your test runs:

1. Select the `desktop` environment to select your local machine as the build target
2. Open the testing module [test_desktop_main.cpp](test/test_desktop/test_desktop_main.cpp) in your editor
3. Go to the debug tab in VSCode
4. Launch the code with the `PIO Debug` configuration

This will automatically attach a gdb debugger and enable VSCode debugging.

## Documentation

### Configuration

Set the configuration in the [config.h](lib/Config/config.h) header file. Use the provided constants as they are used in the code.

## Sources

- [Open Fingi Project]
- [PlatformIO]
- [ESP32 ePulse Feather]
- [Mavin Load Cell - S Type - 200kg]
- [Load Cell Amplifier- HX711]

[Open Fingi Project]: https://github.com/1ucky40nc3/open_fingi_project
[PlatformIO]: https://platformio.org/
[ESP32 ePulse Feather]: https://thingpulse.com/product/epulse-feather-low-power-esp32-development-board/
[Mavin Load Cell - S Type - 200kg]: https://www.tinytronics.nl/en/sensors/weight-pressure-force/load-cells/mavin-load-cell-s-type-200kg
[Load Cell Amplifier- HX711]: https://www.tinytronics.nl/en/sensors/weight-pressure-force/load-cells/load-cell-amplifier-hx711
