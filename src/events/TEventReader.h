// TCode-Event-Reader-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../datatypes/CommandDataTypes.h"
#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"
#include <Arduino.h>
#include <deque>

namespace TCode::Events {

    class TCodeEventReader {
    private:
        static const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 64;
        std::deque<char> inputBuffer;
        std::deque<Datatypes::TCodeEvent> eventBuffer;

        bool parseCommand(const char *buffer, const size_t length);
        size_t consumeNextCommandFromInputBuffer(char *buffer, const size_t length);

    public:
        void read(const byte input);
        void read(const char input);
        void read(const String &input);
        void read(const char *input);

        bool parseNext();
        bool parseAll();
        bool getNext(Datatypes::TCodeEvent &event);
        void flush();

        TCodeEventReader();
        ~TCodeEventReader();
    };

}