#include "gps.h"
#include "nmea.h"
#include "em_emu.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "usart.h"

//char latitude[11];
//char longitude[11];
//char altitude[11];

char nmea_string[NMEA_MAX_LEN];
char nmea_field[NMEA_FIELD_MAX_LEN];
unsigned int nmea_string_index;
nmea_gps_coords_t nmea_gps_coords;


void gps_power_on() {
    GPIO_PinOutSet(GPS_POWER_PORT, GPS_POWER_PIN);
}

void gps_power_off() {
    GPIO_PinOutClear(GPS_POWER_PORT, GPS_POWER_PIN);
}

void init_gps()
{
  //GPIO_PortOutSet();
}

void gps_run_main_seq()
{
  gps_wait_for_valid_data();
}

void gps_wait_for_valid_data()
{
  do
  {
    gps_get_nmea_message();
  }while(nmea_parse_message()==0);
}

void gps_get_nmea_message()
{
  do
  {
    USART_Rx(USART0);
    nmea_add_char_to_message((char)USART0->RXDATA);
  }while(nmea_check_message_complete()==0);
}

void shutdown_gps()
{
  //GPIO_PortOutClear();
}

void gps_print_location() {
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
