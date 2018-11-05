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


## AVR error codes

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

Es braucht also SR1 und SR2
0x20 ist nicht abzubilden.
