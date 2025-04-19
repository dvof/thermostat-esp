#include <Arduino.h>
#include "sevenSegment.h"
#include <DHT.h>
#include "CircularBuffer.h"

SevenSegment sevenSegment;
DHT dht(D2, DHT11);
#define DHT11_AVERAGE_SAMPLE_SIZE 5
#define DHT11_AVERAGING 1 / DHT11_AVERAGE_SAMPLE_SIZE
#define DHT11_READ_TIME 2000
CircularBuffer<float, DHT11_AVERAGE_SAMPLE_SIZE> humidityBuffer;
CircularBuffer<float, DHT11_AVERAGE_SAMPLE_SIZE> temperatureBuffer;
float averageHumidity      = 0;
float averageTemperature   = 0;
uint32_t timeLastReadDHT11 = millis();

void readDHT11(void)
{
    if (millis() - timeLastReadDHT11 > DHT11_READ_TIME)
    {
        timeLastReadDHT11 = millis();
        humidityBuffer.push(dht.readHumidity());
        temperatureBuffer.push(dht.readTemperature()); // Celsius by default

        float first = 0, last = 0;
        temperatureBuffer.get(0, first);
        temperatureBuffer.get(DHT11_AVERAGE_SAMPLE_SIZE, last);
        averageTemperature += (first - last) * DHT11_AVERAGING;

        humidityBuffer.get(0, first);
        humidityBuffer.get(DHT11_AVERAGE_SAMPLE_SIZE, last);
        averageHumidity += (first - last) * DHT11_AVERAGING;
    }
}

float getAverageTemperature(void) { return averageTemperature; }

float getAverageHumidity(void) { return averageHumidity; }

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

    dht.begin();
    for (size_t i = 0; i < DHT11_AVERAGE_SAMPLE_SIZE; i++)
    {
        humidityBuffer.push(dht.readHumidity());
        humidityBuffer.push(dht.readTemperature());
    }
    float newest = 0;
    humidityBuffer.get(0, newest);
    averageHumidity = newest;
    temperatureBuffer.get(0, newest);
    averageTemperature = newest;

    //    sevenSegment.initialize();
    //    sevenSegment.start();
}

static float test = 5.4;

enum struct MenuState
{
    OFF,
    TEMPERATURE,
    HUMIDITY,
    WIFI,
};

MenuState displayState = MenuState::TEMPERATURE;

void loop()
{

    int buttonLeft  = digitalRead(D4);
    int buttonRight = digitalRead(D3);
    if (buttonRight == LOW)
    {
        test = 0;
    }
    if (buttonLeft == LOW && buttonRight == LOW)
    {
        //        sevenSegment.displayError(SevenSegmentError::OK);
    }
    test += 0.1;
    //    sevenSegment.displayTemp(test);
    delay(100);
    if (test > 10)
    {
        //        sevenSegment.displayError(SevenSegmentError::GENERIC);
    }

    float humidity    = getAverageHumidity();
    float temperature = getAverageTemperature(); // Celsius by default

    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println("Failed to read from DHT sensor!");
        sevenSegment.displayError(SevenSegmentError::NO_TEMP_HUM);
        return;
    }
    Serial.printf("Temp: %.1f°C  |  Humidity: %.1f%%\n", temperature, humidity);

    switch (displayState)
    {
    case MenuState::OFF:
        sevenSegment.displayOff();
    case MenuState::TEMPERATURE:
        sevenSegment.displayTemp(temperature);
        break;
    case MenuState::HUMIDITY:
        sevenSegment.displayHum(humidity);
        break;
    case MenuState::WIFI:
        break;
    }
}
