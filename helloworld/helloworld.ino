#include <Arduino.h>

#include <SoftwareSerial.h>

/**
 * Ideally, the serial port used by Arduino is the one on PB6/PB7 under hardware control.
 * For this, we need to configure the alternate function for PB6/PB7 to alternate function 7,
 * using the GPIOB.AFRL register. Possibly, we also need to tell Arduino to use the right UART.
 *
 * For now, using software serial works too.
 */
static SoftwareSerial serial(PB7, PB6);

void setup(void)
{
    pinMode(PB5, OUTPUT);
    digitalWrite(PB5, 0);

    serial.begin(9600);
    serial.println("Hello world!");
}

static unsigned long int last_second = 0;

void loop(void)
{
    // send out a message once a second
    unsigned long int ms = millis();
    unsigned long int second = ms / 1000;
    if (second != last_second) {
        serial.print("Second ");
        serial.println(second);
        last_second = second;
    }

    // flash LED to show we're still alive
    digitalWrite(PB5, (ms / 500) % 2);
}

