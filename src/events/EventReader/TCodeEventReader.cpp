// TCode-Event-Reader-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#include "TCodeEventReader.h"
#include "../../logging/LogHandler.h"
#include "../../utils/TString.h"
#include "../Parsing/TParser.h"
#include <Arduino.h>

namespace TCode::TEvents {
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
            size_t length = consumeNextCommandFromInputBuffer(commandBuffer, MAX_COMMAND_BUFFER_LENGTH_COUNT);
            if (!parseCommand(commandBuffer, length)) {
                LogHandler::error("TEVENT", "Parsing Command Buffer:\"%s\" Could not be parsed", commandBuffer);
            }
        }

        inputBuffer.push_back(input);
        if (input == '\n') {
            while (!inputBuffer.empty()) {
                size_t length = consumeNextCommandFromInputBuffer(commandBuffer, MAX_COMMAND_BUFFER_LENGTH_COUNT);
                if (!parseCommand(commandBuffer, length)) {
                    LogHandler::error("TEVENT", "Parsing Command Buffer:\"%s\" Could not be parsed", commandBuffer);
                }
            }
        }
    }

    bool TCodeEventReader::parse() {
        static char commandBuffer[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
        while (!inputBuffer.empty()) {
            size_t length = consumeNextCommandFromInputBuffer(commandBuffer, MAX_COMMAND_BUFFER_LENGTH_COUNT);
            if (!parseCommand(commandBuffer, length)) {
                LogHandler::error("TEVENT", "Parsing Command Buffer:\"%s\" Could not be parsed", commandBuffer);
            }
        }
        return true;
    }

    bool TCodeEventReader::getNext(TCodeEvent &event) {
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
        while (!inputBuffer.empty() && index < length - 1) {
            char c = inputBuffer.front();
            if (c == ' ' || c == '\n') {
                inputBuffer.pop_front();
                break;
            }

            buffer[index++] = c;
            inputBuffer.pop_front();
        }

        buffer[index++] = '\0';
        return index - 1;
    }

    bool TCodeEventReader::parseCommand(const char *buffer, const size_t length) {
        TCodeEvent event;
        if (TParser::parseCommand(buffer, length, event)) {
            eventBuffer.push_back(event);
            return true;
        }
        return false;
    }

}