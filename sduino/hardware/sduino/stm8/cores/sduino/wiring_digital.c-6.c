#include "wiring_digital.c.h"
/**
 * set an output value for a pin
 */
void digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;

	if (port == NOT_A_PIN) return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	out = portOutputRegister(port);

	BEGIN_CRITICAL

	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}

	END_CRITICAL
}

