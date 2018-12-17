#include "interrupt.h"
#include "accelerometer.h"
#include "em_gpio.h"
#include "em_core.h"
#include "leds.h"

void GPIO_EVEN_IRQHandler(void) {
CORE_ATOMIC_IRQ_DISABLE();
	gecko_external_signal(ACCEL_INT1);
    accel_int1=1;
    led_on(LED1);
    GPIO->IFC = (GPIO->IF & 0x5555);
CORE_ATOMIC_IRQ_ENABLE();
}

void GPIO_ODD_IRQHandler(void) {
CORE_ATOMIC_IRQ_DISABLE();
	gecko_external_signal(ACCEL_INT2);
    accel_int2=1;
    led_on(LED2);
    GPIO->IFC = (GPIO->IF & 0xAAAA);
CORE_ATOMIC_IRQ_ENABLE();
}
