// TCode-Encoder-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../utils/TString.h"
#include <string>

namespace TCode::Encoding {
    /**
     * @brief Encodes a short integer value as a TCode string and writes it to the buffer.
     *
     * The encoding format for signed integers is:
     * $<name id>:<- if negative><value>
     *
     * The encoded value will be prefixed by the `$` symbol, followed by the name identifier, and then the value.
     * A negative sign is included if the value is negative. The encoded string ends with a newline character.
     *
     * @param name The name identifier for the value to be encoded.
     * @param value The short integer value to be encoded.
     * @param buffer The output buffer to hold the encoded string.
     * @param length The maximum length of the buffer.
     * @return true if encoding is successful and the value fits within the buffer, false otherwise.
     *
     * @note The buffer is populated by writing individual characters using `TString::writeChar` and the integer value using `TString::writeInt`.
     */
    static bool encodeValue(const char *name, const short &value, char *buffer, const size_t length) {
        size_t index = 0;
        size_t nameLength = strlen(name);

        // Write the leading character for signed integers ('$')
        if (!TString::writeChar('$', index, buffer, length))
            return false;

        // Write the name identifier
        for (size_t i = 0; i < nameLength; i++) {
            if (!TString::writeChar(name[i], index, buffer, length))
                return false;
        }

        // Write the separator character ':'
        if (!TString::writeChar(':', index, buffer, length))
            return false;

        // Write negative sign if the value is negative
        if (value < 0) {
            if (!TString::writeChar('-', index, buffer, length))
                return false;
        }

        unsigned short valueabs = value < 0 ? -value : value;

        // Write the value itself
        if (!TString::writeInt(value, index, buffer, length))
            return false;

        // Write the terminating newline character
        if (!TString::writeChar('\n', index, buffer, length))
            return false;

        return true;
    }

    /**
     * @brief Encodes an unsigned short integer value as a TCode string and writes it to the buffer.
     *
     * The encoding format for unsigned integers is:
     * @<name id>:<value>
     *
     * The encoded value will be prefixed by the `@` symbol, followed by the name identifier, and then the value.
     * The encoded string ends with a newline character.
     *
     * @param name The name identifier for the value to be encoded.
     * @param value The unsigned short integer value to be encoded.
     * @param buffer The output buffer to hold the encoded string.
     * @param length The maximum length of the buffer.
     * @return true if encoding is successful and the value fits within the buffer, false otherwise.
     */
    static bool encodeValue(const char *name, const unsigned short &value, char *buffer, const size_t length) {
        size_t index = 0;
        size_t nameLength = strlen(name);

        // Write the leading character for unsigned integers ('@')
        if (!TString::writeChar('@', index, buffer, length))
            return false;

        // Write the name identifier
        for (size_t i = 0; i < nameLength; i++) {
            if (!TString::writeChar(name[i], index, buffer, length))
                return false;
        }

        // Write the separator character ':'
        if (!TString::writeChar(':', index, buffer, length))
            return false;

        // Write the value itself
        if (!TString::writeInt(value, index, buffer, length))
            return false;

        // Write the terminating newline character
        if (!TString::writeChar('\n', index, buffer, length))
            return false;

        return true;
    }

    /**
     * @brief Encodes a boolean value as a TCode string and writes it to the buffer.
     *
     * The encoding format for boolean values is:
     * #<name id>:<value 0 - false or 1 - true>
     *
     * The encoded value will be prefixed by the `#` symbol, followed by the name identifier, and then the boolean value (0 or 1).
     * The encoded string ends with a newline character.
     *
     * @param name The name identifier for the value to be encoded.
     * @param value The boolean value to be encoded (0 for false, 1 for true).
     * @param buffer The output buffer to hold the encoded string.
     * @param length The maximum length of the buffer.
     * @return true if encoding is successful and the value fits within the buffer, false otherwise.
     */
    static bool encodeValue(const char *name, const bool &value, char *buffer, const size_t length) {
        size_t index = 0;
        size_t nameLength = strlen(name);

        // Write the leading character for booleans ('#')
        if (!TString::writeChar('#', index, buffer, length))
            return false;

        // Write the name identifier
        for (size_t i = 0; i < nameLength; i++) {
            if (!TString::writeChar(name[i], index, buffer, length))
                return false;
        }

        // Write the separator character ':'
        if (!TString::writeChar(':', index, buffer, length))
            return false;

        // Write the boolean value (1 for true, 0 for false)
        if (value) {
            if (!TString::writeChar('1', index, buffer, length))
                return false;
        } else {
            if (!TString::writeChar('0', index, buffer, length))
                return false;
        }

        // Write the terminating newline character
        if (!TString::writeChar('\n', index, buffer, length))
            return false;

        return true;
    }
};