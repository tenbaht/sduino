#include "wiring-header.h"

/**
 * Helper function for STM8: Switch to the alternate pin functions
 *
 * a) flexible function: supports switching diffent AFR bits for
 * multiple set of pins. Not needed for stm8s103, but might be useful
 * for bigger parts.
 */
#ifdef SUPPORT_ALTERNATE_MAPPINGS
/*
void alternateFunction(uint8_t pin, uint8_t val)
{
	uint16_t optionbyte;
	uint8_t	afr;

	afr = digitalPinToAFR(pin);
	if (afr>7) return;	// ignore request on invalid pin

	optionbyte = FLASH_ReadOptionByte(OPT->OPT2);

	// flip the bit if the current state differs from requested state
	if ( ((optionbyte & (1<<afr) == 0) ^ (val==0) )
		FLASH_ProgramOptionByte(OPT->OPT2, (optionbyte&0xff)^(1<<afr));
	}
}
*/

/**
 * Helper function for STM8: Switch to the alternate pin functions
 *
 * b) simplified function: supports only bit AFR0 to switch pins C5, C6 and C7
 * from GPIO/SPI (default function) to PWM support (alternate function).
 */
void alternateFunction(uint8_t val)
{
	uint16_t optionbyte;

	optionbyte = FLASH_ReadOptionByte(0x4803) >> 8;
//	optionbyte = FLASH_ReadOptionByte(OPT->OPT2) >> 8;

	// flip the bit if the current state differs from requested state
	if ( (optionbyte & (1<<0) == 0) ^ (val==0) ) {
		FLASH_Unlock(FLASH_MEMTYPE_DATA);
//		FLASH_ProgramOptionByte(OPT->OPT2, (optionbyte&0xff)^(1<<0));
		FLASH_ProgramOptionByte(0x4803, optionbyte^(1<<0));
		FLASH_Lock(FLASH_MEMTYPE_DATA);
	}
}
#endif

