#include "LCD.h"
#include "UART.h"
#include "PWM.h"

uint32_t ui32Period;
volatile uint8_t ui8Adjust = 75;



void UART4_Handler(void) {
	// clear flag
	UARTIntClear(UART4_BASE, UART_INT_RX);
	// call receiver
	uart_receive();
}

void PortFIntHandler(void){
	 // The ISR for GPIO PortF Interrupt Handling
    GPIOIntClear(GPIO_PORTF_BASE , GPIO_INT_PIN_4 | GPIO_INT_PIN_0);
		pwm_handler(&ui8Adjust, &ui32Period);
}


int main(void) {
	IntMasterEnable();
	IntEnable(INT_UART4);
	IntEnable(INT_GPIOF);
	
	uart_init();
	lcd_init();
	
	while(1){
	}
}
