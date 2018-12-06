/* usart.c

    Author(s):
        Silicon Labs(c) 2018
        burin 2018/10/7

    Parts copied verbatim from:
        https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg1/html/group__USART.html

*/

#include <em_cmu.h>
#include <em_usart.h>
#include <em_gpio.h>
#include "usart.h"

void UART0_init() {
    /* USART is a HFPERCLK peripheral. Enable HFPERCLK domain and USART0.
     * We also need to enable the clock for GPIO to configure pins. */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Initialize with default settings and then update fields according to application requirements. */
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = UART0_BAUD;
    USART_InitAsync(USART0, &initAsync);

    /* Enable I/O and set location */
    USART0->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    USART0->ROUTELOC0 = (USART0->ROUTELOC0
            & ~(_USART_ROUTELOC0_TXLOC_MASK
                | _USART_ROUTELOC0_RXLOC_MASK))
        | (UART0_US0_TX_LOCATION << _USART_ROUTELOC0_TXLOC_SHIFT)
        | (UART0_US0_RX_LOCATION << _USART_ROUTELOC0_RXLOC_SHIFT);

    /* To avoid false start, configure TX pin as initial high */
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_TX_PORT(UART0_US0_TX_LOCATION), AF_USART0_TX_PIN(UART0_US0_TX_LOCATION), gpioModePushPull, 1);
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART1_RX_PORT(UART0_US0_RX_LOCATION), AF_USART0_RX_PIN(UART0_US0_RX_LOCATION), gpioModeInput, 0);
    /* Don't enable CTS/RTS hardware flow control pins in this example. Automatic hardware flow control is available and can be
     * enabled by setting USART_ROUTEPEN_RTSPEN and USART_ROUTEPEN_CTSPEN, and enable GPIO for the RTS/CTS pins. */
}

/* Not working as SPI, this is a asynchronous setup at the moment */
void UART1_init() {
#if 0
    /* USART is a HFPERCLK peripheral. Enable HFPERCLK domain and USART1.
     * We also need to enable the clock for GPIO to configure pins. */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART1, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Initialize with default settings and then update fields according to application requirements. */
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = UART1_BAUD;
    USART_InitAsync(USART1, &initAsync);

    /* Enable I/O and set location */
    USART1->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    USART1->ROUTELOC0 = (USART1->ROUTELOC0
            & ~(_USART_ROUTELOC0_TXLOC_MASK
                | _USART_ROUTELOC0_RXLOC_MASK))
        | (UART1_US1_TX_LOCATION << _USART_ROUTELOC0_TXLOC_SHIFT)
        | (UART1_US1_RX_LOCATION << _USART_ROUTELOC0_RXLOC_SHIFT);

    /* Also enable CS and CLK pins if the USART is configured for synchronous mode.
     * Set GPIO mode. */
    /*
       if (USART1->CTRL & USART_CTRL_SYNC) {
       USART1->ROUTEPEN |= USART_ROUTEPEN_CSPEN | USART_ROUTEPEN_CLKPEN;
       USART1->ROUTELOC0 = (USART1->ROUTELOC0
       & ~(_USART_ROUTELOC0_CSLOC_MASK
       | _USART_ROUTELOC0_CLKLOC_MASK))
       | (USER_CS_LOCATION << _USART_ROUTELOC0_TXLOC_SHIFT)
       | (USER_CLK_LOCATION << _USART_ROUTELOC0_RXLOC_SHIFT);
       GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_TX_PORT(USER_TX_LOCATION), AF_USART0_TX_PIN(USER_TX_LOCATION), gpioModePushPull, 0);
       GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_RX_PORT(USER_RX_LOCATION), AF_USART0_RX_PIN(USER_RX_LOCATION), gpioModeInput, 0);
       GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_CS_PORT(USER_CS_LOCATION), AF_USART0_CS_PIN(USER_CS_LOCATION), gpioModePushPull, 0);
       GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_CLK_PORT(USER_CLK_LOCATION), AF_USART0_CLK_PIN(USER_CLK_LOCATION), gpioModePushPull, 0);
       } else {
     */

    /* To avoid false start, configure TX pin as initial high */
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART1_TX_PORT(UART1_US1_TX_LOCATION), AF_USART1_TX_PIN(UART1_US1_TX_LOCATION), gpioModePushPull, 1);
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART1_RX_PORT(UART1_US1_RX_LOCATION), AF_USART1_RX_PIN(UART1_US1_RX_LOCATION), gpioModeInput, 0);
    /* Don't enable CTS/RTS hardware flow control pins in this example. Automatic hardware flow control is available and can be
     * enabled by setting USART_ROUTEPEN_RTSPEN and USART_ROUTEPEN_CTSPEN, and enable GPIO for the RTS/CTS pins. */
    //}

#endif
}
