/*
 * attachInterrupt.ino
 *
 * Simple test and demonstration of the pin change interrupt using
 * the attachInterupt() function.
 *
 * The builtin LED is briefly lit on every button press.
 *
 * The button press is detected by an interrupt routine and the main loop()
 * turns the LED on and off.
 * Debouncing is done by simply waiting for a few hundred milliseconds after
 * each button press.
 *
 * required hardware:
 * - Connect a button from PA2 to GND.
 *
 * When using other pins for the button, please keep in mind that there is
 * no pin change interrupt support on PA0, PA1, PA7 and PD7. (for stm8blue:
 * Arduino pin D0 (PA1) is the only one without interrupt support)
 *
 * written for sduino, MIT licence
 *
 * (c) 2020 by Michael Mayer
 */

// the pin where the input button is attached. Change, if needed
#define BUTTON	PA2

// volatile is important, because this variable is modified in IRQ routine
volatile uint8_t flag = 0;


/*
 * IRQ routine
 *
 * Triggered on every falling edge. It sets a flag that can be detected by
 * the main loop.
 *
 * no debouncing is done here. Expect multiple trigger for every button press.
 */
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
