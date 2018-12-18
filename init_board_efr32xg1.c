/***************************************************************************//**
 * @file init_board.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#include "hal-config.h"
#else
#include "bspconfig.h"
#endif

#include "board_features.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "main.h"
#include "leds.h"
#include "accelerometer.h"
#include "gps.h"


#warning "WARNING: Custom boards contain no init code in initBoard. Please make sure you have created the init code needed for your board."
void initBoard(void)
{
  //Reset accelerometer
  //adxl345_power_off();

  // Enable clock for USART0
  CMU_ClockEnable(cmuClock_USART0, true);
  // Enable GPIO clock source
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Place custom board initialization code here.
  CMU_ClockEnable(cmuClock_I2C0, true);

  //Port D will be used to drive LEDS and GPS load switch
  //*We might have a bug, where our input button on PD13 is connected to this port*
  GPIO_DriveStrengthSet(gpioPortD, gpioDriveStrengthStrongAlternateStrong);

  //Setup load switch pin for GPS
  GPIO_PinModeSet(GPS_POWER_PORT, GPS_POWER_PIN, gpioModePushPull, false);

  //Turn on GPS early, big current inrush
  gps_power_on();

  init_leds();


#if 1
  UART0_init();
  USART_Enable(USART0,usartEnable);
#endif

#ifdef DEBUG_ON
  UART1_init();
  USART_Enable(USART1,usartEnable);
#endif

  //Setup power port for acceleromter PB13 - port B
  GPIO_DriveStrengthSet(ADXL345_POWER_PORT, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(ADXL345_POWER_PORT, ADXL345_POWER_PIN, gpioModePushPull, false);

  adxl345_power_on();

  I2C0_init();

#if 1 //Can we talk to accelerometer?
uint16_t adxl345_id=0;
if (! adxl345_get_device_id(&adxl345_id)) {
    led_on(LED1);
    for(;;);
}
#endif

/*
    On a warm boot, the capicitors keep the memory alive,
    Reset to defaults from data sheet
*/
adxl345_defaults();

//Setup accelerometer interrupts
adxl345_setup_interrupts();
//adxl345_setup_for_xyz();
adxl345_setup_for_motion();

}

