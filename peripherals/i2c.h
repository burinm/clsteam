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

void I2C0_init();
void performI2CTransfer(void);

#endif
