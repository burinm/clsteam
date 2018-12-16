/***************************************************************************//**
 * @file init_mcu.c
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

#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_rtcc.h"

#include "bsp.h"

#include "tempdrv.h"

// Bit [19] in MODULEINFO is the HFXOCALVAL:
// 1=No factory cal, use default XO tunning value in FW
// 0=Factory Cal, use XO tunning value in DI 
#define DEVINFO_MODULEINFO_HFXOCALVAL_MASK  0x00080000UL
// Calibration value for HFXO CTUNE is at DEVINFO Offset 0x08
#define DEVINFO_MODULEINFO_CRYSTALOSCCALVAL  (*((uint16_t *) (uint32_t)(DEVINFO_BASE+0x8UL)))
// [15:9] : (LFXOTUNING) Calibration for LFXO TUNING
// [8:0]  : (HFXOCTUNE) Calibration for HFXO CTUNE
#define DEVINFO_HFXOCTUNE_MASK  0x01FFUL

static void initMcu_clocks(void);

void initMcu(void)
{
  // Device errata
  CHIP_Init();

    EMU_DCDCPowerOff();
#if 0
  // Set up DC-DC converter
  EMU_DCDCInit_TypeDef dcdcInit = BSP_DCDC_INIT;
  #if HAL_DCDC_BYPASS
  dcdcInit.dcdcMode = emuDcdcMode_Bypass;
  #endif
  EMU_DCDCInit(&dcdcInit);
#endif

  // Set up clocks
  initMcu_clocks();

  RTCC_Init_TypeDef rtccInit = RTCC_INIT_DEFAULT;
  rtccInit.enable                = true;
  rtccInit.debugRun              = false;
  rtccInit.precntWrapOnCCV0      = false;
  rtccInit.cntWrapOnCCV1         = false;
  rtccInit.prescMode             = rtccCntTickPresc;
  rtccInit.presc                 = rtccCntPresc_1;
  rtccInit.enaOSCFailDetect      = false;
  rtccInit.cntMode               = rtccCntModeNormal;
  RTCC_Init(&rtccInit);

#if defined(_EMU_CMD_EM01VSCALE0_MASK)
  // Set up EM0, EM1 energy mode configuration
  EMU_EM01Init_TypeDef em01Init = EMU_EM01INIT_DEFAULT;
  EMU_EM01Init(&em01Init);
#endif // _EMU_CMD_EM01VSCALE0_MASK

#if defined(_EMU_CTRL_EM23VSCALE_MASK)
  // Set up EM2, EM3 energy mode configuration
  EMU_EM23Init_TypeDef em23init = EMU_EM23INIT_DEFAULT;
  em23init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init(&em23init);
#endif //_EMU_CTRL_EM23VSCALE_MASK

  TEMPDRV_Init();
}

static void initMcu_clocks(void)
{
  // Initialize HFXO
  CMU_HFXOInit_TypeDef hfxoInit = BSP_CLK_HFXO_INIT;
  // if Factory Cal exists in DEVINFO then use it
  if (0==(DEVINFO->MODULEINFO & DEVINFO_MODULEINFO_HFXOCALVAL_MASK)) {
     hfxoInit.ctuneSteadyState = DEVINFO_MODULEINFO_CRYSTALOSCCALVAL & DEVINFO_HFXOCTUNE_MASK;
  }
#if defined(BSP_CLK_HFXO_CTUNE) && BSP_CLK_HFXO_CTUNE > -1
  else {
    hfxoInit.ctuneSteadyState = BSP_CLK_HFXO_CTUNE;
  }
#endif
  CMU_HFXOInit(&hfxoInit);

  // Set system HFXO frequency
  SystemHFXOClockSet(BSP_CLK_HFXO_FREQ);

  // Enable HFXO oscillator, and wait for it to be stable
  CMU_OscillatorEnable(cmuOsc_HFXO, true, true);

  // Enable HFXO Autostart only if EM2 voltage scaling is disabled.
  // In 1.0 V mode the chip does not support frequencies > 21 MHz,
  // this is why HFXO autostart is not supported in this case.
#if!defined(_EMU_CTRL_EM23VSCALE_MASK)
  // Automatically start and select HFXO
  CMU_HFXOAutostartEnable(0, true, true);
#else
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
#endif//_EMU_CTRL_EM23VSCALE_MASK

  // HFRCO not needed when using HFXO
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  // Enabling HFBUSCLKLE clock for LE peripherals
  CMU_ClockEnable(cmuClock_HFLE, true);

  // Initialize LFXO
  CMU_LFXOInit_TypeDef lfxoInit = BSP_CLK_LFXO_INIT;
  //lfxoInit.ctune = BSP_CLK_LFXO_CTUNE;
  //CMU_LFXOInit(&lfxoInit);

/*
TIMEOUT 0x1 RW LFRCO Timeout
Configures the start-up delay for LFRCO. Do not change while LFRCO is enabled. When starting up the LFRCO after it has
been completely turned off, use TIMEOUT=16cycles. If the LFRCO has been retained on in EM4, then the TIMEOUT=2cy-
cles configuration is also allowed when re-enabling the LFRCO after EM4 exit (as it is still running).
Value 0 1 2
Mode 2CYCLES 16CYCLES 32CYCLES
*/

   CMU->LFRCOCTRL |= (1 <<_CMU_LFRCOCTRL_TIMEOUT_SHIFT); //16 cycles


  // Set system LFXO frequency
  //SystemLFXOClockSet(BSP_CLK_LFXO_FREQ);
  //SystemLFXOClockSet(BSP_CLK_LFXO_FREQ);

  // Enable LFRCO oscillator, and wait for it to be stable
  CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

///ODO burin - do we need this? 
  //CMU_ClockEnable(cmuClock_LFE, true);

  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFRCO);
  CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFRCO);
  // Set LFXO if selected as LFCLK
  //CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
  //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
  //CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);
}
