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

    /**
     * @brief The TCodeEventReader class reads and parses input data to extract TCode events.
     *
     * This class is responsible for managing an input buffer and parsing commands to generate
     * `TCodeEvent` objects. It supports multiple input formats (byte, char, string) and maintains
     * an event buffer for parsed events. The class provides methods to parse individual commands
     * or all available commands, retrieve events, and flush the buffers.
     */
    class TCodeEventReader {
    private:
        static const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 128; ///< Maximum length of the command buffer.

        std::deque<char> inputBuffer;                  ///< Buffer to store input data for processing.
        std::deque<Datatypes::TCodeEvent> eventBuffer; ///< Buffer to store parsed TCode events.

        /**
         * @brief Parses a command from the given buffer.
         *
         * This method processes a raw command from the buffer and attempts to parse it
         * into a TCodeEvent. It is used internally to handle command parsing logic.
         *
         * @param buffer The buffer containing the command to be parsed.
         * @param length The length of the command in the buffer.
         * @return true if the command is successfully parsed, false otherwise.
         */
        bool parseCommand(const char *buffer, const size_t length);

        /**
         * @brief Consumes the next command from the input buffer and copies it into the provided buffer.
         *
         * This method extracts the next complete command from the inputBuffer and copies it to the
         * specified buffer for further parsing.
         *
         * @param buffer The buffer to store the next command.
         * @param length The maximum length of the buffer.
         * @return The size of the command copied to the buffer.
         */
        size_t consumeNextCommandFromInputBuffer(char *buffer, const size_t length);

    public:
        /**
         * @brief Reads a single byte input and adds it to the input buffer.
         *
         * This method is used to process and store a byte input into the input buffer for later parsing.
         *
         * @param input The byte input to be read.
         */
        void read(const byte input);

        /**
         * @brief Reads a single character input and adds it to the input buffer.
         *
         * This method processes and stores a character input into the input buffer for later parsing.
         *
         * @param input The character input to be read.
         */
        void read(const char input);

        /**
         * @brief Reads a string input and adds it to the input buffer.
         *
         * This method processes and stores a string input into the input buffer for later parsing.
         *
         * @param input The string input to be read.
         */
        void read(const String &input);

        /**
         * @brief Reads a C-string input and adds it to the input buffer.
         *
         * This method processes and stores a C-string input into the input buffer for later parsing.
         *
         * @param input The C-string input to be read.
         */
        void read(const char *input);

        /**
         * @brief Parses the next available command in the input buffer.
         *
         * This method attempts to parse the next command in the input buffer and generates a `TCodeEvent`
         * if successful. It returns true if parsing succeeds.
         *
         * @return true if the next command is successfully parsed, false otherwise.
         */
        bool parseNext();

        /**
         * @brief Parses all available commands in the input buffer.
         *
         * This method processes and parses all remaining commands in the input buffer and generates
         * `TCodeEvent` objects for each parsed command.
         *
         * @return true if at least one command is successfully parsed, false otherwise.
         */
        bool parseAll();

        /**
         * @brief Retrieves the next parsed TCode event from the event buffer.
         *
         * This method extracts the next available `TCodeEvent` from the event buffer if any are present.
         * It returns true if an event is successfully retrieved.
         *
         * @param event A reference to a `TCodeEvent` object that will be populated with the next event.
         * @return true if an event is successfully retrieved, false otherwise.
         */
        bool getNext(Datatypes::TCodeEvent &event);

        /**
         * @brief Flushes both the input and event buffers.
         *
         * This method clears all stored data in both the input and event buffers, resetting the reader state.
         */
        void flush();

        /**
         * @brief Constructs a new TCodeEventReader object.
         *
         * Initializes the input and event buffers and sets up the reader for event processing.
         */
        TCodeEventReader();

        /**
         * @brief Destroys the TCodeEventReader object.
         *
         * Cleans up resources used by the TCodeEventReader, if any.
         */
        ~TCodeEventReader();
    };

}