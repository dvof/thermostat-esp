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

enum struct DigitSelect : uint8_t
{
    DIGIT1 = (1 << 2),
    DIGIT2 = (1 << 5),
    DIGIT3 = (1 << 3),
    DIGIT4 = (1 << 6),
    COLON  = (1 << 1),
};

typedef uint8_t DigitCode;

static const DigitCode _digitCodes[10] = {
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

static const DigitCode _digitCodeLowerC = (SEG_G | SEG_E | SEG_D);
static const DigitCode _digitCodeUpperC = (SEG_A | SEG_F | SEG_D | SEG_E);
static const DigitCode _digitCodeUpperE = (SEG_A | SEG_F | SEG_G | SEG_E | SEG_D);
static const DigitCode _digitCodeLowerR = (SEG_E | SEG_G);
static const DigitCode _digitCodeOff    = 0;

struct Digit
{
    const DigitSelect select;
    DigitCode code;
};

static volatile Digit _digits[4] = {
    {DigitSelect::DIGIT1, _digitCodes[1]},
    {DigitSelect::DIGIT2, _digitCodes[2]},
    {DigitSelect::DIGIT3, _digitCodes[3]},
    {DigitSelect::DIGIT4, _digitCodes[5]},
};
static volatile uint8_t _digitIndex = 0;

static void spiUpdate()
{
    digitalWrite(RCK, LOW);
    digitalWrite(RCK, HIGH);
}

void IRAM_ATTR onTimerISR()
{
    if (_digitIndex == 4)
    {
        _digitIndex = 0;
    }
    SPI.transfer((uint8_t)_digits[_digitIndex].code);
    SPI.transfer((uint8_t)_digits[_digitIndex].select);
    spiUpdate();
    _digitIndex++;
}

SevenSegment::SevenSegment()
{
    pinMode(RCK, OUTPUT);
    SPI.begin();
    SPI.setFrequency(1000000);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(LSBFIRST);
    timer1_attachInterrupt(onTimerISR);           // Attach ISR
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP); // Loop every time
    timer1_write(5000);
}

void SevenSegment::identifyLed()
{
    for (int i = 0; i < 8; i++)
    {
        //        SPI.transfer(1 << i);
        //        this->_spiUpdate();
        //        delay(2000);
    }
}

void SevenSegment::identifyDisplay()
{
    for (int i = 0; i < 8; ++i)
    {
        //        SPI.transfer(digitCodes[8]);
        //        SPI.transfer(1 << i);
        //        this->_spiUpdate();
        //        delay(2000);
    }
}

void SevenSegment::displayError(SevenSegmentError error)
{
    this->_error = error;

    _digits[0].code = _digitCodeUpperE;
    _digits[1].code = _digitCodeLowerR;
    _digits[2].code = _digitCodeLowerR;
    if (error == SevenSegmentError::GENERIC)
    {
        _digits[3].code = _digitCodeOff;
    }
    else
    {
        _digits[3].code = _digitCodes[(int)error];
    }
}

bool SevenSegment::displayTemp(float temp)
{
    if (this->_error != SevenSegmentError::OK)
    {
        return false;
    }

    uint8_t firstDigit = _digitCodes[int(temp / 10) % 10];
    if (firstDigit == _digitCodes[0])
    {
        _digits[0].code = _digitCodeOff;
    }
    else
    {
        _digits[0].code = firstDigit;
    }
    _digits[1].code = _digitCodes[int(temp) % 10] | SEG_DP;
    _digits[2].code = _digitCodes[int(temp * 10) % 10];
    _digits[3].code = _digitCodeUpperC;
    return true;
}

bool SevenSegment::displayText(char* text)
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
