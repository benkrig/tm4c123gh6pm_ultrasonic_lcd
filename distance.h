/*
 * distance.h
 *
 *  Created on: Sep 16, 2017
 *      Author: Solomon Tan
 */
#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"


void measureDistance(void);

uint8_t calDistance(void);

void SR_Trigger(void);

void SR04_Init(void);

#endif
