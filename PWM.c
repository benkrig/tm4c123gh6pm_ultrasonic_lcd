#include "PWM.h"

void pwm_init(volatile uint8_t* ui8Adjust, uint32_t* ui32Period) {
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
		// Enable the clock for peripherals PortF and PortD and PWM1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configure the PD0 for PWM signal (PWM module 1 generator 0)
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);

    // Remove the Lock present on Switch SW2 (connected to PF0) and commit the change
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // Set the PF0, PF4 as Input and configure them to be Pulled-up
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure and enable the Interrupt for PF0 and PF4
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);

    // Calculate the Timer period of the PWM Module.
    uint32_t ui32PWMClock = SysCtlClockGet() / 64;
    *ui32Period = (ui32PWMClock / 50) - 1;
    // Configure thE PWM1 Genrator0 to work in Count Down Mode
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    // Load the calculated time period to the Generator0 of the PWM1 Module
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, *ui32Period);

    // Set the PWM duty cycle to a specified value
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, (*ui8Adjust) * (*ui32Period) / 1000);
    // Enable the PWM0 pin of the PWM Module 1 as output
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    // Enable the PWM Generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

void pwm_handler(volatile uint8_t* ui8Adjust, uint32_t* ui32Period) {
		// If SW1(PF4) is pressed then reduce the duty cycle
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0)== GPIO_PIN_0){
        *ui8Adjust = *ui8Adjust - 5;
        if (*ui8Adjust < 20){
            *ui8Adjust = 20;
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, (*ui8Adjust) * (*ui32Period) / 1000);
    }
    // If SW2(PW0) is pressed then increase the duty cycle
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0)== GPIO_PIN_4){
        *ui8Adjust = *ui8Adjust + 5;
        if (*ui8Adjust > 130){
            *ui8Adjust = 130;
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, (*ui8Adjust) * (*ui32Period) / 1000);
    }
}
