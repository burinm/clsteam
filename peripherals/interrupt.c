#include "interrupt.h"
#include "accelerometer.h"
#include "em_gpio.h"
#include "leds.h"

void GPIO_EVEN_IRQHandler(void) {
    accel_int1=1;
    led_on(LED1);
    GPIO_IntClear(0x5555);
}

void GPIO_odd_IRQHandler(void) {
    accel_int2=1;
    led_on(LED2);
    GPIO_IntClear(0xAAAA);

}
