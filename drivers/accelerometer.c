#include "accelerometer.h"
#include "em_gpio.h"
#include "i2c.h"

volatile uint8_t accel_int1 = 0;
volatile uint8_t accel_int2 = 0;

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

void adxl345_defaults() {
    i2c_write_register_1_byte(ADXL345_REG_THRESH_TAP,0);
    i2c_write_register_1_byte(ADXL345_REG_OFSX,0);
    i2c_write_register_1_byte(ADXL345_REG_OFSY,0);
    i2c_write_register_1_byte(ADXL345_REG_OFSZ,0);
    i2c_write_register_1_byte(ADXL345_REG_DUR,0);
    i2c_write_register_1_byte(ADXL345_REG_LATENT,0);
    i2c_write_register_1_byte(ADXL345_REG_WINDOW,0);
    i2c_write_register_1_byte(ADXL345_REG_THRESH_ACT,0);
    i2c_write_register_1_byte(ADXL345_REG_THRESH_INACT,0);
    i2c_write_register_1_byte(ADXL345_REG_TIME_INACT,0);
    i2c_write_register_1_byte(ADXL345_REG_ACT_INACT_CTL,0);
    i2c_write_register_1_byte(ADXL345_REG_THRESH_FF,0);
    i2c_write_register_1_byte(ADXL345_REG_TIME_FF,0);
    i2c_write_register_1_byte(ADXL345_REG_TAP_AXES,0);

    i2c_write_register_1_byte(ADXL345_REG_BW_RATE,ADXL345_REG_BW_DEFAULT);
    i2c_write_register_1_byte(ADXL345_REG_POWER_CTL,0);
    i2c_write_register_1_byte(ADXL345_REG_POWER_CTL_AUTO_SLEEP,0);
    i2c_write_register_1_byte(ADXL345_REG_INT_ENABLE,0);
    i2c_write_register_1_byte(ADXL345_REG_INT_MAP,0);

    i2c_write_register_1_byte(ADXL345_REG_DATA_FORMAT,0);

    i2c_write_register_1_byte(ADXL345_REG_FIFO_CTL,0);

    //Clear interrupts with read
    (void) i2c_read_register(ADXL345_REG_INT_SOURCE);
}


void adxl345_setup() {

#if 0
i2c_write_register_1_byte(ADXL345_REG_DATA_FORMAT, ADXL345_REG_DATA_FORMAT_FULL_RES);
//default 100Hz - i2c_write_register_1_byte(ADXL345_REG_BW_RATE
i2c_write_register_1_byte(ADXL345_REG_BW_RATE,ADXL345_REG_BW_RATE_RATE_MASK & ADXL345_DATA_RATE_50_HZ);

//Clear interrupts
i2c_write_register_1_byte(ADXL345_REG_INT_ENABLE, 0);

//Interrupt on data ready
i2c_write_register_1_byte(ADXL345_REG_INT_ENABLE, ADXL345_REG_INT_ENABLE_DATA_READY);

//Data ready interrupt on INT2
//i2c_write_register_1_byte(ADXL345_REG_INT_MAP,ADXL345_REG_INT_MAP_DATA_READY);

//Clear interrupts with read
(void) i2c_read_register(ADXL345_REG_INT_SOURCE);

//Start data
i2c_write_register_1_byte(ADXL345_REG_POWER_CTL, ADXL345_REG_POWER_CTL_MEASURE);

#endif
    

#if 1
    //Setup interrupts for accelerometer
        //even
    GPIO_PinModeSet(ADXL345_INT1_PORT, ADXL345_INT1_PIN,
        gpioModeInputPull, false);
    GPIO_ExtIntConfig(ADXL345_INT1_PORT, ADXL345_INT1_PIN, ADXL345_INT1_PIN,
        true, false, true);

        //odd
    GPIO_PinModeSet(ADXL345_INT2_PORT, ADXL345_INT2_PIN,
        gpioModeInputPull, false);
    GPIO_ExtIntConfig(ADXL345_INT2_PORT, ADXL345_INT2_PIN, ADXL345_INT2_PIN,
        true, false, true);
#endif


//Interrupts on
   //same thing as above GPIO_IntEnable( ADXL345_INT1_PIN <<1 | ADXL345_INT2_PIN <<1 );
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);

}

void adxl345_power_on() {
    GPIO_PinOutSet(ADXL345_POWER_PORT, ADXL345_POWER_PIN);
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

uint8_t adxl345_fifo_depth() {
uint8_t d=0;

    d = i2c_read_register(ADXL345_REG_FIFO_STATUS);    
    if (d & ADXL345_REG_FIFO_STATUS_FIFO_TRIG) {
        return (d & ADXL345_REG_FIFO_STATUS_ENTRIES_MASK);
    }

return 0;
}

uint8_t adxl345_fifo_full() {

return (i2c_read_register(ADXL345_REG_FIFO_STATUS) & ADXL345_REG_FIFO_STATUS_FIFO_TRIG);

}
