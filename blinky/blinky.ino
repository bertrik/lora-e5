#include <Arduino.h>

void setup(void)
{
    pinMode(PB5, OUTPUT);
}

void loop(void)
{
    digitalWrite(PB5, (millis() / 500) % 2);
}
