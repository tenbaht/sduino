# Pin change interrupts

`attachInterrupt()` and `detachInterrupt()` implement the pin change
external interrupts of the STM8.

Pin change interrupts are supported for all pins on ports PA, PB, PC, PD and
PE except for PA0, PA1, PA7 and PD7. The higher ports PF..PJ (high pin count
CPUs only) don't support interrupts.

Pin change interrupt means there is only one common ISR vector for every
port. E.g. pin PB2 and PB3 share a common ISR routine and the ISR has
to figure out which pin triggered the interrupt.


## Description

The first parameter to `attachInterrupt()` and `detachInterrupt()` is an
interrupt number. For SDuino this number is identical with the pin number,
but for compatibility reasons you should still use
`digitalPinToInterrupt(pin)` to translate the actual digital pin to the
specific interrupt number. For example, if you connect to pin 3, use
`digitalPinToInterrupt(3)` as the first parameter to `attachInterrupt()`.


## About the Interrupt Service Routine

The ISR should be as short and fast as possible. Other interrupts are
blocked while the ISR is running. This means `millis()` will not increment
and `delay()` can't work. `delayMicroseconds()` does not use any counter, so
it will work as normal.

Typically global variables are used to pass data between an ISR and the main
program. To make sure variables shared between an ISR and the main program
are updated correctly, declare them as `volatile`.




## Syntax

`attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)` (recommended, fully
Arduino compatible)

`attachInterrupt(interrupt, ISR, mode)` (not recommended, only for Arduino
compatibility)

`attachInterrupt(pin, ISR, mode)` (Not recommended, even if this call
matches the actual implementation. Apart from SDuino this syntax only
works on Arduino SAMD Boards, Uno WiFi Rev2, Due, and 101.)


## Parameters

`interrupt`: For Arduino compatibility only. The interrupt number
definitions are identical with the pin numbers.

`pin`: The Arduino pin number (or a generic pin specifier like `PB2`)

`ISR`: the ISR to call when the interrupt occurs; this function must take no
parameters and return nothing. This function is sometimes referred to as an
interrupt service routine.

`mode`: defines when the interrupt should be triggered. Four constants are
predefined as valid values:

  - **LOW**: Falling edge and low level (Arduino: low level only)
  - **CHANGE**: Rising and falling edge
  - **RISING**: Rising edge
  - **FALLING**: Falling edge



## Returns

Nothing


## Example Code

```
// the pin where the input button is attached. Change, if needed
#define BUTTON	PA2

// volatile is important, because this variable is modified in IRQ routine
volatile uint8_t flag = 0;


void on_button_pressed(void)
{
	flag = 1;
}


void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, 1);	// turn off the LED

	pinMode(BUTTON, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(BUTTON), on_button_pressed, FALLING);
}


void loop()
{
	if (flag) {
		digitalWrite(LED_BUILTIN, 0);
		delay(300);
		digitalWrite(LED_BUILTIN, 1);
		flag = 0;
	}
}
```



## Interrupt Numbers

Normally you should use `digitalPinToInterrupt(pin)`, rather than place an
interrupt/pin number directly into your sketch. For SDuino the interrupt
numbers are identical with the pin numbers and `digitalPinToInterrupt(pin)`
is just an empty preprecessor macro.

The only thing to loose is some typing efficiency when writing the sketch,
but it preserves some more Arduino compatibility.


## Sleep mode

If enabled, these interrupts wake the CPU from all sleep modes, including
halt mode.



## References

Parts of this text are copied from the [Arduino
Reference](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

The Arduino Reference text and this text is licensed under a [Creative
Commons Attribution-Share Alike 3.0 License](https://creativecommons.org/licenses/by-sa/3.0/).
