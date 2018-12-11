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
#include "leds.h"
#include "accelerometer.h"


#warning "WARNING: Custom boards contain no init code in initBoard. Please make sure you have created the init code needed for your board."
void initBoard(void)
{
  // Enable clock for USART0
  CMU_ClockEnable(cmuClock_USART0, true);
  // Enable GPIO clock source
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Place custom board initialization code here.
  CMU_ClockEnable(cmuClock_I2C0, true);

  init_leds();

  //Setup power port for accelerator PB13
  GPIO_DriveStrengthSet(ADXL345_POWER_PORT, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(ADXL345_POWER_PORT, ADXL345_POWER_PIN, gpioModePushPull, false);
  adxl345_power_on();

  I2C0_init();
}

