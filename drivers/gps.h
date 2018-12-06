#ifndef __GPS_H__
#define __GPS_H__
/*
Driver for Venus638FLPx GPS Receiver
    burin, 2018/9/26
*/

#include <stdint.h>

/* Skytraq binary message protocol

start(2)    length(2) id(1)        (1-65535)      checksum(1) end(2)
<0xA0,0xA1> <PL>      <Message ID> <Message Body> <CS>        <0x0D,0x0A>

From datasheet:

Checksum (CS) field is transmitted in all messages. The checksum field is the last field in a message before the end of sequence field. The checksum is the 8-bit exclusive OR of only the payload bytes which start from Message ID until the last byte prior to the checksum byte.


*/

//Message Begin/End
#define SKYTRAQ_MARK_START  { 0xa0, 0xa1 }
#define SKYTRAQ_MARK_END    { 0x0d, 0x0a }

#define SKYTRAQ_ATT_UPDATE_SRAM         0x0
#define SKYTRAQ_ATT_UPDATE_SRAM_FLASH   0x1

//Input message IDs
#define SKYTRAQ_MSG_IN_RESET             0x1
    typedef struct {
        uint8_t message_id;
        uint8_t start_mode;
        uint16_t utc_year;
        uint8_t utc_month;
        uint8_t utc_day;
        uint8_t utc_hour;
        uint8_t utc_minute;
        uint8_t utc_second;
        int16_t latitude;
        int16_t longitude;
        int16_t altitude;
    } skytraq_reset_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_IN_FACTORY_DEFAULT   0x4
    typedef struct {
        uint8_t message_id;
        uint8_t type;
    } skytraq_factory_default_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_IN_CONFIG_SERIAL     0x5
    typedef struct {
        uint8_t message_id;
        uint8_t COM_port;
        uint8_t baud_rate;
            #define SKYTRAQ_BAUD_4800               0x0
            #define SKYTRAQ_BAUD_9600               0x1
            #define SKYTRAQ_BAUD_19200              0x2
            #define SKYTRAQ_BAUD_38400              0x3
            #define SKYTRAQ_BAUD_57600              0x4
            #define SKYTRAQ_BAUD_115200             0x5
        uint8_t attributes
    } skytraq_config_serial_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_IN_CONFIG_NMEA       0x8
    typedef struct {
        uint8_t message_id;
        uint8_t GGA_interval; //all in seconds 0(disable) - 255
        uint8_t GSA_interval;
        uint8_t GSV_interval;
        uint8_t GLL_interval;
        uint8_t RMC_interval;
        uint8_t VTG_interval;
        uint8_t ZDA_interval;
        uint8_t attributes;
    } skytraq_config_nmea_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_IN_OUTPUT_FORMAT     0x9
    typedef struct {
        uint8_t message_id;
        uint8_t type;
            #define SKYTRAQ_MSG_TYPE_NONE   0x0
            #define SKYTRAQ_MSG_TYPE_NMEA   0x1
            #define SKYTRAQ_MSG_TYPE_BINARY 0x2
    } skytraq__msg_output_format_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_IN_POS_CONFIG_UPDATE_RATE   0xe
    typedef struct {
        uint8_t message_id;
        uint8_t rate; /* in Hz 1,2,4,5,8,10,20 - 4-10 for > 38400 baud, 20 for 115200 baud */
        uint8_t attributes;
    } skytraq_config_update_rate_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_IN_CONFIG_DATUM      0x29
    typedef struct {
        uint8_t message_id;
        uint16_t index;
        uint8_t ellip_idx;
        int16_t delta_x;
        int16_t delta_y;
        int16_t delta_z;
        uint32_t semi_major_axis;
        uint32_t inversed_flattening;
        uint8_t attributes;
    } skytraq_config_datum_msg_t __attribute__((__packed__,gcc_struct));


//Output message IDs
#define SKYTRAQ_MSG_OUT_ACK              0x83
    typedef struct {
        uint8_t message_id;
        uint8_t ack_id;
    } skytraq_out_ack_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_OUT_NACK             0x84
    typedef struct {
        uint8_t message_id;
        uint8_t ack_id;
    } skytraq_out_nack_msg_t __attribute__((__packed__,gcc_struct));

#define SKYTRAQ_MSG_OUT_DATUM            0xae
    typedef struct {
        uint8_t message_id;
        uint16_t datum_index;
    } skytraq_data_msg_t __attribute__((__packed__,gcc_struct));


void open();
void close();

void init_gps();
void gps_run_main_seq();
void gps_wait_for_valid_data();
void gps_get_nmea_message();
void shutdown_gps();


#endif
