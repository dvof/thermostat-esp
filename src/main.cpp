#include <Arduino.h>
#include "sevenSegment.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Serial.println("Setup device");

    pinMode(D3, INPUT_PULLUP);
    pinMode(D4, INPUT_PULLUP);
}

void loop() {}
