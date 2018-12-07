# I2C

The
[I2C master library](http://www.dsscircuits.com/articles/arduino-i2c-master-library)
by Wayne Truchsess offers some significant advantages over the Wire/TWI
library included in the standard arduino environment: It fixes some possible
deadlock situations, it allows for communication using a repeated start
condition as required by some devices, the code is much more compact and the
structure is easier to understand.


## API

This is a pre-instantiated singleton library. It is not possible to use more
than one instance per sketch or to change the instance name.

The API syntax is very similar to the original C++ syntax. Some name
mangeling was needed to distinguish the different variant of the `read()`
and `write()` method. Apart from this replacing the dots in the method names
for underscores is all it needs.

The pullup setting is missing because this function is not supported by the
STM8 hardware.


Arduino syntax				|sduino syntax
--------------------			|---------------------
`I2C.begin()`				|`I2C_begin()`
`I2C.end()`				|`I2C_end()`
`I2C.timeOut(val)`			|`I2C_timeOut(val)`
`I2C.setSpeed(mode)`			|`I2C_setSpeed(mode)`
`I2C.pullup(active)`			|not implemented
`I2C.scan()`				|`I2C_scan()`
`n = I2C.available()`			|`n = I2C_available()`
`val = I2C.receive()`			|`val = I2C_receive()`
`I2C.write(addr, val)`			|`I2C_write(addr, val)`
`I2C.write(addr, reg, val)`		|`I2C_write_reg(addr, reg, val)`
`I2C.write(addr, reg, string)`		|`I2C_write_s(addr, reg, string)`
`I2C.write(addr, reg, *data, len)`	|`I2C_write_sn(addr, reg, *data, len)`
`I2C.read(addr, n)`			|`I2C_read(addr, n)`
`I2C.read(addr, reg, n)`		|`I2C_read_reg(addr, reg, n)`
`I2C.read(addr, n, *buf)`		|`I2C_readbuf(addr, n, *buf)`
`I2C.read(addr, reg, n, *buf)`		|`I2C_readbuf_reg(addr, reg, n, *buf)`



### Error codes

All functions return an error code. Return values for functions that use the
timeOut feature will return at what point in the transmission the timeout
occurred.

Looking at a full communication sequence between a master and slave
(transmit data and then readback data) there a total of 7 points in the
sequence where a timeout can occur. These are listed below and correspond to
the returned value.

Errorcode | Meaning: Waiting for...
---:	|---
  0	|Function executed with no errors
  1	|Waiting for successful completion of a Start bit
  2	|Waiting for ACK/NACK while addressing slave in transmit mode (MT)
  3	|Waiting for ACK/NACK while sending data to the slave
  4	|Waiting for successful completion of a Repeated Start
  5	|Waiting for ACK/NACK while addressing slave in receiver mode (MR)
  6	|Waiting for ACK/NACK while receiving data from the slave
  7	|Waiting for successful completion of the Stop bit



### AVR error codes

For reference: The codes listed below where defined for the AVR, but they
don't exist for the STM8 implementation. Combining the values of SR1 and SR2
it would be possible to implement most of these conditions (apart from
0x20), but so far I haven't seen any program needing these values, so I
didn't implement this.


Status Codes| for Master Transmitter Mode
---	|---
0x08	|A START condition has been transmitted
0x10	|A repeated START condition has been transmitted
0x18	|SLA+W has been transmitted; ACK has been received
0x20	|SLA+W has been transmitted; NOT ACK has been received
0x28	|Data byte has been transmitted; ACK has been received
0x30	|Data byte has been transmitted; NOT ACK has been received
0x38	|Arbitration lost in SLA+W or data bytes

Status Codes| for Master Receiver Mode
---	|---
0x08	|A START condition has been transmitted
0x10	|A repeated START condition has been transmitted
0x38	|Arbitration lost in SLA+R or NOT ACK bit
0x40	|SLA+R has been transmitted; ACK has been received
0x48	|SLA+R has been transmitted; NOT ACK has been received
0x50	|Data byte has been received; ACK has been returned
0x58	|Data byte has been received; NOT ACK has been returned


Status Codes| independed of mode
---	|---
0xF8	|No relevant state information available; TWINT = “0”
0x00	|Bus error due to an illegal START or STOP condition




## inner workings

start(): Sends start bit, does not wait for SB flag.
sendAddress()


## Further reading

Programming the I2C communication on the register level is quite complex for
the STM8, especially in receive mode. The most detailed information on this
topic is the

- application note AN3281 bundeled with
- the related source code package [STSW-STM8004](https://www.st.com/en/embedded-software/stsw-stm8004.html)

Downloading from the ST website requires a (free) registration. Somebody
uploaded the full package to github:
https://github.com/jiaohaitao/stsw-stm8004

There are some important notes on I2C implementation in the errata sheets:

- [STM8S001J3/003xx/103xx/903xx Errata sheet, rev. 5
  (CD00265449)](https://www.st.com/content/ccc/resource/technical/document/errata_sheet/c9/f9/ef/bf/63/91/4a/1f/CD00265449.pdf/files/CD00265449.pdf/jcr:content/translations/en.CD00265449.pdf)
- [STM8S005xx STM8S105xx Errata sheet, rev. 7
  (CD00270741)](https://www.st.com/content/ccc/resource/technical/document/errata_sheet/e3/c3/4e/24/0c/ca/4b/e7/CD00270741.pdf/files/CD00270741.pdf/jcr:content/translations/en.CD00270741.pdf)
- [STM8S007xx STM8S20xxx Errata sheet, rev. 6
  (CD00244749)](https://www.st.com/content/ccc/resource/technical/document/errata_sheet/7a/94/8f/fe/84/14/41/6d/CD00244749.pdf/files/CD00244749.pdf/jcr:content/translations/en.CD00244749.pdf)
