#ifndef LCD_H_
#define LCD_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

#define LCD_PORT                         GPIO_PORTB_BASE
#define LCD_PORT_ENABLE                  SYSCTL_PERIPH_GPIOB
#define RS                               GPIO_PIN_0
#define E                                GPIO_PIN_1
#define D4                               GPIO_PIN_4
#define D5                               GPIO_PIN_5
#define D6                               GPIO_PIN_6
#define D7                               GPIO_PIN_7
#define CMD															 0
#define DATA														 1

void lcd_clear(void);
void lcd_puts(char*);
void lcd_goto(char,char);
void lcd_init(void);
void lcd_putc(unsigned char,int type);

void delayMs(uint32_t ui32Ms);
void delayUs(uint32_t ui32Us);
void delayNs(uint32_t ui32Ns);

#endif /* LCD_H_ */
