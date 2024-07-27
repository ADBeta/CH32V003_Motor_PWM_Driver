/******************************************************************************
* Medium Current PWM Motor Controller, using the CH32V003 MCU
* Has Over-Current protection, adjustable PWM setting
* TODO: RPM hold mode
*
* Notes:
* 	TODO: Set the clock to meet this
*	PWM runs at ~40KHz, with 8bits of accuracy
*
* Pinout:
* 	TODO: UART?
* 	TODO: Op Amp
* 	TODO: RPM Sensor
*
* 	D3 (A4)		- Analog Potentiometer Input
*   TODO: D4 Needs to be the OpAmp Input
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

	gpio_set_mode(GPIO_PA1, INPUT_FLOATING);
	gpio_set_mode(GPIO_PA2, INPUT_FLOATING);
	gpio_set_mode(GPIO_PD4, INPUT_FLOATING);

	EXTEN->EXTEN_CTR |=  EXTEN_OPA_EN;


	while(1)
	{

	}

	/*
	// Set up PD3 for Analog Reading
	gpio_init_adc(ADC_CLOCK_DIV_2, ADC_SAMPLE_CYCLES_73);
	gpio_set_mode(GPIO_A4, INPUT_ANALOG);



	pwm_init();
	pwm_set_duty(128);


	while(1) {
		// Divide the input to be within the range of the PWM output
		uint16_t pwm_val = gpio_analog_read(GPIO_ADC_A4) / 4;

		printf("%d\n", pwm_val);
		Delay_Ms(100);
	}
	*/
}



/*** Functions ***************************************************************/
// TODO: Change PWM output pin
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
