#include "accelerometer.h"
#include "em_gpio.h"



// \CS pin high - for I2C
// Pullups required - UM10204 I2C-Bus Specification and User Manual, Rev. 03—19 June 2007
// Max 400kHz


//Disable interrupts before configuring

//5us between reading updated values (I2C is slow anyway)

uint8_t adxl345_get_device_id(uint16_t * id) {
    *id = i2c_read_register(ADXL345_REG_DEVID);
    if (*id == ADXL345_REG_DEVID_ID) {
        return 1;
    }
return 0;
}

void adxl345_power_on() {
    GPIO_PinOutSet(ADXL345_POWER_PORT, ADXL345_POWER_PIN);
}

void adxl345_power_off() {
    GPIO_PinOutClear(ADXL345_POWER_PORT, ADXL345_POWER_PIN);
}
