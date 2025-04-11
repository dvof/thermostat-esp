#include "sevenSegment.h"
#include "wemosD1MiniPins.h"
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

void SevenSegment::_staticDisplayNumberCallback()
{
    if (_instance)
    {
        _instance->_displayNumberCallback();
    }
}
void SevenSegment::_staticDisplayTextCallback()
{
    if (_instance)
    {
        _instance->_displayTextCallback();
    }
}

void SevenSegment::_displayNumberCallback() {}
void SevenSegment::_displayTextCallback() {}

void SevenSegment::initialize()
{
    pinMode(RCK, OUTPUT);
    SPI.begin();
    SPI.setFrequency(1000000);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(LSBFIRST);

    _displayNumberTask.set(TASK_IMMEDIATE, TASK_FOREVER, _staticDisplayNumberCallback);
    _displayTextTask.set(TASK_IMMEDIATE, TASK_FOREVER, _staticDisplayTextCallback);

    this->_scheduler.init();
    this->_scheduler.addTask(_displayNumberTask);
    this->_scheduler.addTask(_displayTextTask);
}

void SevenSegment::_spiUpdate()
{
    digitalWrite(RCK, LOW);
    digitalWrite(RCK, HIGH);
}

void SevenSegment::identifyLed()
{
    for (int i = 0; i < 8; i++)
    {
        SPI.transfer(1 << i);
        this->_spiUpdate();
        delay(2000);
    }
}

void SevenSegment::identifyDisplay()
{
    for (int i = 0; i < 8; ++i)
    {
        SPI.transfer(digitCodes[8]);
        SPI.transfer(1 << i);
        this->_spiUpdate();
        delay(2000);
    }
}

bool SevenSegment::displayNumber(float number, bool colon)
{
    this->_displayNumber     = number;
    this->_displayNumberFlag = true;
    this->_displayColonFlag  = colon;
    this->_displayTextTask.disable();
    this->_displayNumberTask.enable();
    return true;
}
bool SevenSegment::displayText(char* text, SevenSegmentCase segmentCase)
{
    if (text == NULL)
    {
        return false;
    }
    // this->_displayText       = number;
    // this->_displayNumberFlag = true;
    // this->_displayColonFlag  = colon;
    // this->_displayTextTask.disable();
    // this->_displayNumberTask.enable();
    return true;
}

// static int button_increment = 0;
// int buttonLeft              = digitalRead(D4);
// int buttonRight             = digitalRead(D3);
// static uint8_t digit        = DIG_4;
// if (buttonRight == LOW)
//{
//     if (digit == DIG_4)
//     {
//         digit = DIG_3;
//     }
//     else if (digit == DIG_3)
//     {
//         digit = DIG_2;
//     }
//     else if (digit == DIG_2)
//     {
//         digit = DIG_1;
//     }
//     else
//     {
//         digit = DIG_4;
//     }
//
//     delay(400);
// }
// if (buttonLeft == LOW || buttonRight == LOW)
//{
//     if (buttonLeft == LOW)
//     {
//         button_increment++;
//     }
//     SPI.transfer(digitCodes[button_increment % 10]);
//     SPI.transfer(digit);
//     spiUpdate();
//     delay(40);
// }
//
//  for (int i = 0; i < 9999; i++)
//{
//      uint32_t startTime    = millis();
//      uint32_t durationTime = 10;
//      while (millis() - startTime < durationTime)
//      {
//          // digit 1
//          SPI.transfer(digitCodes[(i / 1000) % 10]);
//          SPI.transfer(DIG_1 | COLON);
//          spiUpdate();
//          delay(1);
//          // digit 2
//          SPI.transfer(digitCodes[(i / 100) % 10]);
//          SPI.transfer(DIG_2 | COLON);
//          spiUpdate();
//          delay(1);
//          // digit 3
//          SPI.transfer(digitCodes[(i / 10) % 10]);
//          SPI.transfer(DIG_3 | COLON);
//          spiUpdate();
//          delay(1);
//          // digit 4
//          SPI.transfer(digitCodes[i % 10]);
//          SPI.transfer(DIG_4 | COLON);
//          spiUpdate();
//          delay(1);
//      }
//  }
//   identifySevenSegment();
//   identifySevenSegmentNumber();
