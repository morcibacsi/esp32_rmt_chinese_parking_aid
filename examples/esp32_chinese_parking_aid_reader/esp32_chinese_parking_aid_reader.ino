#include <Arduino.h>
#include "Esp32RmtChineseParkingAidReader.h"
#include "Esp32RmtReader.h"

TaskHandle_t ReadParkingDataTask;

void ReadParkingDataTaskFunction(void* parameter)
{
    uint8_t parkingAidMessageLength;
    uint8_t parkingAidMessage[4];

    Esp32RmtReader *parkingAidReader;
    parkingAidReader = new Esp32RmtChineseParkingAidReader(0, 21, 2);
    parkingAidReader->Start();

    while (1)
    {
        parkingAidReader->ReceiveData(&parkingAidMessageLength, parkingAidMessage);

        //A D C B
        if (parkingAidMessageLength == 4)
        {
            Serial.print(parkingAidMessage[0]);
            Serial.print(" ");

            Serial.print(parkingAidMessage[3]);
            Serial.print(" ");

            Serial.print(parkingAidMessage[2]);
            Serial.print(" ");

            Serial.print(parkingAidMessage[1]);
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
