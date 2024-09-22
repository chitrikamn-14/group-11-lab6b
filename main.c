#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

volatile int i = 0;  // Used to measure the switch press time
int duty;            // PWM duty cycle variable

// Function Declarations
void GPIO_PORTF_setup(void);   // Sets up GPIO Port F pins and interrupts
void PWM_setup(void);          // Sets up the PWM module
void Portf_interrupt_handler(void);  // Interrupt handler for switch presses
