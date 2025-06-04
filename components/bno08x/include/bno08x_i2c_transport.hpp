#pragma once
#include "BNO085_Transport.hpp"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

/**
 * @brief IBNO085Transport implementation using ESP-IDF I2C driver.
 */
class BNO08xI2CTransport : public IBNO085Transport {
public:
    explicit BNO08xI2CTransport(i2c_port_t port = (i2c_port_t)CONFIG_BNO08X_I2C_PORT,
                                 gpio_num_t sda = (gpio_num_t)CONFIG_BNO08X_I2C_SDA,
                                 gpio_num_t scl = (gpio_num_t)CONFIG_BNO08X_I2C_SCL,
                                 uint8_t addr = CONFIG_BNO08X_I2C_ADDR);
    bool open() override;
    void close() override;
    int write(const uint8_t *data, uint32_t length) override;
    int read(uint8_t *data, uint32_t length) override;
    bool dataAvailable() override { return true; }
    void delay(uint32_t ms) override { vTaskDelay(pdMS_TO_TICKS(ms)); }
    uint32_t getTimeUs() override { return (uint32_t)esp_timer_get_time(); }

private:
    i2c_port_t port_;
    gpio_num_t sda_;
    gpio_num_t scl_;
    uint8_t addr_;
    bool opened_{false};
};
