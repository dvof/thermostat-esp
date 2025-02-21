#include <Arduino.h>
#include "WemosD1MiniPins.h"
#include <SPI.h>

#define MOSI    D7
#define SCK     D5
#define RCK     D8

const uint8_t digitCodes[10] = {
    0b11101011,
    0b00101000,
    0b10110011,
    0b10111010,
    0b01111000,
    0b11011010,
    0b11011011,
    0b10101000,
    0b11111011,
    0b11111010
};

static void update()
{
    digitalWrite(RCK, LOW);
    digitalWrite(RCK, HIGH);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Serial.println("Setup device");

    pinMode(RCK, OUTPUT);

    SPI.begin();
    SPI.setFrequency(1000000);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(LSBFIRST);

    SPI.transfer(digitCodes[2]);
    update();
}

void loop()
{
    for (int i=0; i < 10; i++)
    {
        SPI.transfer(digitCodes[i]);
        update();
        delay(2000);
    }
}


