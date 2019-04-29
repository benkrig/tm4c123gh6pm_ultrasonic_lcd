#include "LCD.h"

void lcd_init() {
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOB;	// Enable GPIOB clock

	GPIO_PORTB_DIR_R |= 0xFF; // GPIOB as output
	GPIO_PORTB_DEN_R |= 0xFF;	// GPIOB Digital Enable

	lcd_clear();	// clear any noise from startup

	// see 1602 LCD data sheet for instruction details

	// 4 bit mode, 2 lines
	lcd_putc(0x28, CMD);	// function set - 0b0010 1000 - DL=0, 4-bit mode. N=1, 2 lines.

	// incrementing, no shift
	lcd_putc(0x06, CMD);	// entry mode - 0b0000 0110 - I/D=1, incrementing. S=0, shift is not performed.

	// DDRAM starts at 0
	lcd_putc(0x80, CMD);	// set DDRAM address for write - 0b1000 000 - set DDRAM address to 0's.

	// turn display on, turn cursor on, turn blink on
	lcd_putc(0x0f, CMD); 	// display on/off - 0b0000 1111 - D=1, display on. C=1, cursor on. B=1, blink on.

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
	SysCtlDelay(10);
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
//  second write is least signifigant bits, x in 0b____ xxxx
void lcd_putc(unsigned char c, int type) {
	// WRITE TO MOST SIGNIFICANT BITS

	// data setup
	// we do this first to avoid any need for holding for data set up time
	GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0xF0) ); // write to most significant bits

	// register select
	if(type == DATA)GPIOPinWrite(LCD_PORT, RS, 0x01);	// DATA register is 1
	else GPIOPinWrite(LCD_PORT, RS, 0x00);						// CMD register is 0

	GPIOPinWrite(LCD_PORT, E, 0x02); 	// enable
	delayNs(25 + 1); 									// enable rise time

	// data write occurs here
	// data hold
	delayNs(50 + 1);

	GPIOPinWrite(LCD_PORT, E, 0x00);  // disable
	delayNs(25 + 1); 								  // enable fall time

	// END SIGNIFICANT BITS


	// WRITE TO LEAST SIGNIFICANT BITS

	// data setup
	// we do this first to avoid any need for holding for data set up time
	GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0x0F) << 4 );

	// register select
	if(type == DATA)GPIOPinWrite(LCD_PORT, RS, 0x01);	// DATA register is 1
	else GPIOPinWrite(LCD_PORT, RS, 0x00);						// CMD register is 0

	GPIOPinWrite(LCD_PORT, E, 0x02); 	// enable
	delayNs(25 + 1); 									// enable rise time

	// data write occurs here
	// data hold
	delayNs(50 + 1);

	GPIOPinWrite(LCD_PORT, E, 0x00);  // disable
	delayNs(25 + 1); 								  // enable fall time
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
void delayNs(uint32_t ui32Ns) {
	SysCtlDelay(ui32Ns * (SysCtlClockGet() / 3 / 1000000000));
}
