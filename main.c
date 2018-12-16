/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include "em_core.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "leds.h"
#include "accelerometer.h"
#include "gps.h"

#include "sleep.h" //TODO remove me

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

#define BLE_LATITUDE_LEN  		11
#define BLE_LONGITUDE_LEN       11
#define BLE_ALTITUDE_LEN        6
#define BLE_EW_INDICATOR_LEN    2
#define GPS_DATA_LEN    		30
uint8_t gps_data[GPS_DATA_LEN] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4'};

/**
 * @brief  Main function
 */
void main(void)
{
	// Initialize device
	initMcu();
	// Initialize board
	initBoard();
	// Initialize application
	initApp();

	led_on(LED0);

	// Initialize stack
	gecko_init(&config);

	//Can't connect to BLE without disabling these
	SLEEP_SleepBlockBegin(sleepEM2); // EM3 and EM4 are blocked

	uart_print_string(USART1,"CLS ready.\n\r");

#if 0
	//Test UART0
	while(1) {

		char hello_world[] = "Hello world.";
		for (int i=0; i< strlen(hello_world); i++) {
			USART_Tx(USART1,hello_world[i]);
		}

		//USART_Tx(USART0,USART_Rx(USART0));
	}
#endif

#if 0 //Testing accelerometer data ready interrupt
	xyz_data acc_d;

	char string_buff1[35];

	while(1) {
	int8_t d;
		if (accel_int1 == 1) {
			//Keep reading until empty, then the accelerator interrupt is cleared
			//This actually doesn't work
			//while(i2c_read_register(ADXL345_REG_INT_SOURCE) & ADXL345_REG_INT_SOURCE_DATA_READY) {
			//while(1) {
				adxl345_read_xyz(&acc_d); //This seems to clear the interrupt when sampling low rate
				snprintf(string_buff1,35,"x: %+0.6d y: %+0.6d z: %+0.6d\n\r", acc_d.x, acc_d.y, acc_d.z);
				uart_print_string(USART1,string_buff1);
			//}
			uart_print_string(USART1,"---\n\r");
			CORE_ATOMIC_IRQ_DISABLE();
			accel_int1 = 0;
			CORE_ATOMIC_IRQ_ENABLE();
		}
	}
#endif

#if 0 //GPS/accelerometer send
	while(1) {

	}
#endif


#if 0 //testing general GPIO interrupts
	while(1) {
		if (accel_int1 == 1) {
			uart_print_string(USART1,"int1\n\r");
			CORE_ATOMIC_IRQ_DISABLE();
			accel_int1 = 0;
			CORE_ATOMIC_IRQ_ENABLE();
		}
		if (accel_int2 == 1) {
			uart_print_string(USART1,"int2\n\r");
			CORE_ATOMIC_IRQ_DISABLE();
			accel_int2 = 0;
			CORE_ATOMIC_IRQ_ENABLE();
		}
	}
#endif

#if 0 //Test GPS TX -> UART1
	while(1) {
		USART_Tx(USART1,USART_Rx(USART0));
	}
#endif

#if 0 //Test GPS data parser
	while (1) {
		gps_run_main_seq();

		uart_print_string(USART1, "latitude:");
		uart_print_string(USART1, nmea_gps_coords.latitude);
		uart_print_string(USART1, "\n\r");

		uart_print_string(USART1, "longitude:");
		uart_print_string(USART1, nmea_gps_coords.longitude);
		uart_print_string(USART1, "\n\r");

		uart_print_string(USART1, "altitude:");
		uart_print_string(USART1, nmea_gps_coords.altitude);
		uart_print_string(USART1, "\n\r");

		uart_print_string(USART1, "ns:");
		uart_print_string(USART1, nmea_gps_coords.ns_indicator);
		uart_print_string(USART1, "\n\r");

		uart_print_string(USART1, "ew:");
		uart_print_string(USART1, nmea_gps_coords.ew_indicator);
		uart_print_string(USART1, "\n\r");
	}
#endif

	int send_ble_data=0;
	int count =0;

	led_off(LED0);
	led_off(LED1);
	led_off(LED2);

    while(1) {
        if (accel_int1 == 1) {
            adxl345_interrupts_off();
            uint8_t reason = i2c_read_register(ADXL345_REG_INT_SOURCE);
            if (reason & ADXL345_REG_INT_SOURCE_ACTIVITY) {
                uart_print_string(USART1,"moving\n\r");

                //Inactivity or Motion detection now
                i2c_write_register_1_byte(ADXL345_REG_ACT_INACT_CTL,
                        ADXL345_REG_ACT_X | ADXL345_REG_ACT_Y | ADXL345_REG_ACT_Z |
                        ADXL345_REG_INACT_X | ADXL345_REG_INACT_Y | ADXL345_REG_INACT_Z);
            }

            if (reason & ADXL345_REG_INT_SOURCE_INACTIVITY) {
                //gps_power_on(); //Boo! resets board
                uart_print_string(USART1,"stopped\n\r");
                //gps_print_location();
                gps_run_main_seq();
                uart_print_string(USART1,"got data...");

                //Format GPS for BLE transmission
                char* p=gps_data;
                strncpy(p, nmea_gps_coords.latitude,BLE_LATITUDE_LEN);
                p+=BLE_LATITUDE_LEN;
                strncpy(p, nmea_gps_coords.longitude,BLE_LONGITUDE_LEN);
                p+=BLE_LONGITUDE_LEN;
                strncpy(p, nmea_gps_coords.altitude,BLE_ALTITUDE_LEN);
                p+=BLE_ALTITUDE_LEN;
                strncpy(p, nmea_gps_coords.ew_indicator,BLE_EW_INDICATOR_LEN);

                //gps_power_off();
                send_ble_data = 1;
                if(count>1) {
                    SLEEP_SleepBlockBegin(sleepEM2); // EM2 and EM3 and EM4 are blocked due to needing additional clock accuracy
                    gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
                }

                //Motion detection only now
                i2c_write_register_1_byte(ADXL345_REG_ACT_INACT_CTL,
                        ADXL345_REG_ACT_X | ADXL345_REG_ACT_Y | ADXL345_REG_ACT_Z);
            }

            CORE_ATOMIC_IRQ_DISABLE();
            accel_int1 = 0;
            CORE_ATOMIC_IRQ_ENABLE();
            adxl345_motion_int_on();
        }

        while (send_ble_data) {
            /* Event pointer for handling events */
            struct gecko_cmd_packet* evt;

            /* Check for stack event. */
            evt = gecko_wait_event();

            /* Handle events */
            switch (BGLIB_MSG_ID(evt->header)) {
                /* This boot event is generated when the system boots up after reset.
                * Do not call any stack commands before receiving the boot event.
                * Here the system is set to start advertising immediately after boot procedure. */
                case gecko_evt_system_boot_id:
                     SLEEP_SleepBlockBegin(sleepEM2); // EM2 and EM3 and EM4 are blocked due to needing additional clock accuracy

                    /* Set advertising parameters. 100ms advertisement interval.
                     * The first parameter is advertising set handle
                     * The next two parameters are minimum and maximum advertising interval, both in
                     * units of (milliseconds * 1.6).
                     * The last two parameters are duration and maxevents left as default. */
                    gecko_cmd_le_gap_set_advertise_timing(0, 160, 160, 0, 0);

                    /* Start general advertising and enable connections. */
                    gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
                    led_on(LED0);
                    count++;
                    break;

                case gecko_evt_le_connection_closed_id:

                    /* Check if need to boot to dfu mode */
                    if (boot_to_dfu) {
                        /* Enter to DFU OTA mode */
                        gecko_cmd_system_reset(2);
                    } else {
                        ///* Restart advertising after client has disconnected */
                        //gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
                        SLEEP_SleepBlockEnd(sleepEM2);
                        send_ble_data = 0;
                        led_off(LED0);
                    }
                    break;

                /* Events related to OTA upgrading
                 ----------------------------------------------------------------------------- */

                /* Check if the user-type OTA Control Characteristic was written.
                * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
                case gecko_evt_gatt_server_user_write_request_id:

                    if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
                      /* Set flag to enter to OTA mode */
                      boot_to_dfu = 1;
                      /* Send response to Write Request */
                      gecko_cmd_gatt_server_send_user_write_response(
                              evt->data.evt_gatt_server_user_write_request.connection,
                              gattdb_ota_control,
                              bg_err_success);

                      /* Close connection to enter to DFU OTA mode */
                      gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
                    }
                    break;
                case gecko_evt_gatt_server_characteristic_status_id:
                    gecko_cmd_gatt_server_send_characteristic_notification(
                            evt->data.evt_gatt_server_user_write_request.connection, gattdb_position_gps, GPS_DATA_LEN, gps_data);
                    //once the notification occurs, we should disconnect (and leave ble loop) to save energy
                    gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);

                    break;
                default:
                    break;
            }
        }
    }

}



/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
