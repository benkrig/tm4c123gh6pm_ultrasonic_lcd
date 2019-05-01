#ifndef UART_H_
#define UART_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"

#define UART_PORT_BASE			UART4_BASE
#define GPIO_UART_PORT			GPIO_PORTC_BASE
#define GPIO_PORT_CLK				SYSCTL_PERIPH_GPIOC
#define UART_PORT_CLK				SYSCTL_PERIPH_UART4
#define UART_PORT_TX				GPIO_PC5_U4TX
#define UART_PORT_TX_PIN		GPIO_PIN_5
#define UART_PORT_RX				GPIO_PC4_U4RX
#define GPIO_RX_PIN		GPIO_PIN_4
#define UART_INT						INT_UART4
#define BAUDRATE						115200

void uart_init(void);
void uart_puts(char*);
void uart_receive(volatile char*);
void uart_putc(unsigned char,int type);

#endif /* UART_H_ */
