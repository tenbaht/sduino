Let's ask the forbidden question:

# Why?

The fine line between sense and nonsense.


## When to use an STM8 - and when not

### Hobby applications

The fairly new ESP-14 module includes a STM8S003F3P6: Wifi and a
programmable I/O-CPU for just over two dollars - that might be the most
compelling reason to get started on the STM8S series. Apart from pure
curiosity and eagerness to learn something new, of course.

The simple STM8S103F breakout boards are powerful and dirt cheap. They cost
well under one dollar. You can get three boards and one flash programmer
together for well under five dollars on
[aliexpress](https://www.aliexpress.com/wholesale?SearchText=stm8s103f3p6),
including shipping from China.

*That is an extremely good opportunity to learn something new.*

The major downside of this CPU series is the lack of information and
community support for the STM8. The community support and the sheer number
of existing libraries for all kinds of sensors and hardware is outstanding
in the Arduino world. 

Don't let the temping price tag be your only guide. An Arduino Nano
compatible board is not *that* much more expensive: They start at $1.90 on
[aliexpress](https://www.aliexpress.com/wholesale?SearchText=arduino+nano).
That is only $1.20 more. I am sure you can afford that.

*If you just want to get something done, go for an
Arduino board. Nothing will give you faster and easier results.*

(Or an STM32F103 bluepill for the same price if you need more computing
power).


### Tinkering and Repurposing

The STM8 series by ST might be the cheapest CPUs on the market while still
being as powerful as the ATmega series. That makes them every Chinese
engineer's darling and chances are pretty good that you will find an '003 if
you crack open any cheap appliance like an irrigation, temperature, or
charging controller.

Things like the [W1209 thermostat boards](https://www.aliexpress.com/wholesale?SearchText=w1209)
could be useful for almost every kind of sensor application. There is plenty
of cheap stuff around that only needs a different firmware to be used for
something.

And Sduino might be just the right tool to help you doing this.



### Commercial applications

For commercial use the STM8S offers some interesting advantages:

**Motor control**: The STM8 has a strong focus on motor and position control
systems. Things you need to handle yourself on an ATmega are implemented in
hardware and work independently of the state of the software. There is even
hardware support for quadrature encoders as used in position sensors and
rotary encoders.

**Low power modes**: The numbers in the datasheets don't look that different,
but in real life the STM8 can be powered two or three times longer using the
same battery capacity due to the finer control on the power modes (very,
very careful programming required).

**Value for the money**: 40 to 60 cents for a STM8 with 14 I/O pins compared to
$1.60-$3.00 for an ATmega8.

**Upgrade path**: The peripheral units of the STM8 are identical or at least
very, very similar to the ones used with the STM32 family of 32 bit
ARM-Cortex CPUs. This makes it is relatively easy to migrate existing
software between the 8- and the 32-bit world. This is quite unique among the
other CPUs.

