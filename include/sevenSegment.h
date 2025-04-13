#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

 #include <Arduino.h>

#define SEVEN_SEGMENT_TEXT_LENGTH 5

enum class SevenSegmentCase
{
    DONT_CARE,
    UPPER,
    LOWER,
};

class SevenSegment
{
private:
    uint8_t _displayData[4] = {0};
    bool _colonFlag = false;
    int _commaFlag         = -1;
    void _spiUpdate();

public:
    SevenSegment();
    void initialize();
    void identifyLed();
    void identifyDisplay();
    bool displayNumber(float number);
    bool displayText(char* text, SevenSegmentCase segmentCase = SevenSegmentCase::DONT_CARE);
    void update();
};
#endif /* SEVEN_SEGMENT_H */
