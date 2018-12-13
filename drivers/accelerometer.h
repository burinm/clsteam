#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <stdint.h>

#define ADXL345_I2C_ADDR            0x1D // SDO/ALT ADDRESS pin high
#define ADXL345_I2C_ADDR_ALT        0x53 //   "                 low 

#define ADXL345_REG_DEVID           0x0
    #define ADXL345_REG_DEVID_ID        0xE5
                                  //0x1 - 0x1C reserved
#define ADXL345_REG_THRESH_TAP      0x1D
#define ADXL345_REG_OFSX            0x1E
#define ADXL345_REG_OFSY            0x1F
#define ADXL345_REG_OFSZ            0x20
#define ADXL345_REG_DUR             0x21
#define ADXL345_REG_LATENT          0x22
#define ADXL345_REG_WINDOW          0x23 
#define ADXL345_REG_THRESH_ACT      0x24 //62.5 mg/LSB - don't set to 0 when int enabled
#define ADXL345_REG_THRESH_INACT    0x25 //62.5 mg/LSB - don't set to 0 when int enabled
#define ADXL345_REG_TIME_INACT      0x26 //1s/LSB - don't set less than time constant of data out rate
#define ADXL345_REG_ACT_INACT_CTL   0x27
    #define ADXL345_REG_ACT_AC          (1<<7) // 1=AC, 0=DC
    #define ADXL345_REG_ACT_X           (1<<6) // X register participates in activity
    #define ADXL345_REG_ACT_Y           (1<<5) // Y "
    #define ADXL345_REG_ACT_Z           (1<<4) // Z "
    #define ADXL345_REG_INACT_AC        (1<<3) // 1=AC, 0=DC
    #define ADXL345_REG_INACT_X         (1<<2) // X register participates in inactivity
    #define ADXL345_REG_INACT_Y         (1<<1) // Y "
    #define ADXL345_REG_INACT_Z         (1<<0) // Z "
#define ADXL345_REG_THRESH_FF       0x28 // Freefall threshold, 62.5mg/LSB - don't set to 0 when int enabled
                                         // recommended 300mg - 600mg
#define ADXL345_REG_TIME_FF         0x29 // 5ms/LSB - don't set to 0 when int enabled
#define ADXL345_REG_TAP_AXES        0x2A
    #define ADXL345_REG_TAP_AXES_D7_0   (1<<7) //always 0
    #define ADXL345_REG_TAP_AXES_D6_0   (1<<6) //always 0
    #define ADXL345_REG_TAP_AXES_D5_0   (1<<5) //always 0
    #define ADXL345_REG_TAP_AXES_D4_0   (1<<4) //always 0
    #define ADXL345_REG_TAP_AXES_SUPRESS   (1<<3)
    #define ADXL345_REG_TAP_AXES_TAP_X  (1<<2)
    #define ADXL345_REG_TAP_AXES_TAP_Y  (1<<1)
    #define ADXL345_REG_TAP_AXES_TAP_Z  (1<<0)

#define ADXL345_REG_ACT_TAP_STATUS  0x2B //Read only
    #define ADXL345_REG_ACT_TAP_STATUS_0           (1<<7) //always zero
    #define ADXL345_REG_ACT_TAP_STATUS_ACT_X       (1<<6)
    #define ADXL345_REG_ACT_TAP_STATUS_ACT_Y       (1<<5)
    #define ADXL345_REG_ACT_TAP_STATUS_ACT_Z       (1<<4)
    #define ADXL345_REG_ACT_TAP_STATUS_ASLEEP      (1<<3) // 1 = asleep, 0 = not asleep
    #define ADXL345_REG_ACT_TAP_STATUS_TAP_X       (1<<2)
    #define ADXL345_REG_ACT_TAP_STATUS_TAP_Y       (1<<1)
    #define ADXL345_REG_ACT_TAP_STATUS_TAP_Z       (1<<0)

#define ADXL345_REG_BW_RATE         0x2C
    #define ADXL345_REG_BW_RATE_D7_0                (1<<7)
    #define ADXL345_REG_BW_RATE_D6_0                (1<<6)
    #define ADXL345_REG_BW_RATE_D5_0                (1<<5)
    #define ADXL345_REG_BW_RATE_LOW_PWR             (1<<4) // 0 = normal, 1 = reduced power
        #define ADXL345_REG_BW_RATE_RATE_MASK               0xF //Default = 0xA, 100Hz 

