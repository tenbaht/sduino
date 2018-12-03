# EEPROM

This is a library to access the built-in EEPROM of the STM8 MCUs. It does
**not** work with external I2C or SPI-EEPROM.

It is based on the Arduino EEPROM Library V2.0 by Christopher Andrews.
In addition to the common Arduino-methods it defines some functions for
low-level access. These functions behave similar to the built-in AVR
functions. (see [below](#avr-api))


## Arduino-API

This is a pre-instantiated singleton library. It is not possible to use more
than one instance per sketch or to change the instance name.

The API syntax is very similar to the original C++ syntax. In most cases
replacing the dots in the method names for underscores is all it needs. Only
the `EEPROM[]` operator and the `EEPtr` and `EERef` behave a little
different than their Arduino counterparts.


Arduino syntax			|sduino syntax
--------------------		|---------------------
`val = EEPROM.read(idx)`	|`val = EEPROM_read(idx)`
`EEPROM.write(idx, val)`	|`EEPROM_write(idx, val)`
`EEPROM.update(idx, val)`	|`EEPROM_update(idx, val)`
`val = EEPROM.begin()`		|`val = EEPROM_begin()`
`val = EEPROM.end()`		|`val = EEPROM_end()`
`val = EEPROM.length()`		|`val = EEPROM_length()`
`EEPROM.put(idx,T)`		|`EEPROM_put(idx,T)`
`EEPROM.get(idx,T)`		|`EEPROM_get(idx,T)`
`EEPROM[idx]`			|`EEPROM_cell[idx]`


#### **`EEPROM_read( address )`** [[_example_]](EEPROM/examples/eeprom_read.md)

This function allows you to read a single byte of data from the eeprom. Its
only parameter is an `int` which should be set to the address you wish to
read.

The function returns an `unsigned char` containing the value read.


#### **`EEPROM_write( address, value )`** [[_example_]](EEPROM/examples/eeprom_write.md)

The `EEPROM_write()` method allows you to write a single byte of data to the
EEPROM. Two parameters are needed. The first is an `int` containing the
address that is to be written, and the second is a the data to be written
(`unsigned char`).

This function does not return any value.


#### **`EEPROM_update( address, value )`** [[_example_]](EEPROM/examples/eeprom_update.md)

This function is similar to `EEPROM_write()` however this method will only
write data if the cell contents pointed to by `address` is different to
`value`. This method can help prevent unnecessary wear on the EEPROM cells.

This function does not return any value.


#### **`EEPROM_get( address, object )`** [[_example_]](EEPROM/examples/eeprom_get.md)

This function will retrieve any object from the EEPROM. Two parameters are
needed to call this function. The first is an `int` containing the address
that is to be written, and the second is the object you would like to read.

This function is implemented as a macro. On completion, it evaluates to a
pointer to the `object` passed in. It does not need to be used and is only
returned for convience.

**Note:** This means that the expression for `object` will be evaluated
twice. This **will break sketches** using pre- or postincrement operators,
because they will end up beeing called twice.


#### **`EEPROM_put( address, object )`** [[_example_]](EEPROM/examples/eeprom_put.md)

This function will write any object to the EEPROM. Two parameters are needed
to call this function. The first is an `int` containing the address that is
to be written, and the second is the object you would like to write.

This function uses the _update_ method to write its data, and therefore only
rewrites changed cells.

This function is implemented as a macro using `&object` and `sizeof(object)`
to reference the actual data to be written. This means that:

- integer constants can't be directly written, `EEPROM_put(0,42)` will fail.
- The expression for `object` will be evaluated twice. This will break
  sketches using pre- or postincrement operators.

This macro evaluates to a pointer to the `object` passed in. It does not
need to be used and is only returned for convience.


#### **`EEPROM_length()`**

This function returns an `unsigned int` containing the number of cells in
the EEPROM.


#### **`EEPROM_begin()`**

This function returns the number of the first cell in the EEPROM. This is
similar to the semantics of the original libray, where this method is ment
for use with STL objects, custom iteration and C++11 style ranged for loops.


#### **`EEPROM_end()`**

This function returns the number just behind the last EEPROM cell. Since the
EEPROM cells always start at zero this is the number of EEPROM cells. Used
with `EEPROM_begin()` to provide custom iteration.


#### **Subscript operator: `EEPROM_cell[address]`**

Use the syntax `EEPROM_cell[address]` instead. This allows acessing the
EEPROM like an array. EEPROM cells can be read _and_ **_written_** directly
using this method.

**Note:** For write access, the EEPROM has to be unlocked by calling
`eeprom_unlock()` first. Trying to write the EEPROM without unlocking will
permanently lock the EEPROM after three failed write accesses. Only a
hardware reset can re-enable the EEPROM.

Please keep in mind that every call of `EEPROM_write()` or `EEPROM_update()`
re-locks the EEPROM after the access.





## AVR-API

In addition to the common Arduino-methods it defines some functions for
low-level access. These functions behave similar to the built-in AVR
functions.

For the sake of compatibility to the Arduino world all EEPROM addresses
start at zero at the needed offset calculation is handled internally.

AVR/Arduino syntax	|sduino syntax		|Description
--------------------	|---------------------	|-----------------
N/A			|`eeprom_unlock()`	|
N/A			|`eeprom_lock()`	|
N/A			|`s = eeprom_is_unlocked()`|returns true if EEPROM is unlocked
`s = eeprom_is_ready()`	|`s = eeprom_is_ready()`|returns true if EEPROM is ready (no write in progress)
`eeprom_write_block(idx, *src, len)`|`eeprom_write_block(idx, *src, len)`|
`eeprom_update_block(idx, *src, len)`|`eeprom_update_block(idx, *src, len)`|
`eeprom_read_block(idx, *dest, len)`|`eeprom_read_block(idx, *dest, len)`|


#### **Access control: `eeprom_lock()`/`eeprom_unlock()`**

To prevent accidental modifications of the EEPROM contents, write access is blocked by default. For write access, the EEPROM has to be unlocked first. This can be done by calling `eeprom_unlock()`.

After accessing the EEPROM it can be re-locked by calling `eeprom_lock()`. Please keep in mind that every call of `EEPROM_write()` or `EEPROM_update()` re-locks the EEPROM after the access.

**Note:** Without unlocking, the EEPROM will be permanently locked until the next hardware reset after three failed write accesses.

#### **Status check: `eeprom_is_ready()`**

A write access is slow. To avoid stalling the MCU for a longer time by busy waiting it is possible to check the current write status. This can be used to spread out single byte writes over time and use the waiting time in a more productive way.

This macro returns a non-zero value if the EEPROM is ready for a new write access.



## Example

Stores values read from analog input 0 into the EEPROM. These values will
stay in the EEPROM when the board is turned off and may be retrieved later
by another sketch.

```c
#include <EEPROM.h>

int addr = 0;

void setup()
{
}

void loop()
{
	int val = analogRead(0) / 4;

	EEPROM_write(addr, val);

	addr = addr + 1;
	if (addr == EEPROM_length()) {
		addr = 0;
	}

	delay(100);
}
```


## Accessing the EEPROM using `stm8flash`


Write simple test data:

```bash
$ echo -n "abcdefgh" > testdata.bin
$ hd testdata.bin
00000000  61 62 63 64 65 66 67 68                           |abcdefgh|
00000008
$ stm8flash -c stlinkv2 -p "stm8s103?3" -s eeprom -w testdata.bin
Determine EEPROM area
Due to its file extension (or lack thereof), "testdata.bin" is considered as RAW BINARY format!
8 bytes at 0x4000... OK
Bytes written: 8
```

read the current EEPROM content:

```bash
$ stm8flash -c stlinkv2 -p "stm8s103?3" -s eeprom -r e2.img
Determine EEPROM area
Due to its file extension (or lack thereof), "e2.img" is considered as RAW BINARY format!
Reading 640 bytes at 0x4000... OK
Bytes received: 640
$ hd e2.img 
00000000  61 62 63 64 65 66 67 68  00 00 00 00 00 00 00 00  |abcdefgh........|
00000010  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000280
```
