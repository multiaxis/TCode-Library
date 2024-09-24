// TCode-Event-Reader-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#include "TEventReader.h"
#include "../utils/TString.h"
#include "../parsing/TParser.h"
#include <Arduino.h>

namespace TCode::Events {
    TCodeEventReader::TCodeEventReader() {
    }

    TCodeEventReader::~TCodeEventReader() {
    }

    void TCodeEventReader::read(const byte input) {
        read((const char)input);
    }

    void TCodeEventReader::read(const String &input) {
        for (int i = 0; i < input.length(); i++)
            read(input.charAt(i));
    }

    void TCodeEventReader::read(const char *input) {
        size_t length = strlen(input);
        for (int i = 0; i < length; i++)
            read(input[i]);
    }

    void TCodeEventReader::read(const char input) {
        static char commandBuffer[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
        if (inputBuffer.size() >= MAX_COMMAND_BUFFER_LENGTH_COUNT) {
            parseNext();
        }
        //LogHandler::error("TEVENT", "got char \"%c\"", input);
        inputBuffer.push_back(input);
        if (input == '\n') {
            parseAll();
        }
    }

    bool TCodeEventReader::parseAll() {
        while (!inputBuffer.empty()) {
            parseNext();
        }
        inputBuffer.clear();
        return true;
    }

    bool TCodeEventReader::parseNext() {
        static char commandBuffer[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
        size_t length = consumeNextCommandFromInputBuffer(commandBuffer, MAX_COMMAND_BUFFER_LENGTH_COUNT);
        if (!parseCommand(commandBuffer, length)) {
            //LogHandler::error("TEVENT", "Parsing Command Buffer:\"%s\" Could not be parsed", commandBuffer);
            return false;
        }
        return true;
    }


    bool TCodeEventReader::getNext(Datatypes::TCodeEvent &event) {
        if (!eventBuffer.empty()) {
            event = eventBuffer.front();
            eventBuffer.pop_front();
            return true;
        }
        return false;
    }

    void TCodeEventReader::flush() {
        eventBuffer.clear();
        inputBuffer.clear();
    }

    size_t TCodeEventReader::consumeNextCommandFromInputBuffer(char *buffer, const size_t length) {
        size_t index = 0;
        bool string_value = false;
        while (!inputBuffer.empty() && index + 1 < length) {
            char c = inputBuffer.front();
            if ((c == ' ' && !string_value) || c == '\n') {
                inputBuffer.pop_front();
                break;
            }

            if(c == '\"')
            {
                string_value = !string_value;
            }

            if(!TString::writeChar(c,index,buffer,length))
            {
                return 0;
            }

            inputBuffer.pop_front();
        }

        TString::writeChar('\0',index,buffer,length);
        return index;
    }

    bool TCodeEventReader::parseCommand(const char *buffer, const size_t length) {
        Datatypes::TCodeEvent event;
        if (TParser::parseCommand(buffer, length, event)) {
            eventBuffer.push_back(event);
            return true;
        }
        return false;
    }

}