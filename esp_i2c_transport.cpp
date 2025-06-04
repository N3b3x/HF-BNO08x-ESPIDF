#include "bno08x_i2c_transport.hpp"

BNO08xI2CTransport::BNO08xI2CTransport(i2c_port_t port, gpio_num_t sda,
                                       gpio_num_t scl, uint8_t addr)
    : port_(port), sda_(sda), scl_(scl), addr_(addr) {}

bool BNO08xI2CTransport::open() {
    if (opened_)
        return true;
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda_;
    conf.scl_io_num = scl_;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = CONFIG_BNO08X_I2C_FREQ_HZ;
    esp_err_t err = i2c_param_config(port_, &conf);
    if (err != ESP_OK)
        return false;
    err = i2c_driver_install(port_, conf.mode, 0, 0, 0);
    if (err == ESP_OK)
        opened_ = true;
    return err == ESP_OK;
}

void BNO08xI2CTransport::close() {
    if (opened_) {
        i2c_driver_delete(port_);
        opened_ = false;
    }
}

int BNO08xI2CTransport::write(const uint8_t *data, uint32_t length) {
    esp_err_t err = i2c_master_write_to_device(
        port_, addr_, data, length,
        CONFIG_BNO08X_I2C_TIMEOUT_MS / portTICK_PERIOD_MS);
    return err == ESP_OK ? (int)length : -1;
}

int BNO08xI2CTransport::read(uint8_t *data, uint32_t length) {
    esp_err_t err = i2c_master_read_from_device(
        port_, addr_, data, length,
        CONFIG_BNO08X_I2C_TIMEOUT_MS / portTICK_PERIOD_MS);
    return err == ESP_OK ? (int)length : -1;
}
