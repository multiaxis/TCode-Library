// TCode-Encoder-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../utils/TString.h"
#include <string>

namespace TCode::Encoding {

    // #<name id>:<value 0 - false or 1 - true> - bool
    // $<name id>:<- if negative><value> - int,long
    // @<name id>:<value> - uint,ulong
    // float,string are not supported currently for encoding
    static bool encodeValue(const char *name, const short &value, char *buffer, const size_t length) {
        size_t index = 0;
        size_t nameLength = strlen(name);
        if (!TString::writeChar('$', index, buffer, length))
            return false;
        for (size_t i = 0; i < nameLength; i++) {
            if (!TString::writeChar(name[i], index, buffer, length))
                return false;
        }

        if (!TString::writeChar(':', index, buffer, length))
            return false;

        if (value < 0) {
            if (!TString::writeChar('-', index, buffer, length))
                return false;
        }

        unsigned short valueabs = value;
        if (value < 0) {
            valueabs = -value;
        }

        if (!TString::writeInt(value, index, buffer, length))
            return false;

        if (!TString::writeChar('\n', index, buffer, length))
            return false;

        return true;
    }

    // #<name id>:<value 0 - false or 1 - true> - bool
    // $<name id>:<- if negative><value> - int,long
    // @<name id>:<value> - uint,ulong
    // float,string are not supported currently for encoding
    static bool encodeValue(const char *name, const unsigned short &value, char *buffer, const size_t length) {
        size_t index = 0;
        size_t nameLength = strlen(name);
        if (!TString::writeChar('@', index, buffer, length))
            return false;
        for (size_t i = 0; i < nameLength; i++) {
            if (!TString::writeChar(name[i], index, buffer, length))
                return false;
        }

        if (!TString::writeChar(':', index, buffer, length))
            return false;

        if (!TString::writeInt(value, index, buffer, length))
            return false;

        if (!TString::writeChar('\n', index, buffer, length))
            return false;

        return true;
    }

    // #<name id>:<value 0 - false or 1 - true> - bool
    // $<name id>:<- if negative><value> - int,long
    // @<name id>:<value> - uint,ulong
    // float,string are not supported currently for encoding
    static bool encodeValue(const char *name, const bool &value, char *buffer, const size_t length) {
        size_t index = 0;
        size_t nameLength = strlen(name);
        if (!TString::writeChar('#', index, buffer, length))
            return false;
        for (size_t i = 0; i < nameLength; i++) {
            if (!TString::writeChar(name[i], index, buffer, length))
                return false;
        }

        if (!TString::writeChar(':', index, buffer, length))
            return false;

        if (value) {
            if (!TString::writeChar('1', index, buffer, length))
                return false;
        } else {
            if (!TString::writeChar('0', index, buffer, length))
                return false;
        }
        if (!TString::writeChar('\n', index, buffer, length))
            return false;
        return true;
    }

    // #<name id>:<value 0 - false or 1 - true> - bool
    // $<name id>:<- if negative><value> - int,long
    // @<name id>:<value> - uint,ulong
    // float,string are not supported currently for encoding

    /*
    bool encodeVariantValue(TaggedDataContainer &data, char *buffer, const size_t length){
        const char* tag = data.getTag();
        switch (data.getDataType()) {
        case VariantType::Boolean: {
            bool value;
            if (!data.getValue(value)) {
                return false;
            }
            return encodeValue(tag, value, buffer, length);
        } break;
        case VariantType::Int8: {
            char value;
            if (!data.getValue(value)) {
                return false;
            }
            return encodeValue(tag, (unsigned short)value, buffer, length);
        }
        case VariantType::Int32: {
            int value;
            if (!data.getValue(value)) {
                return false;
            }
            return encodeValue(tag, (short)value, buffer, length);
        } break;
        case VariantType::Int64: {
            long value;
            if (!data.getValue(value)) {
                return false;
            }
            return encodeValue(tag, (short)value, buffer, length);
        }
        case VariantType::UInt32: {
            unsigned int value;
            if (!data.getValue(value)) {
                return false;
            }
            return encodeValue(tag, (unsigned short)value, buffer, length);
        } break;
        case VariantType::UInt64: {
            unsigned long value;
            if (!data.getValue(value)) {
                return false;
            }
            return encodeValue(tag, (unsigned short)value, buffer, length);
        }
        }
        return false;
    }
    */

}