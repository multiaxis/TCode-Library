// TCode-C-String-Utils-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "TString.h"
#include <stack>

namespace TCode::TString {
    bool isHex(const char value) {
        return ((value >= '0')&&(value <= '9')) || ((value >= 'a') && (value <= 'f')) || ((value >= 'A') && (value <= 'F'));
    }

    uint8_t hexCharToUint8(const char value)
    {
        if((value >= 'a') && (value <= 'f'))
            return (value - 'a') + 10;

        if((value >= 'A') && (value <= 'F'))
            return (value - 'A') + 10;
        
        if((value >= '0') && (value <= '9'))
            return value - '0';

        return 0;
    }

    bool readInt(size_t &&index, const char *buffer, const size_t length, unsigned long &value, size_t &log) {
        return readInt(index, buffer, length, value, log);
    }

    bool readInt(size_t &index, const char *buffer, const size_t length, unsigned long &value, size_t &log) {
        size_t startIndex = index;
        long accum = 0;
        while (isdigit(buffer[index])) {
            accum *= 10;
            accum += toupper(buffer[index]) - '0';
            index++;
        }

        log = index - startIndex;
        if (log == 0)
            return false;

        value = accum;
        return true;
    }

    bool readHexByte(size_t &index, const char *buffer, const size_t length, uint8_t &value) {
        char firstNibble = readCharOrDefault(index++,buffer,length);
        char secondNibble = readCharOrDefault(index++,buffer,length);
        if(!(isHex(firstNibble) && isHex(secondNibble)))
            return false;

        value = (hexCharToUint8(firstNibble) << 4) + hexCharToUint8(secondNibble);
        return true;
    }

    bool readVIntHex(size_t &index, const char *buffer, const size_t length, unsigned long long &value) {
        bool end_terminate = false;
        uint8_t count = 0;
        value = 0;
        while((!end_terminate) && (count <= 9)) {
            uint8_t nextByte;
            if(!readHexByte(index,buffer,length,nextByte))
                break;

            if((nextByte & 0x80) > 0)
                end_terminate = true;

            value |= (nextByte & 0x7F);
            value <<= 7;
            count++;
        }
        return end_terminate;
    }

    bool readTCodeFloat(size_t &&index, const char *buffer, const size_t length, float &value, size_t &log) {
        return readTCodeFloat(index, buffer, length, value, log);
    }

    bool readTCodeFloat(size_t &index, const char *buffer, const size_t length, float &value, size_t &log) {
        unsigned long valueLong;
        if (!readInt(index, buffer, length, valueLong, log))
            return false;

        while (log < 4) {
            valueLong *= 10;
            log++;
        }

        value = TMath::getFloatFromTCode(valueLong, log);
        return true;
    }

    char readCharOrDefault(const size_t index, const char *buffer, const size_t length, const char defaultValue) {
        if (index >= length)
            return defaultValue;

        return buffer[index];
    }

    bool writeChar(const char value, size_t &&index, char *buffer, const size_t length) {
        return writeChar(value, index, buffer, length);
    }

    bool writeChar(const char value, size_t &index, char *buffer, const size_t length) {
        if (index >= length)
            return false;

        buffer[index++] = value;
        return true;
    }

    bool writeString(const String &value, size_t &&index, char *buffer, const size_t length) {
        return writeString(value.c_str(), value.length(), index, buffer, length);
    }

    bool writeString(const char *value, size_t &&index, char *buffer, const size_t length) {
        return writeString(value, strlen(value), index, buffer, length);
    }

    bool writeString(const char *value, const size_t count, size_t &&index, char *buffer, const size_t length) {
        return writeString(value, count, index, buffer, length);
    }

    bool writeString(const String &value, size_t &index, char *buffer, const size_t length) {
        return writeString(value.c_str(), value.length(), index, buffer, length);
    }

    bool writeString(const char *value, size_t &index, char *buffer, const size_t length) {
        return writeString(value, strlen(value), index, buffer, length);
    }

    bool writeString(const char *value, const size_t count, size_t &index, char *buffer, const size_t length) {
        if (index + count >= length)
            return false;

        for (size_t i = 0; i < count; i++)
            buffer[index++] = value[i];

        return true;
    }

    bool writeInt(long value, size_t &&index, char *buffer, const size_t length) {
        return writeInt(value, index, buffer, length);
    }

    bool writeInt(long value, size_t &index, char *buffer, const size_t length) {
        std::stack<char> output;
        if (value == 0)
            output.push('0');

        bool isNegative = value < 0;
        value = abs(value);

        while (value > 0) {
            output.push((value % 10) + '0');
            value /= 10;
        }

        if (isNegative)
            output.push('-');

        if (index + output.size() >= length)
            return false;

        while (output.size() > 0) {
            buffer[index++] = output.top();
            output.pop();
        }

        return true;
    }

    unsigned long getHash(const char *str, size_t length) {
        // implementation of djb2 from http://www.cse.yorku.ca/~oz/hash.html
        char c;
        unsigned long hash = 5381;
        for (int i = 0; c = str[i], i < length; i++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }

    char axisTypeToChar(const Datatypes::AxisType &type) {
        switch (type) {
        case Datatypes::AxisType::Auxiliary:
            return 'A';
        case Datatypes::AxisType::Linear:
            return 'L';
        case Datatypes::AxisType::Rotation:
            return 'R';
        case Datatypes::AxisType::Vibration:
            return 'V';
        default:
            return '?';
        }
    }

    const char *axisTypeToVerboseString(const Datatypes::AxisType &type) {
        switch (type) {
        case Datatypes::AxisType::Auxiliary:
            return "Auxiliary";
        case Datatypes::AxisType::Linear:
            return "Linear";
        case Datatypes::AxisType::Rotation:
            return "Rotation";
        case Datatypes::AxisType::Vibration:
            return "Vibration";
        default:
            return "UNDEFINED";
        }
    }

    String axisIdToString(const Datatypes::AxisId &id) {
        return axisTypeToChar(id.type) + String((int)id.channel);
    }

}
