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
