#include "accelerometer.h"



// \CS pin high - for I2C
// Pullups required - UM10204 I2C-Bus Specification and User Manual, Rev. 03—19 June 2007
// Max 400kHz


//Disable interrupts before configuring

//5us between reading updated values (I2C is slow anyway)
