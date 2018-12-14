#ifndef __NMEA_H__
#define __NMEA_H__

#include <stdint.h>

// DEFINES

#define NMEA_MAX_LEN 82

#define NMEA_TYPE_INDEX 0

//The following field is based on observed field lengths that come from the GPS
#define NMEA_FIELD_MAX_LEN 12
#define NMEA_FIELD_IND_LEN 2

#define GGA "$GNGGA\0" //Global Positioning System Fix Data
    #define UTC_TIME        1
    #define LATITUDE        2
    #define NS_INDICATOR    3
    #define LONGITUDE       4
    #define EW_INDICATOR    5
    #define GPS_QUALITY     6
    #define SATILITES_USED  7
    #define HDOP            8
    #define ALTITUDE        9
    #define DGPS_STATION_ID 10
    #define CHECKSUM        11

#define GLL "$GPGLL\0" //Latitude/Longitude
#define GSA "$GPGSA\0" //GNSS DOP and Active Satellites
#define GSV "$GPGSV\0" //GNSS Satellites in View
#define RMC "$GPRMC\0" //Recommended Minimum Specific GNSS Data
#define VTG "$GPVTG\0" //Course Over Ground and Ground Speed

//STRUCTS

typedef struct {
    char latitude[NMEA_FIELD_MAX_LEN];
    char ns_indicator[NMEA_FIELD_IND_LEN];
    char longitude[NMEA_FIELD_MAX_LEN];
    char ew_indicator[NMEA_FIELD_IND_LEN];
    char altitude[NMEA_FIELD_MAX_LEN];
} nmea_gps_coords_t;

//FUNCTIONS

uint8_t nmea_add_char_to_message(char c);
uint8_t nmea_check_message_complete();
void nmea_clear_message();
uint8_t nmea_parse_message();
uint8_t nmea_str_split(unsigned int index);
uint8_t nmea_field_comp(const char *c);
uint8_t nmea_field_copy(char dest[NMEA_FIELD_MAX_LEN]);
uint8_t nmea_field_copy_short(char dest[NMEA_FIELD_IND_LEN]);

//MEMBERS
extern char nmea_string[NMEA_MAX_LEN];
extern char nmea_field[NMEA_FIELD_MAX_LEN];
extern unsigned int nmea_string_index;
extern nmea_gps_coords_t nmea_gps_coords;

#endif
