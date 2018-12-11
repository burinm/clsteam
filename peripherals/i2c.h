#ifndef __I2C_H__
#define __I2C_H__

#include <em_gpio.h>
#include "../drivers/accelerometer.h"


//accelerometer interface
/*                                                 BRD4302A pins
                                                   -------------
*/
#define I2C0_ADDR   ADXL345_I2C_ADDR_ALT

#define I2C0_SDA_LOCATION    I2C_ROUTELOC0_SDALOC_LOC5   //          PA5 -  P16
#define I2C0_SDA_PORT        gpioPortA 
#define I2C0_SDA_PIN         5

#define I2C0_SCL_LOCATION    I2C_ROUTELOC0_SCLLOC_LOC3   //          PA4 -  P14
#define I2C0_SCL_PORT        gpioPortA
#define I2C0_SCL_PIN         4 

#define ACCELEROMETER_CS     PB13 //             - P22
#define ACCELEROMETER_INT1   PB12 //             - P20
#define ACCELEROMETER_INT2   PB11 //             - P18

#define I2C_RX_BUFF_SZ  256
extern uint8_t i2c_RX_buff[I2C_RX_BUFF_SZ];
extern uint16_t i2c_RX_count;

#define I2C_CMD_ARRAY_SZ    1
extern uint8_t i2c_cmd_array[I2C_CMD_ARRAY_SZ];
#define I2C_DATA_ARRAY_SZ    10
extern uint8_t i2c_data_array[I2C_DATA_ARRAY_SZ];

void I2C0_init();
void performI2CTransfer(void);
void i2c_transfer(uint16_t device_addr, uint8_t cmd_array[], uint8_t data_array[], uint16_t cmd_len, uint16_t data_len, uint8_t flag);
uint8_t i2c_read_register(uint8_t reg_offset);
void i2cRead();

#endif
