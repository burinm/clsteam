#include "leds.h"
#include <em_gpio.h>

void init_leds() {
    GPIO_PinModeSet(LED_port, LED2_pin, gpioModePushPull, LED_default);
    GPIO_PinModeSet(LED_port, LED1_pin, gpioModePushPull, LED_default);
    GPIO_PinModeSet(LED_port, LED0_pin, gpioModePushPull, LED_default);
}

//turn on led
void led_on(unsigned int pin){
    GPIO_PinOutSet(LED_port, pin);
}

//turn off led
void led_off(unsigned int pin){
    GPIO_PinOutClear(LED_port, pin);
}

//toggle led
void led_toggle(unsigned int pin) {
    GPIO_PinOutToggle(LED_port, pin);
}
