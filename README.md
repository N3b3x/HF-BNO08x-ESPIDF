# HF-BNO08x-ESPIDF
This repository packages the [HF-BNO08x](https://github.com/N3b3x/HF-BNO08x) driver as an ESP-IDF component for the ESP32‑C6. Only a simple I²C transport is provided here. The hardware‑agnostic C++ driver and vendor SH‑2 library live in the `HF-BNO08x` submodule.

## Features

- Complete access to all BNO08x sensor reports
- Lightweight C++11 implementation with no threads
- Automatic recovery from sensor resets
- Optional callback based event handling
- Simple I²C transport implementation for ESP‑IDF

## Configuration

The component exposes several Kconfig options under `BNO08X`:

- **I2C port** (`BNO08X_I2C_PORT`) – controller number (0 or 1)
- **SDA GPIO** (`BNO08X_I2C_SDA`) – pin used for SDA
- **SCL GPIO** (`BNO08X_I2C_SCL`) – pin used for SCL
- **I2C clock speed** (`BNO08X_I2C_FREQ_HZ`) – default 400 kHz
- **Device address** (`BNO08X_I2C_ADDR`) – 0x4A or 0x4B depending on the ADR pin
- **Timeout** (`BNO08X_I2C_TIMEOUT_MS`) – I²C transaction timeout
- **INT GPIO** (`BNO08X_INT_GPIO`) – optional interrupt pin (-1 to poll)

Adjust these in `menuconfig` to match your hardware wiring.

## Usage

Include the transport header and create an instance:

```cpp
#include "bno08x_i2c_transport.hpp"
#include "BNO085.hpp"

BNO08xI2CTransport transport;        // uses Kconfig defaults
BNO085 imu(&transport);

void app_main(void)
{
    if (!imu.begin()) {
        printf("BNO08x not found\n");
        return;
    }
    imu.enableSensor(BNO085Sensor::RotationVector, 10); // 100 Hz
    while (true) {
        imu.update();
        if (imu.hasNewData(BNO085Sensor::RotationVector)) {
            auto event = imu.getLatest(BNO085Sensor::RotationVector);
            printf("Yaw %.1f\n", event.toEuler().yaw);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

For efficient operation connect the sensor's **INT** pin to a GPIO and set
`CONFIG_BNO08X_INT_GPIO` accordingly. When the pin goes low call `imu.update()`
and optionally register a callback:

```cpp
static void imu_isr(void* arg)
{
    auto* imu = static_cast<BNO085*>(arg);
    imu->update();
}

void app_main(void)
{
    BNO08xI2CTransport transport;
    static BNO085 imu(&transport);
    // configure interrupt
    gpio_set_intr_type((gpio_num_t)CONFIG_BNO08X_INT_GPIO, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add((gpio_num_t)CONFIG_BNO08X_INT_GPIO, imu_isr, &imu);

    imu.setCallback([](const SensorEvent& e) {
        // handle events here
    });
}
```

The driver and SH‑2 sources are compiled directly from the `HF-BNO08x`
submodule, so you benefit from all upstream updates.

## Repository Structure

```
include/                # public API for transport
src/esp_i2c_transport.cpp   # I²C implementation
CMakeLists.txt          # builds the driver and SH‑2 library
Kconfig / Kconfig.projbuild
README.md               # this document
HF-BNO08x/              # upstream submodule with library code
```

Refer to the upstream README for an in‑depth description of the driver
architecture and capabilities.
