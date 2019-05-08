#include "LCD.h"

void lcd_init() {
	SysCtlPeripheralEnable(LCD_PORT_ENABLE);	// Enable GPIOB clock

	GPIOPinTypeGPIOOutput(LCD_PORT, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	
	//GPIO_PORTB_DIR_R |= 0xFF;	// GPIOB as output
	//GPIO_PORTB_DEN_R |= 0xFF;	// GPIOB Digital Enable

	lcd_clear();	// clear any noise from startup

	// see 1602 LCD data sheet for instruction details

	// 4 bit mode, 2 lines
	lcd_putc(0x28, CMD);	// function set - 0b0010 1000 - DL=0, 4-bit mode. N=1, 2 lines.

	// incrementing, no shift
	lcd_putc(0x06, CMD);	// entry mode - 0b0000 0110 - I/D=1, incrementing. S=0, shift is not performed.

	// DDRAM starts at 0
	lcd_putc(0x80, CMD);	// set DDRAM address for write - 0b1000 000 - set DDRAM address to 0's.

	// turn display on, turn cursor on, turn blink on
	lcd_putc(0x0C, CMD); 	// display on/off - 0b0000 1100 - D=1, display on. C=0, cursor off. B=0, blink off.

	lcd_clear();	// clear any noise after setup
}

void lcd_goto(char x, char y) {
	if(x==1)
		lcd_putc(0x80+((y-1) % 16), CMD);
	else
		lcd_putc(0xC0+((y-1) % 16), CMD);
}

// clear the display
void lcd_clear(void) {
	lcd_putc(0x01, CMD);
	delayMs(2);
}

void lcd_puts( char* s) {
	while(*s){
		lcd_putc(*s++, DATA);
	}
}

// type = 0, instruction write = CMD
// type = 1, data write = DATA
// 4-bit mode means we need to perform 2 writes each time.
// 	first write is most signifigant bits, x in 0bxxxx ____
// 	second write is least signifigant bits, x in 0b____ xxxx
void lcd_putc(unsigned char c, int type) {
	// WRITE TO MOST SIGNIFICANT BITS
	// select register
	if(type == DATA)GPIOPinWrite(LCD_PORT, RS, 0x01);	// DATA register is 1	
	else GPIOPinWrite(LCD_PORT, RS, 0x00);			// CMD register is 0

	// write data
	GPIOPinWrite(LCD_PORT, E, 0x02); 	// enable	
	GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0xF0) );	// write to most significant bits

	delayUs(3);	// hold for data write

	// disable
	GPIOPinWrite(LCD_PORT, E, 0x00);  	// disable
	delayUs(1);				// enable fall time
	// END SIGNIFICANT BITS

	// WRITE TO LEAST SIGNIFICANT BITS
	// register is already selected
	// write data
	GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0x0F) << 4 );	// write to least significant bits
	GPIOPinWrite(LCD_PORT, E, 0x02);	// enable

	delayUs(3);	// hold for data write

	// disable
	GPIOPinWrite(LCD_PORT, E, 0x00);  	// disable
	delayUs(1);				// enable fall time
	// END LEAST SIGNIFICANT BITS

	// delay times from mcu, propagation times all require this extended delay
	// delay times inside lcd and propagation within the lcd additionally require this delay
	// if we were dealing directly with the lcd's cu we could match timings directly from datasheet
	// but because of the limited abilities of our tm4c123 we are not able to generate exact timings for nanoseconds
	delayUs(20);	// hold time required for write to RAM
}

// *********************************************
// UTILITY
// *********************************************

// delay milliseconds
void delayMs(uint32_t ui32Ms) {

	// 1 clock cycle = 1 / SysCtlClockGet() second
	// 1 SysCtlDelay = 3 clock cycle = 3 / SysCtlClockGet() second
	// 1 second = SysCtlClockGet() / 3
	// 0.001 second = 1 ms = SysCtlClockGet() / 3 / 1000

	SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

// delay microseconds
void delayUs(uint32_t ui32Us) {
	SysCtlDelay(ui32Us * (SysCtlClockGet() / 3 / 1000000));
}


// delay nanoseconds
// clock speed is 16MHz, meaning the lowest we can go is 62.5ns
void delayNs(uint32_t ui32Ns) {
	SysCtlDelay(ui32Ns * (SysCtlClockGet() / 3 / 1000000000));
}
