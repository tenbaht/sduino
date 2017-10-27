# Bare metal programming

It all started by compiling some plain example programs without using any
libraries. This avoids the code overhead of the Arduino libraries.

Lujji posted a [series of
articles](https://lujji.github.io/blog/bare-metal-programming-stm8/) on bare
metal programming of the STM8S CPUs. This is by far the best introduction I
found so far. It would have helped me a lot if it would have been around at
the time when I started this project.




## Modifications for the sdcc example programs

I started my journey into the STM8 world with the
[stm8-examples-sdcc](https://github.com/vdudouyt/sdcc-examples-stm8) by
Valentin Dudouyt (vdudouyt), the author of
[stm8flash](https://github.com/vdudouyt/stm8flash).

These examples are still part of the repository in
examples/sdcc-examples-stm8. To compile for my STM8S103F3 board they
required some modifications:

**blinky.c**: LED pin assignment

**uart.c**:  pin assignment (TX is at PD5, RX is at PD6).  
The UART is sending at 1200 Baud => CPU clock only 2MHz instead of 16MHz.
The clock divider needs to be configured or a different baud rate prescale value
has to be used. Pitfall: The register address for the clock divider is
different for the STM8S and the STM8L.

