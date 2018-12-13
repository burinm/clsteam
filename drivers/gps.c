#include "gps.h"
#include "nmea.h"
#include "em_emu.h"
#include "em_usart.h"
#include "em_gpio.h"

char latitude[11];
char longitude[11];
char altitude[11];

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
