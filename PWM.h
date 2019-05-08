#ifndef PWM_H_
#define PWM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "inc/tm4c123gh6pm.h"

void pwm_init(volatile uint8_t* ui8Adjust, uint32_t* ui32Period);
void pwm_handler(volatile uint8_t* ui8Adjust, uint32_t* ui32Period);

#endif /* PWM_H_ */
