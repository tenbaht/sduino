# I2C

The
[I2C master library](http://www.dsscircuits.com/articles/arduino-i2c-master-library)
by Wayne Truchsess offers some significant advantages over the Wire/TWI
library included in the standard arduino environment: It fixes some possible
deadlock situations, it allows for communication using a repeated start
condition as required by some devices, the code is much more compact and the
structure is easier to understand.

The current state of the port does not include the deadlock protection,
though.
