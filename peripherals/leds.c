#include "leds.h"
#include <em_gpio.h>

void init_leds() {
    GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
    GPIO_PinModeSet(LED_port, LED2_pin, gpioModePushPull, LED2_default);
    GPIO_PinOutSet(LED_port, LED2_pin);

    GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
    GPIO_PinModeSet(LED_port, LED1_pin, gpioModePushPull, LED1_default);
    GPIO_PinOutSet(LED_port, LED1_pin);

    GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
    GPIO_PinModeSet(LED_port, LED0_pin, gpioModePushPull, LED0_default);
    GPIO_PinOutSet(LED_port, LED0_pin);
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
