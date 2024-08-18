// TCode-Data-Writer-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../context/TCodeContext.h"
#include "../datatypes/CommandDataTypes.h"
#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"
#include "../logging/LogHandler.h"
#include "../encoding/TCodeEncoder.h"
#include "../outputstream/outputStreamInterface.h"

#ifndef MAX_ENCODER_BUFFER_LENGTH
#define MAX_ENCODER_BUFFER_LENGTH 512
#endif

namespace TCode::TEncoding {
    template <typename T>
    bool writeTCodeValue(const char *name, const T &value, TCodeContext &context) {
        OutputStreamInterface *outputStream;
        if (!context.getOutputStream(outputStream)) {
            LogHandler::warning("TWriter", "Cannot Encode output, Output Stream is not set");
            return false;
        }

        char buffer[MAX_ENCODER_BUFFER_LENGTH];

        for (size_t i = 0; i < MAX_ENCODER_BUFFER_LENGTH; i++)
            buffer[i] = '\0';

        if (!encodeValue(name, value, buffer, MAX_ENCODER_BUFFER_LENGTH)) {
            LogHandler::error("TEncoder", "Encoding Failed");
            return false;
        }

        outputStream->write(buffer);
        return true;
    }
}