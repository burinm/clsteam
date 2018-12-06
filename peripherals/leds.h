#ifndef __LEDS_H__
#define __LEDS_H__

#define LED_port       gpioPortD
#define LED_default    false    //off

#define LED2_pin        12
#define LED2            LED2_pin

#define LED1_pin        11
#define LED1            LED1_pin

#define LED0_pin        10
#define LED0            LED0_pin

void init_leds();



#endif
