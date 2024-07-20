/******************************************************************************
* lib_GPIOCTRL
* A runtime-capable GPIO Library, with Digital Read/Write and
* TODO: Analog Read & Analog Write/PWM
*
*
* See GitHub for details: https://github.com/ADBeta/CH32V003_lib_GPIOCTRL
*
* ADBeta (c)    Jun 2024    Ver 0.5.0
******************************************************************************/
#ifndef LIB_GPIOCTRL_H
#define LIB_GPIOCTRL_H

#include <stdint.h>
#include <stddef.h>

/*** GPIO Pin Enumeration ****************************************************/
/// @breif This enum is used as binary data for pin and port addressing. 
/// 0x[PIN][PORT] - 0x 0603 6th pin of port 4 (PORTD)
/// NOTE: Little-Endian Architecture means value in memory is [PORT][PIN] order
typedef enum {
	GPIO_PA0      = 0x0000,
	GPIO_PA1      = 0x0100, 
	GPIO_PA2      = 0x0200, 
	GPIO_PA3      = 0x0300,
	GPIO_PA4      = 0x0400,
	GPIO_PA5      = 0x0500,
	GPIO_PA6      = 0x0600,
	GPIO_PA7      = 0x0700,

	GPIO_PB0      = 0x0001,
	GPIO_PB1      = 0x0101, 
	GPIO_PB2      = 0x0201, 
	GPIO_PB3      = 0x0301,
	GPIO_PB4      = 0x0401,
	GPIO_PB5      = 0x0501,
	GPIO_PB6      = 0x0601,
	GPIO_PB7      = 0x0701,

	GPIO_PC0      = 0x0002,
	GPIO_PC1      = 0x0102, 
	GPIO_PC2      = 0x0202, 
	GPIO_PC3      = 0x0302,
	GPIO_PC4      = 0x0402,
	GPIO_PC5      = 0x0502,
	GPIO_PC6      = 0x0602,
	GPIO_PC7      = 0x0702,

	GPIO_PD0      = 0x0003,
	GPIO_PD1      = 0x0103, 
	GPIO_PD2      = 0x0203, 
	GPIO_PD3      = 0x0303,
	GPIO_PD4      = 0x0403,
	GPIO_PD5      = 0x0503,
	GPIO_PD6      = 0x0603,
	GPIO_PD7      = 0x0703,
} gpio_pin_t;


/*** GPIO Pin Mode Enumeration ***********************************************/
/// @breif GPIO Pin Mode data. The lower nibble is the raw binary data for the
/// [R32_GPIOx_CFGLR] Register. The upper nibble is used for additional flags 
typedef enum {
	INPUT_ANALOG       = 0x00,
	INPUT_FLOATING     = 0x04,
	// Mapped to INPUT_PP, Sets OUTDR based on the upper nibble
	INPUT_PULLUP       = 0x18,
	INPUT_PULLDOWN     = 0x08,
	//
	OUTPUT_10MHZ_PP    = 0x01,
	OUTPUT_10MHZ_OD    = 0x05,
	//
	OUTPUT_2MHZ_PP     = 0x02,
	OUTPUT_2MHZ_OD     = 0x06,
	//
	OUTPUT_PP_AF       = 0x08,
	OUTPUT_OD_AF       = 0x0C,
} gpio_mode_t;


/*** GPIO Output State Enumerations ******************************************/
/// @breif GPIO Pin State Enum, simple implimentation of a HIGH/LOW System
typedef enum {
	GPIO_LOW     = 0x00,
	GPIO_HIGH    = 0x01,
} gpio_state_t;


/*** Registers for GPIO Port *************************************************/
/// @breif GPIO Port Register, Directly Maps to Memory starting at 
/// [R32_GPIOx_CFGLR] for each PORT Respectively
typedef struct {
	volatile uint32_t CFGLR;  // Configuration Register (lower)
	volatile uint32_t CFGHR;  // Configuration Register (upper)
	volatile uint32_t INDR;   // Input Data Register
	volatile uint32_t OUTDR;  // Output Data Register
	volatile uint32_t BSHR;   // Set/Reset Register
	volatile uint32_t BCR;    // Port Reset Register
	volatile uint32_t LCKR;   // Lock Register
} gpio_port_reg_t;


/*** Register Address Definitions ********************************************/
#define RCC_APB2PCENR ((volatile uint32_t *)0x40021018)
#define APB2PCENR_AFIO   0x01
#define APB2PCENR_IOPxEN 0x04

#define PORTA_GPIO_REGISTER_BASE 0x40010800
// NOTE: PORTB is not available for the CH32V003.
#define PORTB_GPIO_REGISTER_BASE 0x40010C00
#define PORTC_GPIO_REGISTER_BASE 0x40011000
#define PORTD_GPIO_REGISTER_BASE 0x40011400

#define GPIO_PORTA ((gpio_port_reg_t *)PORTA_GPIO_REGISTER_BASE)
// NOTE: PORTB is not available for the CH32V003.
#define GPIO_PORTB ((gpio_port_reg_t *)PORTB_GPIO_REGISTER_BASE)
#define GPIO_PORTC ((gpio_port_reg_t *)PORTC_GPIO_REGISTER_BASE)
#define GPIO_PORTD ((gpio_port_reg_t *)PORTD_GPIO_REGISTER_BASE)

/// @breif The GPIO Ports are places into an array for easy indexing in the
/// GPIO Functions
/// NOTE: Only 3 PORTs are usable in the CH32V003, 4 for other MCUs
extern gpio_port_reg_t *gpio_port_reg[4]; 


/*** GPIO Mode Setting *******************************************************/
/// @breif Sets the Config and other needed Registers for a passed pin and mode
/// @param gpio_pin_t pin, the GPIO Pin & Port Variable (e.g GPIO_PD6)
/// @param gpio_mode_t mode, the GPIO Mode Variable (e.g OUTPUT_10MHZ_PP)
/// @return None
void gpio_set_mode(const gpio_pin_t pin, const gpio_mode_t mode);

/*** Digital Write/Read ******************************************************/
/// @breif Sets the OUTDR Register for the passed Pin
/// @param gpio_pin_t pin, the GPIO Pin & Port Variable (e.g GPIO_PD6)
/// @param gpio_state_t state, GPIO State to be set (e.g GPIO_HIGH)
/// @return None
void gpio_digital_write(const gpio_pin_t pin, const gpio_state_t state);

/// @breif Reads the INDR Register of the specified pin and returns state
/// @param gpio_pin_t pin, the GPIO Pin & Port Variable (e.g GPIO_PD6)
/// @return gpio_state_t, the current state of the pin, (e.g GPIO_HIGH)
gpio_state_t gpio_digital_read(const gpio_pin_t pin);


#endif
