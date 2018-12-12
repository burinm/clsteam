#include "accelerometer.h"
#include "em_gpio.h"
#include "i2c.h"

uint8_t accel_int1 = 0;
uint8_t accel_int2 = 0;

extern uint8_t i2c_data_array[I2C_DATA_ARRAY_SZ];

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

void adxl345_setup() {
i2c_write_register_1_byte(ADXL345_REG_DATA_FORMAT, ADXL345_REG_DATA_FORMAT_FULL_RES);
//default 100Hz - i2c_write_register_1_byte(ADXL345_REG_BW_RATE
i2c_write_register_1_byte(ADXL345_REG_BW_RATE,ADXL345_REG_BW_RATE_RATE_MASK & ADXL345_DATA_RATE_50_HZ);

//Interrupt on data ready
//i2c_write_register_1_byte(ADXL345_REG_INT_ENABLE, ADXL345_REG_INT_ENABLE_DATA_READY);



i2c_write_register_1_byte(ADXL345_REG_POWER_CTL, ADXL345_REG_POWER_CTL_MEASURE);
    
}

void adxl345_power_on() {
    GPIO_PinOutSet(ADXL345_POWER_PORT, ADXL345_POWER_PIN);

    //Setup interrupts for accelerometer
    GPIO_PinModeSet(ADXL345_INT1_PORT, ADXL345_INT1_PIN,gpioModeInput,false);
    GPIO_IntConfig(ADXL345_INT1_PORT, ADXL345_INT1_PIN, true, false, true); //even

    GPIO_PinModeSet(ADXL345_INT2_PORT, ADXL345_INT2_PIN,gpioModeInput,false);
    GPIO_IntConfig(ADXL345_INT2_PORT, ADXL345_INT2_PIN, true, false, true); //odd

    GPIO_IntEnable( ADXL345_INT1_PIN <<1 | ADXL345_INT2_PIN <<1 );
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);

}

void adxl345_power_off() {
    GPIO_PinOutClear(ADXL345_POWER_PORT, ADXL345_POWER_PIN);
}

void adxl345_read_xyz(xyz_data * d) {
    i2c_read_bytes(ADXL345_REG_DATAX0, 6);
    d->x = i2c_data_array[0] + (i2c_data_array[1] << 8);
    d->y = i2c_data_array[2] + (i2c_data_array[3] << 8);
    d->z = i2c_data_array[4] + (i2c_data_array[5] << 8);
}
