#include <Arduino.h>
#include "WemosD1MiniPins.h"

#define MOSI    D7
#define CLK     D5
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

static void write(byte b)
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(MOSI, b & 1);
        b >>= 1;
        digitalWrite(CLK, LOW);
        digitalWrite(CLK, HIGH);
    }
}

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

    pinMode(MOSI, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(RCK, OUTPUT);
    digitalWrite(MOSI, LOW);
    digitalWrite(CLK, HIGH);
    write(digitCodes[2]);
    update();
}

void loop()
{
    for (int i=0; i < 10; i++)
    {
        write(digitCodes[i]);
        update();
        delay(2000);
    }
}


