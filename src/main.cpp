#include <Arduino.h>
#include "WemosD1MiniPins.h"
#include <SPI.h>

#define MOSI D7
#define SCK D5
#define RCK D8

//    A
//  ┌─────┐
// F│  G  │B
//  ├─────┤
// E│     │C
//  └─────┘
//     D   DP
#define SEG_A (uint8_t)(1 << 7)
#define SEG_B (uint8_t)(1 << 6)
#define SEG_C (uint8_t)(1 << 3)
#define SEG_D (uint8_t)(1 << 1)
#define SEG_E (uint8_t)(1 << 0)
#define SEG_F (uint8_t)(1 << 5)
#define SEG_G (uint8_t)(1 << 4)
#define SEG_DP (uint8_t)(1 << 2)

#define DIG_1 (uint8_t)(1 << 2)
#define DIG_2 (uint8_t)(1 << 5)
#define DIG_3 (uint8_t)(1 << 3)
#define DIG_4 (uint8_t)(1 << 6)
#define COLON (uint8_t)(1 << 1)

const uint8_t digitCodes[10] = {
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),         // 0
    (SEG_B | SEG_C),                                         // 1
    (SEG_A | SEG_B | SEG_G | SEG_E | SEG_D),                 // 2
    (SEG_A | SEG_B | SEG_G | SEG_C | SEG_D),                 // 3
    (SEG_F | SEG_G | SEG_B | SEG_C),                         // 4
    (SEG_A | SEG_F | SEG_G | SEG_C | SEG_D),                 // 5
    (SEG_A | SEG_F | SEG_G | SEG_E | SEG_D | SEG_C),         // 6
    (SEG_A | SEG_B | SEG_C),                                 // 7
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), // 8
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)          // 9
};

static void update()
{
    digitalWrite(RCK, LOW);
    digitalWrite(RCK, HIGH);
}

static void identifySevenSegmentPart()
{
    for (int i = 0; i < 8; i++)
    {
        SPI.transfer(1 << i);
        update();
        delay(2000);
    }
}

static void identifySevenSegmentNumber()
{
    for (int i = 0; i < 8; ++i)
    {
        SPI.transfer(digitCodes[8]);
        SPI.transfer(1 << i);
        update();
        delay(2000);
    }
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
    pinMode(D3, INPUT_PULLUP);

    SPI.begin();
    SPI.setFrequency(1000000);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(LSBFIRST);

    SPI.transfer(digitCodes[2]);
    SPI.transfer(DIG_4 );
    update();
}

void loop()
{
    static int button_increment = 0;
    int button                  = digitalRead(D3);
    Serial.print("Button: ");
    Serial.println(button);
    if (button == LOW)
    {
        button_increment++;
        SPI.transfer(digitCodes[button_increment % 10]);
        SPI.transfer(DIG_4 );
         update();
         delay(40);
    }
    // for (int i = 0; i < 9999; i++)
    //{
    //     uint32_t startTime    = millis();
    //     uint32_t durationTime = 10;
    //     while (millis() - startTime < durationTime)
    //     {
    //         // digit 1
    //         SPI.transfer(digitCodes[(i / 1000) % 10]);
    //         SPI.transfer(DIG_1 | COLON);
    //         update();
    //         delay(1);
    //         // digit 2
    //         SPI.transfer(digitCodes[(i / 100) % 10]);
    //         SPI.transfer(DIG_2 | COLON);
    //         update();
    //         delay(1);
    //         // digit 3
    //         SPI.transfer(digitCodes[(i / 10) % 10]);
    //         SPI.transfer(DIG_3 | COLON);
    //         update();
    //         delay(1);
    //         // digit 4
    //         SPI.transfer(digitCodes[i % 10]);
    //         SPI.transfer(DIG_4 | COLON);
    //         update();
    //         delay(1);
    //     }
    // }
    //  identifySevenSegment();
    //  identifySevenSegmentNumber();
}
