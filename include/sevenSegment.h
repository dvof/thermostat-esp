#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <Arduino.h>
#include <TaskScheduler.h>

#define SEVEN_SEGMENT_TEXT_LENGTH 4

enum class SevenSegmentCase
{
    DONT_CARE,
    UPPER,
    LOWER,
};

class SevenSegment
{
private:
    float _displayNumber                         = 0.0;
    char _displayText[SEVEN_SEGMENT_TEXT_LENGTH] = "  hi";
    bool _displayNumberFlag                      = false;
    bool _displayTextFlag                        = false;
    bool _displayColonFlag                       = false;
    Scheduler _scheduler;
    void displayNumberCallback();
    void displayTextCallback();
    Task _displayNumberTask(TASK_IMMEDIATE, TASK_FOREVER, &this->displayNumberCallback);
    Task _displayTextTask(TASK_IMMEDIATE, TASK_FOREVER, &this->displayTextCallback);
    void spiUpdate();

public:
    void initialize();
    void identifyLed();
    void identifyDisplay();
    bool displayNumber(float number, bool colon = false);
    bool displayText(char* text, SevenSegmentCase segmentCase = SevenSegmentCase::DONT_CARE);
};
#endif /* SEVEN_SEGMENT_H */
