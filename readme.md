# Chinese parking aid decoder with ESP32 RMT peripheral for the Arduino framework

## Description

This is a ESP32 RMT peripheral based decoder for this specific Chinese parking aid:

![device](https://github.com/morcibacsi/esp32_rmt_chinese_parking_aid/raw/master/extras/chinese_car_parking_aid.webp)

It has four sensors and a small display for showing the distance. The accuracy of the parking aid is 10 cm. The maximum distance it can measure is 240 cm and the minimum is 30 cm, below that it sends 0 cm. 

The internals looks like this:
![internals](https://github.com/morcibacsi/esp32_rmt_chinese_parking_aid/raw/master/extras/chinese_parking_aid_internal.jpg)

## Protocol

It uses a simple protocol which uses a single wire (marked as SIGNAL, with yellow color on the picture above). 
There is an inter-frame sequence which is a 226ms long low signal. If is followed by a header which consist of a 1.9 ms high and 1.0 ms low signal. A zero bit is represented by 205 µs low signal followed by a 104 µs of high signal. A one bit is represented by 104 µs low signal followed by a 205 µs high signal.

    <     IFS    ><                   Header                     ><        0           ><          1         >
                  +---------------------+                 +------+              +------+      +--------------+
                  ¦                     ¦                 ¦      ¦              ¦      ¦      ¦              ¦
                  ¦                     ¦                 ¦      ¦              ¦      ¦      ¦              ¦
        226 ms    ¦       1.9 ms        ¦      1.0 ms     ¦104 µs¦    205 µs    ¦104 µs¦104 µs¦    205 µs    ¦
                  ¦                     ¦                 ¦      ¦              ¦      ¦      ¦              ¦
                  ¦                     ¦                 ¦      ¦              ¦      ¦      ¦              ¦
                  ¦                     ¦                 ¦      ¦              ¦      ¦      ¦              ¦
    --------------+                     +-----------------+      +--------------+      +------+              +--------
    
![protocol](https://github.com/morcibacsi/esp32_rmt_chinese_parking_aid/raw/master/extras/chinese_parking_aid_protocol.png)

The header is followed by the readings from the sensors in the following order:  A D C B. Where A is the sensor on the left and B is the sensor on the right of the car (D and C are the two in the middle, yes it is weird). See the product manual below. The data it sends is divided by 10. So when it sends 4 the distance is 40 cm.

![product manual](https://github.com/morcibacsi/esp32_rmt_chinese_parking_aid/raw/master/extras/chinese_parking_aid_manual.jpg)

## Schematics

I connected pin 21 of the ESP32 to the display on the SIGNAL wire marked on the picture above (and of course grounds should be also connected)

![schema](https://github.com/morcibacsi/esp32_rmt_chinese_parking_aid/raw/master/extras/chinese_parking_aid_esp32.jpg

## Example

```cpp
#include <Arduino.h>
#include "Esp32RmtChineseParkingAidReader.h"
#include "Esp32RmtRawReader.h"

TaskHandle_t ReadParkingDataTask;

void ReadParkingDataTaskFunction(void* parameter)
{
    uint8_t parkRadarMessageLength;
    uint8_t parkRadarMessage[4];

    Esp32RmtReader *parkingAidReader;
    parkingAidReader = new Esp32RmtChineseParkingAidReader(0, 21, 2);
    parkingAidReader->Start();

    while (1)
    {
        parkingAidReader->ReceiveData(&parkRadarMessageLength, parkRadarMessage);

        //sensor order: A D C B
        if (parkRadarMessageLength == 4)
        {
            Serial.print(parkRadarMessage[0]);//A
            Serial.print(" ");

            Serial.print(parkRadarMessage[3]);//B
            Serial.print(" ");

            Serial.print(parkRadarMessage[2]);//C
            Serial.print(" ");

            Serial.print(parkRadarMessage[1]);//D
            Serial.print(" ");

            Serial.println();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(500000);
    printf("\nChinese parking aid reader\n");

    xTaskCreatePinnedToCore(
        ReadParkingDataTaskFunction,     // Function to implement the task
        "ReadParkingDataTask",          // Name of the task
        20000,                          // Stack size in words
        NULL,                           // Task input parameter
        1,                              // Priority of the task (higher the number, higher the priority)
        &ReadParkingDataTask,           // Task handle.
        0);                             // Core where the task should run
}

void loop()
{
    vTaskDelay(50 / portTICK_PERIOD_MS);
}

```

## Sample output

![output](https://github.com/morcibacsi/esp32_rmt_chinese_parking_aid/raw/master/extras/chinese_parking_aid_arduino_output.jpg)

## Installation

You need to install the official STM32 core based on the info from here: 

Copy the following files to your **documents\Arduino\libraries\esp32_rmt_chinese_parking_aid** folder

- Esp32RmtChineseParkingAidReader.cpp
- Esp32RmtChineseParkingAidReader.h
- Esp32RmtReader.cpp
- Esp32RmtReader.h
- keywords.txt
- library.properties

## ESP8266 Support

**ESP8266 is not supported as it doesn't have the RMT peripheral which is required for this library to work**