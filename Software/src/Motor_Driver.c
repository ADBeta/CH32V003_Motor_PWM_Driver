/******************************************************************************
* Medium Current PWM Motor Controller, using the CH32V003 MCU
*
* Pinout:
*	 D4 - PWM Motor Output
*
* ADBeta (c)	20 Jul 2024
******************************************************************************/
#include "ch32v003fun.h"

//#include <stdio.h>

/*** Forward Declarations ****************************************************/
/// @breif Initialised TIM2 Channel 1 (D4) To be a PWM Output, active HIGH.
/// Autoreload is set to 1023, Capture mode is 0b111, PWM2.
/// @param none
/// @return none
void pwm_init(void);

/// @breif Sets the Duty Cycle of PWM output. Max input is 1024
/// @param val, input duty cycle value
/// @return none
void pwm_set_duty(uint32_t val);

/*** Main ********************************************************************/
int main()
{
	SystemInit();
	Delay_Ms( 100 );

	pwm_init();	
	while(1) {
		for(uint32_t x = 0; x < 1024; x++) {
			pwm_set_duty(x);
			Delay_Ms(10);

		}
	}
}



/*** Functions ***************************************************************/
// TODO: Set mode 1, invert val

void pwm_init(void)
{
	// Enable TIM2 Clock	
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	// Enable GPIOD Clock
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
	// Reset PORTD-4, then set to 10-MHz, Push-Pull, Alternate Function
	GPIOD->CFGLR &= ~(0xf<<(4*4));
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*4);

	// Reset TIM2, Inits all registers
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;

	// Set Prescaler
	TIM2->PSC = 0x0000;
	// Set PWM Max Value (Autoreload Value)
	TIM2->ATRLR = 1023;

	// Set the Compare Capture Register
	// TIM2_OC1M = 0b111 - PWM Mode 2 - Enable Preload
	TIM2->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1 | TIM_OC1M_0 | TIM_OC1PE;

	// Enable auto-reload
	TIM2->CTLR1 |= TIM_ARPE;

	// Enable channel output, set polarity ACTIVE_LOW
	TIM2->CCER |= TIM_CC1E | TIM_CC1P;

	// Initialise Counter
	TIM2->SWEVGR |= TIM_UG;

	// Enable TIM2
	TIM2->CTLR1 |= TIM_CEN;
}


void pwm_set_duty(uint32_t val)
{
	TIM2->CH1CVR = val;
}
