#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

enum class SevenSegmentCase{
    DONT_CARE,
    UPPER,
    LOWER,
};

class SevenSegment
{
private:
    void spiUpdate();

public:
    void initialize();
    void identifyLed();
    void identifyDisplay();
    void displayNumber(uint16_t number, bool colon = false); 
    void displayText(char* text, SevenSegmentCase case = SevenSegmentCase::DONT_CARE);
}
#endif /* SEVEN_SEGMENT_H */
