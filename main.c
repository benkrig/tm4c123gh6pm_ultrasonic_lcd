#include "LCD.h"
#include "UART.h"

volatile char* distance;

void led_init(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
}

void test_fun(void) {
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
	SysCtlDelay(SysCtlClockGet() / 2);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	SysCtlDelay(SysCtlClockGet() / 2);
	
	while(UARTCharsAvail(UART4_BASE)) {
		UARTCharGet(UART4_BASE);
	}
}

void UART4_Handler(void) {
	// clear flag
	UARTIntClear(UART4_BASE, UART_INT_RX);
	test_fun();
	// call receiver
	//uart_receive(distance);
}


int main(void) {
	IntMasterEnable();
	IntEnable(INT_UART4);
	
	led_init();
	uart_init();
	lcd_init();
	
	while(1){
		lcd_goto(1,1);
		lcd_puts("No distance...");
		lcd_goto(2,1);
		lcd_puts("Connect UART...");
		SysCtlDelay(SysCtlClockGet() / 2);
		lcd_clear();
	}
}
