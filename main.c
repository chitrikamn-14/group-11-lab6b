// lab 5

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void systick_timer(void);


void GPIO_PORT_F_init(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;            // ENABLE CLOCK TO GPIOF
    GPIO_PORTF_LOCK_R = 0x4C4F434B;          // UNLOCK COMMIT REGISTER
    GPIO_PORTF_CR_R   = 0x1F;                // MAKE PORTF0 CONFIGURABLE
    GPIO_PORTF_DEN_R  = 0x1F;                // SET PORTF DIGITAL ENABLE
    GPIO_PORTF_DIR_R  = 0x0E;                // SET PF0, PF4 as input and PF1, PF2 and PF3 as output
    GPIO_PORTF_PUR_R  = 0x11;                // PORTF PF0 and PF4 IS PULLED UP

    NVIC_EN0_R |= 1 << 30;
    GPIO_PORTF_IS_R  = 0x00;
    GPIO_PORTF_IBE_R = 0x00;
    GPIO_PORTF_IEV_R = 0x00;
    GPIO_PORTF_IM_R  |= 0x11;
}
void systickInit(void)
{
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 0x05;
    NVIC_ST_RELOAD_R = 4000000;
}

void systick_timer()                           // DEFINING systick_timer FUNCTION
{
    systickInit();
           NVIC_ST_RELOAD_R = 4000000;
           while((NVIC_ST_CTRL_R & 0x10000)==0)
                    {

                    }
           NVIC_ST_CTRL_R = 0x0;
}

void Portf_interrupt_handler(void)
{
    systick_timer();
    if (GPIO_PORTF_RIS_R & 0x10)
    {
    GPIO_PORTF_DATA_R ^= 0x02;
    GPIO_PORTF_ICR_R = 0x10;
    }

    if (GPIO_PORTF_RIS_R & 0x01)
    {
    GPIO_PORTF_DATA_R ^= 0x02;
    GPIO_PORTF_ICR_R = 0x01;
    }
}
int main(void)                               // MAIN FUNCTION
{
    GPIO_PORT_F_init();                      // GPIO INITIALISATION FUNCTION
    systick_timer();                       // SYSTICK SETUP
    while (1)
    {                                        // DO NOTHING
    }
}
