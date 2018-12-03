## **EEPROM Library V1.0** for SDuino

**Written by:** _Michael Mayer_.
**Based on the Arduino EEPROM Library V2.0 by:** _Christopher Andrews_.

### **What is the EEPROM library.**

The EEPROM library provides an easy to use interface to interact with the internal non-volatile storage found in STM8 microprocessors.


### **How to use it**

The EEPROM library is included in your IDE download. To add its functionality to your sketch you'll need to reference the library header file. You do this by adding an include directive to the top of your sketch.

```Arduino
#include <EEPROM.h>

void setup(){

}

void loop(){

}

```

The library provides a global variable named `EEPROM`, you use this variable to access the library functions. The methods provided in the EEPROM class are listed below.

You can view all the examples [here](examples/).

### **Library functions**

For the sake of compatibility to the Arduino world all EEPROM addresses start at zero at the needed offset calculation is handled internally.


#### **`EEPROM_read( address )`** [[_example_]](examples/eeprom_read/eeprom_read.ino)

This function allows you to read a single byte of data from the eeprom.
Its only parameter is an `int` which should be set to the address you wish to read.

The function returns an `unsigned char` containing the value read.

#### **`EEPROM_write( address, value )`** [[_example_]](examples/eeprom_write/eeprom_write.ino)

The `write()` method allows you to write a single byte of data to the EEPROM.
Two parameters are needed. The first is an `int` containing the address that is to be written, and the second is a the data to be written (`unsigned char`).

This function does not return any value.

#### **`EEPROM_update( address, value )`** [[_example_]](examples/eeprom_update/eeprom_update.ino)

This function is similar to `EEPROM_write()` however this method will only write data if the cell contents pointed to by `address` is different to `value`. This method can help prevent unnecessary wear on the EEPROM cells.

This function does not return any value.

#### **`EEPROM_get( address, object )`** [[_example_]](examples/eeprom_get/eeprom_get.ino)

This function will retrieve any object from the EEPROM.
Two parameters are needed to call this function. The first is an `int` containing the address that is to be written, and the second is the object you would like to read.

This function is implemented as a macro. On completion, it evaluates to a pointer to the `object` passed in. It does not need to be used and is only returned for convience. But this means, that the expression for `object` will be evaluated twice. This will break  sketches using pre- or postincrement operators, because they will end up beeing called twice.

#### **`EEPROM_put( address, object )`** [[_example_]](examples/eeprom_put/eeprom_put.ino)

This function will write any object to the EEPROM.
Two parameters are needed to call this function. The first is an `int` containing the address that is to be written, and the second is the object you would like to write.

This function uses the _update_ method to write its data, and therefore only rewrites changed cells.

This function is implemented as a macro using `&object` and `sizeof(object)` to reference the actual data to be written. This means that:

- integer constants can't be directly written, `EEPROM_put(0,42)` will fail.
- The expression for `object` will be evaluated twice. This will break  sketches using pre- or postincrement operators.

This macro evaluates to a pointer to the `object` passed in. It does not need to be used and is only returned for convience.

#### **`EEPROM_length()`**

This function returns an `unsigned int` containing the number of cells in the EEPROM.

#### **`EEPROM_begin()`**

This function returns the number of the first cell in the EEPROM. This is similar to the semantics of the original libray, where this method is ment for use with STL objects, custom iteration and C++11 style ranged for loops.

#### **`EEPROM_end()`**

This function returns the number just behind the last EEPROM cell. Since the EEPROM cells always start at zero this is the number of EEPROM cells.
Used with `begin()` to provide custom iteration.

---

### additional features

#### **Access control: `eeprom_lock()`/`eeprom_unlock()`**

To prevent accidental modifications of the EEPROM contents, write access is blocked by default. For write access, the EEPROM has to be unlocked first. This can be done by calling `eeprom_unlock()`.

After accessing the EEPROM it can be re-locked by calling `eeprom_lock()`. Please keep in mind that every call of `EEPROM_write()` or `EEPROM_update()` re-locks the EEPROM after the access.

**Note:** Without unlocking, the EEPROM will be permanently locked until the next hardware reset after three failed write accesses.

#### **Status check: `eeprom_is_ready()`**

A write access is slow. To avoid stalling the MCU for a longer time by busy waiting it is possible to check the current write status. This can be used to spread out single byte writes over time and use the waiting time in a more productive way.

This macro returns a non-zero value if the EEPROM is ready for a new write access.

---

### not implemented

#### **Subscript operator: `EEPROM[address]`**

Use the syntax `EEPROM_cell[address]` instead. This allows acessing the EEPROM like an array. EEPROM cells can be read _and_ **_written_** directly using this method.

**Note:** For write access, the EEPROM has to be unlocked by calling `eeprom_unlock()` first. Without unlocking, the EEPROM will be permanently locked until the next hardware reset after three failed write accesses.

Please keep in mind that every call of `EEPROM_write()` or `EEPROM_update()` re-locks the EEPROM after the access.


#### **`EERef` class**

This object references an EEPROM cell.
Its purpose is to mimic a typical byte of RAM, however its storage is the EEPROM.
This class has an overhead of two bytes, similar to storing a pointer to an EEPROM cell.

```C++
EERef ref = EEPROM[ 10 ]; //Create a reference to 11th cell.

ref = 4; //write to EEPROM cell.

unsigned char val = ref; //Read referenced cell.
```

#### **`EEPtr` class**

This object is a bidirectional pointer to EEPROM cells represented by `EERef` objects.
Just like a normal pointer type, this type can be dereferenced and repositioned using
increment/decrement operators.

```C++
EEPtr ptr = 10; //Create a pointer to 11th cell.

*ptr = 4; //dereference and write to EEPROM cell.

unsigned char val = *ptr; //dereference and read.

ptr++; //Move to next EEPROM cell.
```


