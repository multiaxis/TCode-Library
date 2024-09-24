// TCode-Output-Stream-Interface-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 16/08/2023
// Please copy, share, learn, innovate, give attribution.
//
#pragma once
#include <Arduino.h>

namespace TCode::Output {

    class TCodeIOutputStream {
    public:
        virtual void write(const char value) const = 0;
        virtual void write(const char *value) const = 0;
        virtual void write(const __FlashStringHelper *value) const = 0;
        virtual void write(const String &value) const = 0;
        virtual void print(const char value) const = 0;
        virtual void print(const char *value) const = 0;
        virtual void print(const __FlashStringHelper *value) const = 0;
        virtual void print(const String &value) const = 0;
        virtual void println(const char value) const = 0;
        virtual void println(const char *value) const = 0;
        virtual void println(const __FlashStringHelper *value) const = 0;
        virtual void println(const String &value) const = 0;
    };

};