/******************************************************************************
* lib_GPIOCTRL
* A runtime-capable GPIO Library, with Digital Read/Write and
* TODO: Analog Read & Analog Write/PWM
*
*
* See GitHub for details: https://github.com/ADBeta/CH32V003_lib_GPIOCTRL
*
* ADBeta (c) 2024
******************************************************************************/
#include "lib_GPIOCTRL.h"

#include <stdint.h>


// TODO:
gpio_port_reg_t *gpio_port_reg[4] = {
	GPIO_PORTA,
	NULL,
	GPIO_PORTC,
	GPIO_PORTD,
};


/// @breif Sets the Config and other needed Registers for a passed pin and mode
/// @param gpio_pin_t pin, the GPIO Pin & Port Variable (e.g GPIO_PD6)
/// @param gpio_mode_t mode, the GPIO Mode Variable (e.g OUTPUT_10MHZ_PP)
/// @return None
void gpio_set_mode(const gpio_pin_t pin, const gpio_mode_t mode)
{
	// TODO: Any pin over 8 needs to change CFGHR
	
	// Make array of uint8_t from [pin] enum. See definition for details
	uint8_t *byte = (uint8_t *)&pin;
	
	// Set the RCC Register to enable clock on the specified port
	*RCC_APB2PCENR |= (APB2PCENR_AFIO | (APB2PCENR_IOPxEN << byte[0]));

	// Clear then set the GPIO Config Register
	gpio_port_reg[ byte[0] ]->CFGLR &=        ~(0x0F  << (4 * byte[1]));
	gpio_port_reg[ byte[0] ]->CFGLR |=  (mode & 0x0F) << (4 * byte[1]);

	// If [mode] is INPUT_PULLUP or INPUT_PULLDOWN, set the [OUTDR] Register
	if(mode == INPUT_PULLUP || mode == INPUT_PULLDOWN)
		gpio_digital_write(pin, mode >> 4);
}

/// @breif Sets the OUTDR Register for the passed Pin
/// @param gpio_pin_t pin, the GPIO Pin & Port Variable (e.g GPIO_PD6)
/// @param gpio_state_t state, GPIO State to be set (e.g GPIO_HIGH)
/// @return None
__attribute__((always_inline))
inline void gpio_digital_write(const gpio_pin_t pin, const gpio_state_t state)
{
	// Make array of uint8_t from [pin] enum. See definition for details
	uint8_t *byte = (uint8_t *)&pin;

	uint32_t mask = 0x01 << byte[1];          // Shift by pin number
	if(state == GPIO_LOW) mask = mask << 16;  // Shift by 16 if LOW, to Reset

	gpio_port_reg[ byte[0] ]->BSHR = mask;
}

/// @breif Reads the INDR Register of the specified pin and returns state
/// @param gpio_pin_t pin, the GPIO Pin & Port Variable (e.g GPIO_PD6)
/// @return gpio_state_t, the current state of the pin, (e.g GPIO_HIGH)
__attribute__((always_inline))
inline gpio_state_t gpio_digital_read(const gpio_pin_t pin)
{
	// Make array of uint8_t from [pin] enum. See definition for details
	uint8_t *byte = (uint8_t *)&pin;

	// If the Input Reg has the wanted bit set, return HIGH
	if( (gpio_port_reg[ byte[0] ]->INDR & (0x01 << byte[1])) != 0x00 ) 
		return GPIO_HIGH;

	// else return LOW 
	return GPIO_LOW;
}


