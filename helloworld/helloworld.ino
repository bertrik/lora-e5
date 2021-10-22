#include <Arduino.h>

#include <stdbool.h>
#include <stdint.h>

#include "stm32wlxx_hal_subghz.h"

static SUBGHZ_HandleTypeDef handle = {.Init = {.BaudratePrescaler =
                                               SUBGHZSPI_BAUDRATEPRESCALER_64 } };

void HAL_SUBGHZ_MspInit(SUBGHZ_HandleTypeDef * hsubghz)
{
    Serial.println("HAL_SUBGHZ_MspInit");

    // enable clock to sub-ghz module
    __HAL_RCC_SUBGHZSPI_CLK_ENABLE();
}

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);

    pinMode(USER_BTN, INPUT_PULLUP);

    Serial.begin(9600);

    HAL_StatusTypeDef result = HAL_SUBGHZ_Init(&handle);
    Serial.println(result);
}

static unsigned long int last_second = 0;
static unsigned long int last_button_change = 0;
static bool button_status = false;
static bool button_event = true;
static bool last_button_status = false;

void loop(void)
{
    // send out a message once a second
    unsigned long int ms = millis();
    unsigned long int second = ms / 1000;
    if (second != last_second) {
        Serial.print("Second ");
        Serial.println(second);
        last_second = second;
    }
    // debounce button
    bool button_event = false;
    if ((ms - last_button_change) > 100) {
        button_status = (digitalRead(USER_BTN) == 0);
        if (button_status != last_button_status) {
            button_event = true;
            last_button_status = button_status;
            last_button_change = ms;
        }
    }
    if (button_event) {
        Serial.print("Button: ");
        Serial.println(button_status);

        // on button press, read out the sync word registers (nominally 0x14, 0x24) as proof-of-communication
        if (button_status) {
            uint8_t buf[2];
            HAL_StatusTypeDef result = HAL_SUBGHZ_ReadRegisters(&handle, 0x740, buf, sizeof(buf));
            Serial.println(result);
            Serial.print(buf[0], HEX);
            Serial.print(" ");
            Serial.println(buf[1], HEX);
        }
    }
    // flash LED to show we're still alive
    digitalWrite(LED_BUILTIN, (ms / 500) % 2);
}
