#include "UART.h"
#include "LCD.h"

// UART message format can have whatever form you want
// We chose the following:
// $ -- messgae begins
// * -- message ends
// An example message would look like:
// $212*

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

void uart_receive(void) {
	char start_c; // track our start char
	
	lcd_goto(1, 1);
	lcd_puts("Distance (cm):");
	lcd_goto(2, 1);

	// find message start
	while(UARTCharsAvail(UART_PORT_BASE)) {
		start_c = UARTCharGet(UART_PORT_BASE);
		if (start_c == '$') {
			// message is beginning
			break;
		}
	}
	
	// read message
	while(UARTCharsAvail(UART_PORT_BASE)) {
		char message_c = UARTCharGet(UART_PORT_BASE);
		if (message_c == '*') {			
			// message has ended
			// to allow time for lcd_clear, we disable UART Interrupts and clear
			UARTIntDisable(UART_PORT_BASE, UART_INT_RX);
			SysCtlDelay(SysCtlClockGet() / 6);
			lcd_clear();
			UARTIntEnable(UART_PORT_BASE, UART_INT_RX);
			
			// empty out received chars before the next interrupt occurs
			while(UARTCharsAvail(UART_PORT_BASE)) {UARTCharGet(UART_PORT_BASE);}
			break;
		};
		if (start_c == '$') {
			// message has began, write to lcd
			lcd_putc(message_c, DATA);
		}
	}
}
