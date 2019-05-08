#include "LCD.h"
#include "UART.h"

void UART4_Handler(void) {
	// clear flag
	UARTIntClear(UART4_BASE, UART_INT_RX);
	// call receiver
	uart_receive();
}

int main(void) {
	// enable all interrupts
	IntMasterEnable();
	// enable UART4 interrupts
	IntEnable(INT_UART4);
	
	uart_init();
	lcd_init();
	
	while(1){}
}
