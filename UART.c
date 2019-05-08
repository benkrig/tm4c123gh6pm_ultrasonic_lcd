#include "UART.h"
#include "LCD.h"

void uart_init(void) {
	// enable clocks for UART and GPIO
	SysCtlPeripheralEnable(UART_PORT_CLK);
  SysCtlPeripheralEnable(GPIO_PORT_CLK);

	// configure UART TX and RX ports
  GPIOPinConfigure(UART_PORT_RX);
	GPIOPinConfigure(UART_PORT_TX);

	

	// configure GPIO pin types to UART
  GPIOPinTypeUART(GPIO_UART_PORT, GPIO_RX_PIN);
	GPIOPinTypeUART(GPIO_UART_PORT, GPIO_TX_PIN);


	// Set BAUD rate, packet length, stop bit(s), and parity
	UARTConfigSetExpClk(UART_PORT_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntEnable(UART_PORT_BASE, UART_INT_RX);
}

void uart_receive(void) {
	int i = 0;
	char * test = "nothing";
	char c;
		lcd_goto(1,1);

	
	while(UARTCharsAvail(UART_PORT_BASE)) {
		c = UARTCharGet(UART_PORT_BASE);
		if (c == '$') {
			break;
		}
	}
	while(UARTCharsAvail(UART_PORT_BASE)) {
		char b = UARTCharGet(UART_PORT_BASE);
		if (b == '*') break;
		if (c == '$') {
			lcd_putc(b, DATA);
		}
	}
}

void uart_send(char* message, int len) {
	
	UARTCharPut(UART_PORT_BASE, '$');
	for (int i = 0; i < len; i ++) {
		UARTCharPut(UART_PORT_BASE, message[i]);
	}
	UARTCharPut(UART_PORT_BASE, '*');
}