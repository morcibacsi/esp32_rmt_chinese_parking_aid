// Esp32RmtReader.h
#pragma once

#ifndef _Esp32RmtReader_h
    #define _Esp32RmtReader_h

#include <stdint.h>
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

class Esp32RmtReader
{
    #define RMT_READER_MAX_DATA 255
    protected:
        uint16_t _idleTreshold;
        uint8_t _clkDiv;
        uint8_t _channel;
        uint8_t _rxPin;
        int8_t _ledPin;
        uint8_t _messageLength;
        uint8_t _message[RMT_READER_MAX_DATA];

        Esp32RmtReader(uint8_t clkDiv, uint16_t idleTreshold, uint8_t channel, uint8_t rxPin, int8_t ledPin);
        void InitLed();
        void SwitchLed(uint8_t state);
        virtual void BeforeProcessSignal() = 0;
        virtual void AfterProcessSignal() = 0;
        virtual void ProcessSignal(uint32_t level, uint32_t duration) = 0;
    public:
        int32_t Start();
        void Stop();
        void ReceiveData(uint8_t *messageLength, uint8_t message[]);
};

#endif
