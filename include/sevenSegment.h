#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <Arduino.h>

#define SEVEN_SEGMENT_TEXT_LENGTH 5

enum struct SevenSegmentError
{
    OK      = -1,
    GENERIC = 0,
    NO_TEMP_HUM = 1,
};

class SevenSegment
{
private:
    uint8_t _displayData[4]  = {0};
    bool _colonFlag          = false;
    int _commaFlag           = -1;
    SevenSegmentError _error = SevenSegmentError::OK;

public:
    SevenSegment();
    void initialize();
    void identifyLed();
    void identifyDisplay();
    bool displayTemp(float temp);
    bool displayHum(float hum);
    bool displayOff(void);
    bool displayText(char* text);
    void displayError(SevenSegmentError error);
};
#endif /* SEVEN_SEGMENT_H */
