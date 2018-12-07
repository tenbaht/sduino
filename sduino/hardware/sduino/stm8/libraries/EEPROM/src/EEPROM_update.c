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

#include "EEPROM.h"


/* --- Arduino-like interface -------------------------------------------- */

void EEPROM_update( int idx, uint8_t val )
{
	// make sure not to write data beyond the end of the EEPROM area
	// (this could accidentally hit the option byte area)
	if (idx >= EEPROM_end()) return;
	idx += (uint16_t)FLASH_DATA_START_PHYSICAL_ADDRESS;

	if (*((uint8_t*)idx) != val)
	{
		eeprom_unlock();
		if (eeprom_is_unlocked())
		{
			// write only after a successful unlock.
			*((uint8_t*)idx) = val;
			// re-lock the EEPROM again.
			FLASH->IAPSR &= FLASH_FLAG_DUL;
		}
	}
}
