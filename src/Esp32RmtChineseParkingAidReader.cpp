#include "Esp32RmtChineseParkingAidReader.h"

void Esp32RmtChineseParkingAidReader::BeforeProcessSignal()
{
    _dataCounter = 0;
    _bitCounter = 0;
    _bitPart1 = 0;
    _bitPart2 = 0;
}

void Esp32RmtChineseParkingAidReader::AfterProcessSignal()
{
}

/*
IFS: inter-frame sequence
The protocol has an inter frame sequence of 226 ms.
It is followed by a header which contains an 1.9 ms of high signal followed by an 1.0 ms of low signal which is followed by a 104 us of high signal
Zero bit is described as a 205 us low signal followed by a 104 us of high
One  bit is described as a 104 us low signal followed by a 205 us of high

<     IFS     ><                   Header                    ><        0           ><          1         >
              ┌─────────────────────┐                 ┌──────┐              ┌──────┐      ┌──────────────┐
              │                     │                 │      │              │      │      │              │
              │                     │                 │      │              │      │      │              │
    226 ms    │       1.9 ms        │      1.0 ms     │104 us│    205 us    │104 us│104 us│    205 us    │
              │                     │                 │      │              │      │      │              │
              │                     │                 │      │              │      │      │              │
              │                     │                 │      │              │      │      │              │
──────────────┘                     └─────────────────┘      └──────────────┘      └──────┘              └────────
*/
void Esp32RmtChineseParkingAidReader::ProcessSignal(uint32_t level, uint32_t duration)
{
    ///*
    if (_dataCounter > 2)
    {
        if (_dataCounter % 2 == 0)
        {
            _bitPart2 = duration;

            if (_bitPart1 < _bitPart2)
            {
                _message[_messageLength] |= (1 << (7-_bitCounter));
            }

            if (_bitCounter == 7)
            {
                _bitCounter = 0;
                _messageLength = _messageLength + 1;
                _message[_messageLength] = 0;
            }
            else
            {
                _bitCounter++;
            }
        }
        else
        {
            _bitPart1 = duration;
        }
    }
    _dataCounter++;
    //*/
}