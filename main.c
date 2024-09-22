#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

volatile int i = 0;  // Used to measure the switch press time
int duty;            // PWM duty cycle variable

// Function Declarations
void GPIO_PORTF_setup(void);   // Sets up GPIO Port F pins and interrupts
void PWM_setup(void);          // Sets up the PWM module
void Portf_interrupt_handler(void);  // Interrupt handler for switch presses

// Setup for GPIO Port F to handle buttons and LEDs
void GPIO_PORTF_setup(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 5);          // Enable clock for Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;         // Unlock Port F
    GPIO_PORTF_CR_R = 0x1F;                 // Enable changes to PF4-PF0
    GPIO_PORTF_PUR_R = 0x11;                // Enable pull-up for PF4 and PF0 (switches)
    GPIO_PORTF_DEN_R = 0x1F;                // Digital enable PF4-PF0
    GPIO_PORTF_DIR_R = 0x0E;                // Set PF1-PF3 as outputs (LEDs), PF0 and PF4 as inputs

    // Setup interrupts for PF4 and PF0 (switches)
    GPIO_PORTF_IM_R &= ~0x11;               // Disable interrupts for PF4 and PF0 during setup
    GPIO_PORTF_IS_R &= ~0x11;               // Set PF4 and PF0 as edge-sensitive
    GPIO_PORTF_IBE_R &= ~0x11;              // Disable both edges triggering for PF4 and PF0
    GPIO_PORTF_IEV_R &= ~0x11;              // Set PF4 and PF0 to trigger on falling edge

    GPIO_PORTF_ICR_R |= 0x11;               // Clear any prior interrupt on PF4 and PF0
    GPIO_PORTF_IM_R |= 0x11;                // Enable interrupts for PF4 and PF0

    NVIC_PRI7_R &= 0xFF3FFFFF;              // Set priority for Port F interrupt
    NVIC_EN0_R |= 1 << 30;                  // Enable interrupt 30 (Port F)
}


// Setup for PWM Module
void PWM_setup(void)
{
    SYSCTL_RCGCPWM_R |= (1 << 1);           // Enable clock for PWM module 1
    GPIO_PORTF_AFSEL_R |= (1 << 2);         // Enable alternate function for PF2
    GPIO_PORTF_PCTL_R |= 0x500;             // Set PF2 for PWM output

    PWM1_3_CTL_R = 0x00;                    // Disable PWM generator 3 during setup
    PWM1_3_GENA_R = 0x8C;                   // Configure output to be set when counting down and load is reached
    PWM1_3_LOAD_R = 160;                    // Set period for PWM (adjust for desired frequency)
    PWM1_3_CMPA_R = (duty / 100) * 160 - 1; // Set initial duty cycle
    PWM1_3_CTL_R |= 0x01;                   // Enable PWM generator 3
    PWM1_ENABLE_R |= 0x040;                 // Enable PWM output on PF2
}

// Main function
void main(void)
{
    GPIO_PORTF_setup();                     // Initialize GPIO Port F
    PWM_setup();                            // Initialize PWM
    duty = 50;                              // Set initial duty cycle to 50%
    PWM1_3_CMPA_R = (160 * duty) / 100;     // Set the duty cycle

    while (1)
    {
        // Main loop, nothing to do as interrupts handle the logic
    }
}

// Interrupt handler for Port F, triggered by switch press
void Portf_interrupt_handler(void)
{
    i = 0;

    // Measure how long the switch is pressed by counting in a loop
    while ((GPIO_PORTF_DATA_R & 0x01) == 0) {
        i++;
    }

    // Handle switch 1 (PF4)
    if (GPIO_PORTF_MIS_R & 0x10)
    {
        // Implement behavior for switch 1 if needed
    }
    // Handle switch 2 (PF0)
    else if (GPIO_PORTF_MIS_R & 0x01)
    {
        // Long press on switch 2, decrease duty cycle by 5%
        if (i >= 900000)
        {
            if (duty > 5)
            {
                duty -= 5;
            }
            else
            {
                duty = 5;
            }
        }
    // Short press on switch 2, increase duty cycle by 5%
        else if (i >= 0 && i <= 900000)
        {
            if (duty < 95)
            {
                duty += 5;
            }
            else
            {
                duty = 100;
            }
        }
    }

    PWM1_3_CMPA_R = (160 * duty) / 100;     // Update PWM duty cycle

    // Simple delay for debounce
    int j;
    for (j = 0; j < 1600 * 100 / 4; j++) {}

    GPIO_PORTF_ICR_R = 0x11;                // Clear interrupt flags for PF4 and PF0
}