#define ADXL345_REG_POWER_CTL       0x2D
    #define ADXL345_REG_POWER_CTL_D7_0              (1<<7) 
    #define ADXL345_REG_POWER_CTL_D6_0              (1<<6) 
    #define ADXL345_REG_POWER_CTL_LINK              (1<<5) // 1 = delays start of activity until inactivity
                                                           // Links activity/inactivity functions ***
    #define ADXL345_REG_POWER_CTL_AUTO_SLEEP        (1<<4) // 1 - sleep when inactivity detected
    #define ADXL345_REG_POWER_CTL_MEASURE           (1<<3) // 1 = measurement mode, 0 = standby mode
    #define ADXL345_REG_POWER_CTL_SLEEP             (1<<2) // 1 = sleep mode, 0 = normal mode
        #define ADXL345_REG_POWER_CTL_WAKEUP_MASK           0x3
            #define ADXL345_REG_POWER_CTL_WAKEUP_1_HZ           0x3
            #define ADXL345_REG_POWER_CTL_WAKEUP_2_HZ           0x2 
            #define ADXL345_REG_POWER_CTL_WAKEUP_4_HZ           0x1 
            #define ADXL345_REG_POWER_CTL_WAKEUP_8_HZ           0x0

#define ADXL345_REG_INT_ENABLE      0x2E // 1 = enable
    #define ADXL345_REG_INT_ENABLE_DATA_READY       (1<<7)
    #define ADXL345_REG_INT_ENABLE_SINGLE_TAP       (1<<6)
    #define ADXL345_REG_INT_ENABLE_DOUBLE_TAP       (1<<5)
    #define ADXL345_REG_INT_ENABLE_ACTIVITY         (1<<4)
    #define ADXL345_REG_INT_ENABLE_INACTIVITY       (1<<3)
    #define ADXL345_REG_INT_ENABLE_FREE_FALL        (1<<2)
    #define ADXL345_REG_INT_ENABLE_WATERMARK        (1<<1)
    #define ADXL345_REG_INT_ENABLE_OVERRUN          (1<<0)

#define ADXL345_REG_INT_MAP         0x2F // 1 = INT2, 0 = INT1
    #define ADXL345_REG_INT_MAP_DATA_READY          (1<<7)
    #define ADXL345_REG_INT_MAP_SINGLE_TAP          (1<<6)
    #define ADXL345_REG_INT_MAP_DOUBLE_TAP          (1<<5)
    #define ADXL345_REG_INT_MAP_ACTIVITY            (1<<4)
    #define ADXL345_REG_INT_MAP_INACTIVITY          (1<<3)
    #define ADXL345_REG_INT_MAP_FREE_FALL           (1<<2)
    #define ADXL345_REG_INT_MAP_WATERMARK           (1<<1)
    #define ADXL345_REG_INT_MAP_OVERRUN             (1<<0)

#define ADXL345_REG_INT_SOURCE      0x30 // Read only, 1 = triggered event, 0 = event not occurred
    #define ADXL345_REG_INT_SOURCE_DATA_READY       (1<<7)
    #define ADXL345_REG_INT_SOURCE_SINGLE_TAP       (1<<6)
    #define ADXL345_REG_INT_SOURCE_DOUBLE_TAP       (1<<5)
    #define ADXL345_REG_INT_SOURCE_ACTIVITY         (1<<4)
    #define ADXL345_REG_INT_SOURCE_INACTIVITY       (1<<3)
    #define ADXL345_REG_INT_SOURCE_FREE_FALL        (1<<2)
    #define ADXL345_REG_INT_SOURCE_WATERMARK        (1<<1)
    #define ADXL345_REG_INT_SOURCE_OVERRUN          (1<<0)

