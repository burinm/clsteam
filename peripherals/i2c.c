/* The majority of this code copied and modified from 

https://github.com/SiliconLabs/makersguide

 Copyright Silicon Labs, http://www.silabs.com
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
*/


#include "i2c.h"
#include <em_i2c.h>
#include <em_gpio.h>
#include <em_cmu.h>

#include <stdlib.h> //NULL
#include "accelerometer.h"
#include "leds.h" //TODO remove me

uint8_t i2c_RX_buff[I2C_RX_BUFF_SZ];
uint16_t i2c_RX_count = 0;
uint8_t i2c_cmd_array[I2C_CMD_ARRAY_SZ];
uint8_t i2c_data_array[I2C_DATA_ARRAY_SZ];


void I2C0_init() {
/* Parts copied verbatim from i2c_master_slave.c (an/an0011_efm32_i2c)
    brief I2C Demo Application
    author Silicon Labs
    version 1.06
    (C) Copyright 2014 Silicon Labs, http://www.silabs.com

  Parts copied verbatim from 
    https://github.com/SiliconLabs/peripheral_examples/blob/public/i2c/i2c/src/main_efr.c
    Copyright 2018 Silicon Labs, Inc. http://www.silabs.com
    1509ef8 on Aug 22, silabs-BrianL Summer 2018 update
*/


  /* Using PA5 (SDA) and PA4 (SCL) */
  GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);
  GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);

//This doesn't work after re-routing pins, do here

    // From Kathy Spring 2018 Fincal project IOT (c) 2018

    //SCL AND SDA GPIO pins have been enabled in gpio_init
    //Need to toggle I2C SCL 9 times to reset any I2C slave that may require it
    //due to possible glitching
    //From Lecture 6 slide 16
  for (uint8_t i=0; i<9; i++) {
      GPIO_PinOutClear(I2C0_SCL_PORT, I2C0_SCL_PIN);
      GPIO_PinOutSet(I2C0_SCL_PORT, I2C0_SCL_PIN);
  }

  I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
  I2C0->ROUTELOC0 = (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK)) |
                        I2C0_SDA_LOCATION;
  I2C0->ROUTELOC0 = (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK)) |
                        I2C0_SCL_LOCATION;
        
  //After Resetting, bus and peripherals may be out of synch
  //CHeck for busy state (Lecture 6 slide 18)
  if(I2C0->STATE & I2C_STATE_BUSY){
      I2C0->CMD = I2C_CMD_ABORT;
  }

  /* Initializing the I2C */

 // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT; //Standard max 100Hz
  I2C_Init(I2C0, &i2cInit);

#if 0
  /* Setting the status flags and index */
  i2c_rxInProgress = false;
  i2c_startTx = false;
  i2c_rxBufferIndex = 0;

  /* Setting up to enable slave mode */
  I2C0->SADDR = I2C_ADDRESS;
  I2C0->CTRL |= I2C_CTRL_SLAVE | I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;
  enableI2cSlaveInterrupts();
#endif
  //I2C0->CTRL |= I2C_CTRL_EN; 

}

void performI2CTransfer(void)
{
/* Taken verbatim from https://github.com/SiliconLabs/peripheral_examples/blob/public/i2c/i2c/src/main_efr.c

    Copyright 2018 Silicon Labs, Inc. http://www.silabs.com
*/

  // Transfer structure
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;

  // Setting LED to indicate transfer
  //GPIO_PinOutSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);


/* testing */
uint8_t i2c_txBuffer[] = "Hello I2C";
uint8_t i2c_txBufferSize = sizeof(i2c_txBuffer);
uint8_t i2c_rxBuffer = NULL;
#define I2C_RXBUFFER_SIZE 0;

  // Initializing I2C transfer
  i2cTransfer.addr          = I2C0_ADDR;
  i2cTransfer.flags         = I2C_FLAG_WRITE;
  i2cTransfer.buf[0].data   = i2c_txBuffer;
  i2cTransfer.buf[0].len    = i2c_txBufferSize;
  i2cTransfer.buf[1].data   = i2c_rxBuffer;
  i2cTransfer.buf[1].len    = I2C_RXBUFFER_SIZE;
  result = I2C_TransferInit(I2C0, &i2cTransfer);

  // Sending data
  while (result == i2cTransferInProgress)
  {
    result = I2C_Transfer(I2C0);
  }

  // Clearing pin to indicate end of transfer
  //GPIO_PinOutClear(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
  //enableI2cSlaveInterrupts();
}

