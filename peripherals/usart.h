#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>

//GPS interface
#define UART0_BAUD  9600
//#define UART0_BAUD  38400
//#define UART0_BAUD  115200
#define UART0_US0_TX_LOCATION   0   //PA0 - Exp header #12
#define UART0_US0_RX_LOCATION   0   //PA1 - Exp header #14

void UART0_init();

//FSK interface
#define UART1_BAUD  9600
#define UART1_US1_TX_LOCATION    11 //PC6 - EXP header #4
#define UART1_US1_RX_LOCATION    11 //PC7 - EXP header #6
#define UART1_US1_CS_LOCATION    11 //PC9 - EXP header #10
#define UART1_US1_CLK_LOCATION    11 //PC8 - EXP header #8

void UART1_init();

#endif
