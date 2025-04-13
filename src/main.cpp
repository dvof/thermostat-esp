#include <Arduino.h>
#include <TaskScheduler.h>
#include "sevenSegment.h"

SevenSegment sevenSegment;
Scheduler scheduler;

void displayCallbackTask() { sevenSegment.update(); }

Task displayTask(TASK_IMMEDIATE, TASK_FOREVER, &displayCallbackTask, &scheduler, true);

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
