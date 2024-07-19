#include "ch32v003fun.h"
#include <stdio.h>


int main()
{
	SystemInit();

	// Enable TIM2 Clock
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	// Reset PORTD-4 Flags
	GPIOD->CFGLR &= ~(0xf<<(4*4));
	// Set PORTD-4 to 10MHz Puls-Pull, Alternate Function
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*4);

	// Reset TIM2, Inits all registers
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;

	// NOTE: 190KHz 80%
	// Set Prescaler (INPUT / 0)
	TIM2->PSC = 0x0000;
	// Set PWM total cycle width
	TIM2->ATRLR = 255;

	// Set the Compare Capture Register
	// TIM2_OC1M = 0b110 - PWM Mode 1
	// TODO: Why?   Preload Enabled
	TIM2->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1 | TIM_OC1PE;

	// Enable auto-reload
	TIM2->CTLR1 |= TIM_ARPE;

	// Enable channel output, set polarity ACTIVE_LOW
	// TODO: TIM2_DEFAULT, why???
	#define TIM2_DEFAULT 0xFF
	TIM2->CCER |= TIM_CC1E | (TIM_CC1P & TIM2_DEFAULT);
	TIM2->CCER |= TIM_CC2E | (TIM_CC2P & TIM2_DEFAULT);

	// Initialise Counter
	TIM2->SWEVGR |= TIM_UG;

	// Enable TIM2
	TIM2->CTLR1 |= TIM_CEN;

	

	TIM2->CH1CVR = 64;

	while(1) {

	}
}
