#include "LCD.h"
#include "UART.h"
#include "PWM.h"

volatile char* distance = "asdasdasd";

uint32_t ui32Period;
volatile uint8_t ui8Adjust = 75;

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
	//test_fun();
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
	
	//led_init();
	pwm_init(&ui8Adjust, &ui32Period);;
	uart_init();
	//lcd_init();
	
	while(1){
		//lcd_goto(1,1);
		//lcd_puts(distance);
		//lcd_goto(2,1);
		//lcd_puts("Connect UART...");
		//SysCtlDelay(SysCtlClockGet() / 2);
		//lcd_clear();
	}
}
