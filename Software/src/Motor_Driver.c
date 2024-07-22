/******************************************************************************
* Medium Current PWM Motor Controller, using the CH32V003 MCU
*
* Notes:
*	PWM runs at 190KHz, with 8bits of accuracy
*
* Pinout:
* 	TODO: UART?
*
* 	D3 (A4)		- Analog Potentiometer Input
*	D4			- PWM Motor Output
*
* ADBeta (c)	20 Jul 2024
******************************************************************************/
#include "ch32v003fun.h"
#include "lib_GPIOCTRL.h"

#include <stdio.h>

/*** Forward Declarations ****************************************************/
/// @breif initialises the ADC for the Potentiometer, and the Current Sensor
/// @param none
/// @return none
void adc_init(void);

/// @breif Initialised TIM2 Channel 1 (D4) To be a PWM Output, active HIGH.
/// Autoreload is set to 254, Capture mode is 0b111, PWM2.
/// @param none
/// @return none
void pwm_init(void);

/// @breif Sets the Duty Cycle of PWM output. Max input is 255
/// @param duty, input duty cycle value
/// @return none
void pwm_set_duty(uint32_t duty);

/*** Main ********************************************************************/
int main()
{
	SystemInit();
	pwm_init();
	pwm_set_duty(128);

	adc_init();

	while(1) {
		// Start a software conversion, wait for it to finish
		ADC1->CTLR2 |= ADC_SWSTART;
		while(!(ADC1->STATR & ADC_EOC));
	
		// Get the resulting data from the ADC
		uint16_t data = ADC1->RDATAR;

		printf("val: %d\n", data);
	}
}



/*** Functions ***************************************************************/
void adc_init(void)
{
	// Enable the ADC clock
	RCC->APB2PCENR |= RCC_APB2Periph_ADC1;

	// Set ACD Clock. Set bits 15:11 to 0 - HBCLK/2 = 24MHz
	RCC->CFGR0 &= 0xFFFF07FF;
		
	// Set Pin Mode to Analog Input
	gpio_set_mode(GPIO_PD3, INPUT_ANALOG);	

	// Reset the ADC, Inits all registers
	RCC->APB2PRSTR |=  RCC_APB2Periph_ADC1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;
	
	// Set rule channel conversion for single conversion on channel 4 (Pin A4)
	ADC1->RSQR1 = 0;
	ADC1->RSQR2 = 0;
	ADC1->RSQR3 = 4;
	
	// Reset, then set the sample time for channel 4 (Pin A4) to 241 cycles
	ADC1->SAMPTR2 &= ~(ADC_SMP0 << (3*4));
	ADC1->SAMPTR2 |= 0x110 << (3*4);
	
	// Enable the ADC, and set the triggering to external
	ADC1->CTLR2 |= ADC_ADON | ADC_EXTSEL;
	
	// Reset calibration, wait for it to finish
	ADC1->CTLR2 |= ADC_RSTCAL;
	while(ADC1->CTLR2 & ADC_RSTCAL);
}


void pwm_init(void)
{
	// Enable TIM2 Clock	
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	// Set GPIO-D4 OUTPUT 10MHz, Aleternate Function (Multiplex)
	gpio_set_mode(GPIO_PD4, OUTPUT_10MHZ_PP | OUTPUT_PP_AF);

	// Reset TIM2, Inits all registers
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;

	// Set Prescaler
	TIM2->PSC = 0x0000;
	// Set PWM Max Value (Autoreload Value)
	TIM2->ATRLR = 254;

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


void pwm_set_duty(uint32_t duty)
{
	TIM2->CH1CVR = duty;
}