/* Copied verbatim and modified from Silcon Labs community blog
    Chapter 10.2: Control an accelerometer over I2C Part 2 - Initialize the I2C Bus
    https://www.silabs.com/community/blog.entry.html/2016/03/09/chapter_10_2_contro-l8qA
*/
/* https://github.com/SiliconLabs/makersguide/blob/master/10_i2c_accelerometer/src/main_read_data.c */
void i2c_transfer(uint16_t device_addr, uint8_t cmd_array[], uint8_t data_array[], uint16_t cmd_len, uint16_t data_len, uint8_t flag) {
      // Transfer structure
      I2C_TransferSeq_TypeDef i2cTransfer;

      // Initialize I2C transfer
      I2C_TransferReturn_TypeDef result;
      i2cTransfer.addr          = (device_addr <<1);
      i2cTransfer.flags         = flag;
      i2cTransfer.buf[0].data   = cmd_array;
      i2cTransfer.buf[0].len    = cmd_len;

      // Note that WRITE_WRITE this is tx2 data
      i2cTransfer.buf[1].data   = data_array;
      i2cTransfer.buf[1].len    = data_len;

      // Set up the transfer
      result = I2C_TransferInit(I2C0, &i2cTransfer);

      // Do it until the transfer is done
      while (result != i2cTransferDone)
      {
            if (result != i2cTransferInProgress)
            {
                  led_on(LED2);
                  for(;;);
                  //TODO add blinky code?
            }
            result = I2C_Transfer(I2C0);
      }
}

/* https://www.silabs.com/community/blog.entry.html/2016/03/09/chapter_10_2_contro-l8qA */
/* https://github.com/SiliconLabs/makersguide/blob/master/10_i2c_accelerometer/src/main_read_data.c */
uint8_t i2c_read_register(uint8_t reg_offset) {
      i2c_cmd_array[0] = reg_offset;
      i2c_transfer(ADXL345_I2C_ADDR_ALT, i2c_cmd_array, i2c_data_array, 1, 1, I2C_FLAG_WRITE_READ);
      return i2c_data_array[0];
}

uint8_t i2c_read_bytes(uint8_t reg_offset, uint8_t n) {
if (n > I2C_DATA_ARRAY_SZ) {
    n = I2C_DATA_ARRAY_SZ;
}

      i2c_cmd_array[0] = reg_offset;
      i2c_transfer(ADXL345_I2C_ADDR_ALT, i2c_cmd_array, i2c_data_array, 1, n, I2C_FLAG_WRITE_READ);
      return i2c_data_array[0];
}

/* https://github.com/SiliconLabs/makersguide/blob/master/10_i2c_accelerometer/src/main_interrupt_on_event_energy_profiler.c */
void i2c_write_register_1_byte(uint8_t reg_offset, uint8_t write_data) {
	i2c_cmd_array[0] = reg_offset;
	i2c_data_array[0] = write_data;
	i2c_transfer(ADXL345_I2C_ADDR_ALT, i2c_cmd_array, i2c_data_array, 1, 1, I2C_FLAG_WRITE_WRITE);
}

#if 0 //not tested
void i2c_write_register_bytes(uint8_t reg_offset, uint8_t write_data, uint8_t n) {
	i2c_cmd_array[0] = reg_offset;
	i2c_data_array[0] = write_data;
	i2c_transfer(ADXL345_I2C_ADDR_ALT, i2c_cmd_array, i2c_data_array, 1, 2, I2C_FLAG_WRITE_WRITE);
}
#endif

void i2c_read() {

    while (1) {
        uint32_t status = I2C0->IF;
        if (status & I2C_IF_RXDATAV) {
            i2c_RX_buff[i2c_RX_count++] = I2C0->RXDATA;
            if (i2c_RX_count == I2C_RX_BUFF_SZ -1) {
                i2c_RX_buff[I2C_RX_BUFF_SZ-1] = '\0';
                break;
            }
        }
        if (status & I2C_IEN_SSTOP) {
            i2c_RX_buff[i2c_RX_count] = '\0';
            i2c_RX_count = 0;
            I2C_IntClear(I2C0, I2C_IEN_SSTOP);
            //i2c_rxInProgress = false;
            break;
        }
    }
}
