// TCode-Event-Reader-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include <Arduino.h>
#include <deque>
#include "../../datatypes/CommandDataTypes.h"
#include "../../datatypes/CommonDataTypes.h"
#include "../../datatypes/EnumTypes.h"

namespace TCode::TEvents
{
        
    class TCodeEventReader
    {
    private:
        static const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 512; 
        std::deque<char> inputBuffer;
        std::deque<TCodeEvent> eventBuffer;

        bool parseCommand(const char *buffer, const size_t length);
        size_t consumeNextCommandFromInputBuffer(char *buffer, const size_t length);
    public:
        void read(const byte input);
        void read(const char input);
        void read(const String &input);
        void read(const char *input);

        bool parse();
        bool getNext(TCodeEvent& event);
        void flush();

        TCodeEventReader();
        ~TCodeEventReader();
    };

}