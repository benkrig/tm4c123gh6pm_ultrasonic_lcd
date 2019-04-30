#include "LCD.h"

int main(void) {
	lcd_init();
	while(1){
		lcd_goto(1,1);
		lcd_puts("No distance...");
		lcd_goto(2,1);
		lcd_puts("Connect UART...");

		SysCtlDelay(SysCtlClockGet()/2);
		lcd_clear();
	}
}
