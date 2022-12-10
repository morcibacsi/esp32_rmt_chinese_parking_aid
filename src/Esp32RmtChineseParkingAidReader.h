//Esp32RmtChineseParkingAidReader.h
#pragma once

#ifndef _Esp32RmtChineseParkingAidReader_h
    #define _Esp32RmtChineseParkingAidReader_h

#include <stdint.h>
#include "Esp32RmtReader.h"

class Esp32RmtChineseParkingAidReader : public Esp32RmtReader
{
    protected:
        uint8_t _dataCounter;
        uint8_t _bitCounter;
        uint32_t _bitPart1;
        uint32_t _bitPart2;

        void BeforeProcessSignal() override;
        void AfterProcessSignal() override;
        void ProcessSignal(uint32_t level, uint32_t duration) override;
    public:
        Esp32RmtChineseParkingAidReader(uint8_t channel, uint8_t rxPin, int8_t ledPin) : Esp32RmtReader(80, 0xFFFF, channel, rxPin, ledPin)
        {
            _dataCounter = 0;
            _bitCounter = 0;
            _bitPart1 = 0;
            _bitPart2 = 0;
        }
};

#endif