#define ADXL345_REG_DATA_FORMAT     0x31
    #define ADXL345_REG_DATA_FORMAT_SELF_TEST       (1<<7) // 1 = enable, 0 = disable 
    #define ADXL345_REG_DATA_FORMAT_SPI             (1<<6) // 1 = 3-wire, 0 = 4-wire
    #define ADXL345_REG_DATA_FORMAT_INT_INVERT      (1<<5) // 1 = active-low, 0 = active-high (interrupts)
    #define ADXL345_REG_DATA_FORMAT_D4_0            (1<<4) // always 0
    #define ADXL345_REG_DATA_FORMAT_FULL_RES        (1<<3) // 1 = full resolution, 0 = 10 bit mode
    #define ADXL345_REG_DATA_FORMAT_JUSTIFY         (1<<2) // 1 = left (MSB) mode, 0 = right justified w/ sign ext.
        #define ADXL345_REG_DATA_FORMAT_RANGE_MASK          0x3
            #define ADXL345_REG_DATA_FORMAT_RANGE_16G           0x3
            #define ADXL345_REG_DATA_FORMAT_RANGE_8G            0x2
            #define ADXL345_REG_DATA_FORMAT_RANGE_4G            0x1
            #define ADXL345_REG_DATA_FORMAT_RANGE_2G            0x0

// 0x32 - 0x37 - Read only, two's compliment
//       ?0 = least significant byte 
//       ?1 = most significant byte 
//          ADXL345_REG_DATA_FORMAT 0x31 controls data format
// Always do multi-byte read (all registers)
#define ADXL345_REG_DATAX0          0x32
#define ADXL345_REG_DATAX1          0x33
#define ADXL345_REG_DATAY0          0x34
#define ADXL345_REG_DATAY1          0x35
#define ADXL345_REG_DATAZ0          0x36
#define ADXL345_REG_DATAZ1          0x37  

#define ADXL345_REG_FIFO_CTL        0x38  
        #define ADXL345_REG_FIFO_CTL_FIFO_MASK          0xC
            #define ADXL345_REG_FIFO_CTL_FIFO_TRIGGER       0x3
            #define ADXL345_REG_FIFO_CTL_FIFO_STREAM        0x2
            #define ADXL345_REG_FIFO_CTL_FIFO_FIFO          0x1
            #define ADXL345_REG_FIFO_CTL_FIFO_BYPASS        0x0
    #define ADXL345_REG_FIFO_CTL_TRIGGER            (1<<5) // 1 = link trigger INT2, 0 = link trigger INT1
        #define ADXL345_REG_FIFO_CTL_SAMPLES_MASK       0x1F //count of FIFO entries in category
                                                             //  31 - 0

#define ADXL345_REG_FIFO_STATUS     0x39    // Read only
    #define ADXL345_REG_FIFO_STATUS_FIFO_TRIG       (1<<7) // 1 = trigger occ, 0 = no trigger occ
    #define ADXL345_REG_FIFO_STATUS_D6_0            (1<<6) // always 0
        #define ADXL345_REG_FIFO_STATUS_ENTRIES_MASK        0x3F // How many data values in FIFO

//Data rates from Table 6
#define ADXL345_DATA_RATE_3200_HZ   0xF
#define ADXL345_DATA_RATE_1600_HZ   0xE 
#define ADXL345_DATA_RATE_800_HZ    0xD
#define ADXL345_DATA_RATE_400_HZ    0xC
#define ADXL345_DATA_RATE_200_HZ    0xB
#define ADXL345_DATA_RATE_100_HZ    0xA
#define ADXL345_DATA_RATE_50_HZ     0x9
#define ADXL345_DATA_RATE_25_HZ     0x8
#define ADXL345_DATA_RATE_12_5_HZ   0x7
#define ADXL345_DATA_RATE_6_25_HZ   0x6

uint8_t adxl345_get_device_id(uint16_t * id);

#define ADXL345_POWER_PORT  gpioPortB
#define ADXL345_POWER_PIN   13
void adxl345_power_on();
void adxl345_setup();

#define ADXL345_INT1_PORT   gpioPortB //TP 18
#define ADXL345_INT1_PIN    12

#define ADXL345_INT2_PORT   gpioPortB //TP 19
#define ADXL345_INT2_PIN    11

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} xyz_data;

void adxl345_read_xyz(xyz_data *);
uint8_t adxl345_fifo_depth();
uint8_t adxl345_fifo_full();

extern uint8_t accel_int1;
extern uint8_t accel_int2;




#endif
