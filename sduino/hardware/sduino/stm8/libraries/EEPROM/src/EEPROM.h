/*
  EEPROM.h - EEPROM library
  Plain-C version for SDuino by Michael Mayer 2018.

  This is a rewrite from scratch. The basic API is inspired by the
  Arduino EEPROM library, but none of the operator overloading functions
  can be ported in any sensible way to C.

  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

#include <stddef.h>     /* size_t */
#include <stdint.h>
#include <stm8s.h>

/* --- Arduino-like interface -------------------------------------------- */

// compatibility definitions
#define E2END		(FLASH_DATA_BLOCKS_NUMBER * FLASH_BLOCK_SIZE - 1)
#define EERef(N)	(*((uint8_t*)((uint16_t)FLASH_DATA_START_PHYSICAL_ADDRESS)+(N)))

typedef uint16_t EEPtr;
//#define EEPtr uint16_t

#define EEPROM_cell	((uint8_t*)((uint16_t)FLASH_DATA_START_PHYSICAL_ADDRESS))

inline uint8_t EEPROM_read( int idx )		{ return EERef( idx ); }
void EEPROM_write( int idx, uint8_t val );
void EEPROM_update( int idx, uint8_t val );

inline EEPtr EEPROM_begin()			{ return 0x00; }
inline EEPtr EEPROM_end()			{ return E2END + 1; }
inline uint16_t EEPROM_length()			{ return E2END + 1; }

//Functionality to 'get' and 'put' objects to and from EEPROM.
#define EEPROM_put(idx,T)	eeprom_update_block(idx,(uint8_t*)(&(T)),sizeof(T));&(T)
#define EEPROM_get(idx,T)	eeprom_read_block(idx,(uint8_t*)(&(T)),sizeof(T));&(T)


/* --- a more AVR-like interface ----------------------------------------- */

void eeprom_unlock(void);
void eeprom_lock(void);

// returns true if EEPROM is unlocked
inline uint8_t eeprom_is_unlocked(void) {return (FLASH->IAPSR & FLASH_FLAG_DUL);}

// returns true if EEPROM is ready (no write in progress)
inline uint8_t eeprom_is_ready(void) {return (FLASH->IAPSR & FLASH_IAPSR_EOP);}

void eeprom_write_block(uint16_t idx, const uint8_t *ptr, size_t len);
void eeprom_update_block(uint16_t idx, uint8_t *ptr, size_t len);
void eeprom_read_block(const uint16_t idx, uint8_t *ptr, size_t len);

#endif /* EEPROM_h */
