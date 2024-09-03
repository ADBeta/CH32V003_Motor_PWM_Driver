/******************************************************************************
* High(ish) Current PWM Motor Controller, using the CH32V003 MCU
* Has Over-Current protection, adjustable PWM setting
* TODO: RPM hold mode
*
* Notes:
*	PWM runs at ~46KHz, with 8bits of accuracy
*	Op-Amp is set to a gain of 22, 5A should be ~ TODO:
*
* Pinout:
* 	TODO: UART?
* 	TODO: RPM Sensor
* 	
*	A1          - Op-Amp Negative
*	A2          - Op-Amp Positive
* 	C0          - PWM Motor Output  (TIM2 Channel 3)
* 	D3 (A4)		- Analog Potentiometer Input
* 	D4 (A7)     - Analog Op-Amp Input Value
*
* ADBeta (c)	31 Jul 2024
******************************************************************************/
#include "ch32v003fun.h"
#include "lib_gpioctrl.h"

#include <stdio.h>

/*** Pin Definitions *********************************************************/
#define OPAMP_NEG GPIO_PA1
#define OPAMP_POS GPIO_PA2
#define OPAMP_OUT GPIO_A7

#define MODE_POT  GPIO_A4

#define PWM_OUT   GPIO_PC0


/*** Forward Declarations ****************************************************/
/// @breif Initialised TIM2 Channel 3 (C0) To be a PWM Output, active HIGH.
/// Autoreload is set to 254, Capture mode is 0b111, PWM2.
/// @param none
/// @return none
void pwm_init(void);

/// @breif Sets the Duty Cycle of PWM output (C0). Max input is 255
/// @param duty, input duty cycle value
/// @return none
void pwm_set_duty(uint32_t duty);

/*** Main ********************************************************************/
int main()
{
	SystemInit();

	// Set the Op-Amp Input Positive and Negative to Floating
	gpio_set_mode(OPAMP_NEG, INPUT_FLOATING);
	gpio_set_mode(OPAMP_POS, INPUT_FLOATING);

	// Set the Default Op-Amp pins to OPP0 and OPN0, then Enable the Op-Amp
	EXTEN->EXTEN_CTR &= ~(EXTEN_OPA_NSEL | EXTEN_OPA_PSEL); 
	EXTEN->EXTEN_CTR |=  EXTEN_OPA_EN;

	// Initiliase the ADC to use 24MHz clock, and Sample for 73 Clock Cycles
	gpio_init_adc(ADC_CLOCK_DIV_2, ADC_SAMPLE_CYCLES_73);
	// Set PD3/A4 and PD4/A7 to Analog Inputs
	gpio_set_mode(MODE_POT,  INPUT_ANALOG);
	gpio_set_mode(OPAMP_OUT, INPUT_ANALOG);

	// Initialise the PWM to use TIM2 Channel 3 (PC0), at ~47KHz
	pwm_init();

	/*** Main Control Loop ***************************************************/
	while(1) {



		// Divide the input to be within the range of the PWM output
		uint16_t pwm_val = gpio_analog_read(GPIO_ADC_A4) / 4;
		pwm_set_duty(pwm_val);

		Delay_Ms(100);
	
	}
}



/*** Functions ***************************************************************/
void pwm_init(void)
{
	// NOTE: Uses TIM2 Channel 3 (PC0) as the PWM Output pin

	// Enable TIM2 Clock
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	// Set GPIO-C0 OUTPUT 10MHz, Aleternate Function (Multiplex)
	gpio_set_mode(GPIO_PC0, OUTPUT_10MHZ_PP | OUTPUT_PP_AF);

	// Reset TIM2, Inits all registers
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;

	// Set Prescaler (46kHz)
	TIM2->PSC = 0x0003;
	// Set PWM Max Value (Autoreload Value)
	TIM2->ATRLR = 254;

	// Set the Compare Capture Register for Channel 3
	// TIM2_OC3M = 0b111 - PWM Mode 2 - Enable Preload
	TIM2->CHCTLR2 |= TIM_OC3M_2 | TIM_OC3M_1 | TIM_OC3M_0 | TIM_OC3PE;

	// Enable auto-reload
	TIM2->CTLR1 |= TIM_ARPE;

	// Enable channel output, set polarity ACTIVE_LOW
	TIM2->CCER |= TIM_CC3E | TIM_CC3P;

	// Initialise Counter
	TIM2->SWEVGR |= TIM_UG;

	// Enable TIM2
	TIM2->CTLR1 |= TIM_CEN;
}

void pwm_set_duty(uint32_t duty)
{
	TIM2->CH3CVR = duty;
}
