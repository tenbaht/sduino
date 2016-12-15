#include "Arduino.h"

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
}


void loop()
{
    u16 i;

    digitalWrite(LED_BUILTIN, HIGH);

    for(i = 0; i < 30000; i++);
//    delay(100);

    digitalWrite(LED_BUILTIN, LOW);

    for(i = 0; i < 30000; i++);
}
