// TCode-Data-Writer-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../context/TContext.h"
#include "../datatypes/CommandDataTypes.h"
#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"
#include "../encoding/TEncoder.h"
#include "../outputstream/TOutputStreamInterface.h"

#ifndef MAX_ENCODER_BUFFER_LENGTH
#define MAX_ENCODER_BUFFER_LENGTH 512
#endif

namespace TCode::Writing {

    /**
     * @brief Encodes and writes a TCode value to the output stream.
     * 
     * This template function takes a name-value pair and encodes it using the `encodeValue` function. 
     * Once encoded, it writes the resulting data to the output stream set in the provided TCodeContext. 
     * If the output stream is not set or encoding fails, it returns false.
     * 
     * @tparam T The type of the value to be written.
     * @param name The name of the TCode value to be written (e.g., a key or identifier).
     * @param value The value to be encoded and written to the output stream.
     * @param context The TCodeContext that contains references to the output stream.
     * @return true if the value is successfully encoded and written, false if an error occurs (e.g., no output stream or encoding failure).
     * 
     * @note The `context` must have a valid output stream set using `setOutputStream`. If the encoding fails, the function will return false.
     * 
     * Example usage:
     * @code
     * TCodeContext context;
     * context.setOutputStream(myOutputStream);
     * bool success = writeTCodeValue("example", 1234, context);
     * @endcode
     */
    template <typename T>
    bool writeTCodeValue(const char *name, const T &value, TCodeContext &context) {
        Output::TCodeIOutputStream *outputStream;
        if (!context.getOutputStream(outputStream)) {
            // LogHandler::warning("TWriter", "Cannot Encode output, Output Stream is not set");
            return false;
        }

        char buffer[MAX_ENCODER_BUFFER_LENGTH];

        for (size_t i = 0; i < MAX_ENCODER_BUFFER_LENGTH; i++)
            buffer[i] = '\0';

        if (!Encoding::encodeValue(name, value, buffer, MAX_ENCODER_BUFFER_LENGTH)) {
            // LogHandler::error("TEncoder", "Encoding Failed");
            return false;
        }

        outputStream->write(buffer);
        return true;
    }
}