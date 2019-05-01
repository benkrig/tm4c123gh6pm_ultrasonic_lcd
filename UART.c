#include "UART.h"

void uart_init(void) {
	// enable clocks for UART and GPIO
	SysCtlPeripheralEnable(UART_PORT_CLK);
  SysCtlPeripheralEnable(GPIO_PORT_CLK);

	// configure UART TX and RX ports
  GPIOPinConfigure(UART_PORT_RX);

	// configure GPIO pin types to UART
  GPIOPinTypeUART(GPIO_UART_PORT, GPIO_RX_PIN);

	// Set BAUD rate, packet length, stop bit(s), and parity
	UARTConfigSetExpClk(UART_PORT_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntEnable(UART_PORT_BASE, UART_INT_RX);
}

void uart_receive(volatile char* dest) {
	
}
