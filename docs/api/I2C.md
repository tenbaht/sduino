# I2C

The
[I2C master library](http://www.dsscircuits.com/articles/arduino-i2c-master-library)
by Wayne Truchsess offers some significant advantages over the Wire/TWI
library included in the standard arduino environment: It fixes some possible
deadlock situations, it allows for communication using a repeated start
condition as required by some devices, the code is much more compact and the
structure is easier to understand.

The current state of the port is quite incomplete. Missing parts:

- read functions
- speed setting (100kHz only)
- pullup setting
- does not include the deadlock protection


## Error codes

Return values for functions that use the timeOut feature will return at what
point in the transmission the timeout occurred.

All possible return values:

Errorcode | Meaning
---:	|---
  0	|Function executed with no errors
  1-7	|Timeout occurred, see above list
  8-0xFF|See datasheet for exact meaning


Looking at a full communication sequence between a master and slave
(transmit data and then readback data) there a total of 7 points in the
sequence where a timeout can occur. These are listed below and correspond to
the returned value.

Errorcode | Meaning: Waiting for...
---:	|---
  1	|Waiting for successful completion of a Start bit
  2	|Waiting for ACK/NACK while addressing slave in transmit mode (MT)
  3	|Waiting for ACK/NACK while sending data to the slave
  4	|Waiting for successful completion of a Repeated Start
  5	|Waiting for ACK/NACK while addressing slave in receiver mode (MR)
  6	|Waiting for ACK/NACK while receiving data from the slave
  7	|Waiting for successful completion of the Stop bit
